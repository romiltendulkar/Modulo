PAnimScript = function(AnimationType, Counter)
{
	NewCounter = Counter - 1;
	SpriteFile = "RobotStill.png";
	if AnimationType == 1
	then
		if Counter == 1
		then
			SpriteFile = "RobotStill.png";		
		end
	end
	return SpriteFile, NewCounter
}