#include "stdafx.h"
#include "Audio_Engine.h"

Implementation::Implementation() {
	mpStudioSystem = NULL;
	Audio_Engine::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
	Audio_Engine::ErrorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	mpSystem = NULL;
	Audio_Engine::ErrorCheck(mpStudioSystem->getLowLevelSystem(&mpSystem));
}

Implementation::~Implementation() {
	Audio_Engine::ErrorCheck(mpStudioSystem->unloadAll());
	Audio_Engine::ErrorCheck(mpStudioSystem->release());
	mChannels.clear();
	mSounds.clear();
}

void Implementation::Update() {
	vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels)
	{
		mChannels.erase(it);
	}
	Audio_Engine::ErrorCheck(mpStudioSystem->update());
}

Implementation* sgpImplementation = nullptr;

Audio_Engine::Audio_Engine()
{
}


Audio_Engine::~Audio_Engine()
{
	Shutdown();
	
}

void Audio_Engine::Init() {
	sgpImplementation = new Implementation;
}

void Audio_Engine::Update() {
	sgpImplementation->Update();
}

void Audio_Engine::Loadsound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt != sgpImplementation->mSounds.end())
		return;

	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* pSound = nullptr;
	Audio_Engine::ErrorCheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) {
		sgpImplementation->mSounds[strSoundName] = pSound;
	}

}

void Audio_Engine::UnLoadSound(const std::string& strSoundName)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
		return;

	Audio_Engine::ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
}

int Audio_Engine::PlaySound(const string& strSoundName, const Vector3& vPosition, float fVolumedB)
{
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
	{
		Loadsound(strSoundName);
		tFoundIt = sgpImplementation->mSounds.find(strSoundName);
		if (tFoundIt == sgpImplementation->mSounds.end())
		{
			return nChannelId;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	Audio_Engine::ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFmod(vPosition);
			Audio_Engine::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		Audio_Engine::ErrorCheck(pChannel->setVolume((fVolumedB)));
		Audio_Engine::ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;
	}
	return nChannelId;
}

void Audio_Engine::StopAllChannels()
{
	
}

void Audio_Engine::SetChanneledPosition(int nChannelId, const Vector3& vPosition)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	Audio_Engine::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void Audio_Engine::SetMusicvolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	Audio_Engine::ErrorCheck(tFoundIt->second->setVolume(fVolumedB));
}

void Audio_Engine::SetSFXvolume(int nChannelId, float fVolumedB)
{
	for (auto aP = sgpImplementation->mChannels.begin(); aP != sgpImplementation->mChannels.end(); aP++)
	{
		if (aP->first != nChannelId)
		{
			Audio_Engine::ErrorCheck(aP->second->setVolume(fVolumedB));
		}
	}
	/*auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	Audio_Engine::ErrorCheck(tFoundIt->second->setVolume(fVolumedB));*/
}

void Audio_Engine::StopChannel(int ChannelID)
{
	/*auto tFoundIt = sgpImplementation->mChannels.find(ChannelID);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;
	Audio_Engine::ErrorCheck(tFoundIt->second->stop());*/

	for (auto tFoundIt : sgpImplementation->mChannels)
	{
		auto channel = tFoundIt.second;
		Audio_Engine::ErrorCheck(channel->stop());
	}
	
}

void Audio_Engine::PauseChannel(int ChannelID)
{
	/*cout << ChannelID << endl;
	auto tFoundIt = sgpImplementation->mChannels.find(ChannelID);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	Audio_Engine::ErrorCheck(tFoundIt->second->setPaused(true));*/

	for (auto tFoundIt : sgpImplementation->mChannels)
	{
		auto channel = tFoundIt.second;
		Audio_Engine::ErrorCheck(channel->setPaused(true));
	}
}

void Audio_Engine::ResumeChannel(int ChannelID)
{
	/*auto tFoundIt = sgpImplementation->mChannels.find(ChannelID);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	Audio_Engine::ErrorCheck(tFoundIt->second->setPaused(false));*/

	for (auto tFoundIt : sgpImplementation->mChannels)
	{
		auto channel = tFoundIt.second;
		Audio_Engine::ErrorCheck(channel->setPaused(false));
	}
}

void Audio_Engine::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
	auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
	if (tFoundIt != sgpImplementation->mBanks.end())
		return;
	FMOD::Studio::Bank* pBank;
	Audio_Engine::ErrorCheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
	if (pBank) {
		sgpImplementation->mBanks[strBankName] = pBank;
	}
}

void Audio_Engine::LoadEvent(const std::string& strEventName) {
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit != sgpImplementation->mEvents.end())
		return;
	FMOD::Studio::EventDescription* pEventDescription = NULL;
	Audio_Engine::ErrorCheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
	if (pEventDescription) {
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		Audio_Engine::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance) {
			sgpImplementation->mEvents[strEventName] = pEventInstance;
		}
	}
}

void Audio_Engine::PlayEvent(const string &strEventName) {
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit == sgpImplementation->mEvents.end()) {
		LoadEvent(strEventName);
		tFoundit = sgpImplementation->mEvents.find(strEventName);
		if (tFoundit == sgpImplementation->mEvents.end())
			return;
	}
	tFoundit->second->start();
}

void Audio_Engine::StopEvent(const string &strEventName, bool bImmediate) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	Audio_Engine::ErrorCheck(tFoundIt->second->stop(eMode));
}

bool Audio_Engine::IsEventPlaying(const string &strEventName) const {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return false;

	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
		return true;
	}
	return false;
}

void Audio_Engine::GeteventParameter(const string &strEventName, const string &strParameterName, float* parameter) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	Audio_Engine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	Audio_Engine::ErrorCheck(pParameter->getValue(parameter));
}

void Audio_Engine::SetEventParameter(const string &strEventName, const string &strParameterName, float fValue) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	Audio_Engine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	Audio_Engine::ErrorCheck(pParameter->setValue(fValue));
}

FMOD_VECTOR Audio_Engine::VectorToFmod(const Vector3& vPosition) {
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

float  Audio_Engine::dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float  Audio_Engine::VolumeTodb(float volume)
{
	return 20.0f * log10f(volume);
}

int Audio_Engine::ErrorCheck(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		cout << "FMOD ERROR " << result << endl;
		return 1;
	}
	// cout << "FMOD all good" << endl;
	return 0;
}

void Audio_Engine::Shutdown() {
	delete sgpImplementation;
}

int Audio_Engine::SendChannelId()
{
	return sgpImplementation->mnNextChannelId;
}
