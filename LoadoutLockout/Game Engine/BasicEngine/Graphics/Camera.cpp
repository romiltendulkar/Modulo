#include "stdafx.h"
#include "Camera.h"
#include "..\State\GameState.h"
#include "..\Components\Body.h"
#include "..\Managers\CollisionManager.h"

extern GameState* current_state;
extern GraphicsManager* pGfx;
#define BUFFER 1.7

Camera::Camera(float x, float y, float x_pos, float y_pos, 
	bool _visable_view, float w, float h)
{
	visable_view = _visable_view;
	view = glm::lookAt(
		glm::vec3(x_pos, y_pos, 100),// Camera is at (4,3,3), in World Space
		glm::vec3(x_pos, y_pos, 0), // 
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	camera_space = new Body();
	camera_space->pos.x = x_pos;
	camera_space->pos.y = y_pos;
	move_to = camera_space->pos;
	Rect* rec = new Rect();
	rec->width = x;
	rec->height = y;
	camera_space->shape = rec;
	camera_space->ridged = false;
	camera_space->shape->pOwnerBody = camera_space;
	cam_box_x = x / 4.f;
	cam_box_y = y / 4.f;
}


Camera::~Camera()
{
	delete camera_space;
}

void Camera::SetCameraSize(float width, float height)
{

	float w = width;
	float h = height;

	cam_box_x = w /4.f;
	cam_box_y = h / 4.f;

	Rect* rec = static_cast<Rect*>(camera_space->shape);
	rec->width = w*BUFFER;
	rec->height = h*1.25f *BUFFER;
	camera_space->shape = rec;
	
}

void Camera::MoveCamera(float x, float y, bool lerp)
{
	float move_y = 0;
	float move_x =0;
	if (current_state->paused) return;
	camera_space->prev_pos = camera_space->pos;
	if (x < camera_space->pos.x - cam_box_x) {
			float cam_w = (camera_space->pos.x - cam_box_x/2.f);
			move_x = x - cam_w;
	}
	else if(x > camera_space->pos.x +cam_box_x) {
			float cam_w = (camera_space->pos.x + cam_box_x/2.f);
			move_x = x - cam_w;
	} else if (y < camera_space->pos.y) {
		float cam_h = (camera_space->pos.y);
		move_y = y - cam_h;
		camera_space->pos.y = y;
	}
	else if (y > camera_space->pos.y + cam_box_y) {
		float cam_h = (camera_space->pos.y + cam_box_y/2.f);
		move_y = y - cam_h;
	}
	move_to.x = x;
	move_to.y = y;

	//move_to = camera_space->pos + move_to;
	// = move_to;
	
}

void Camera::Update(float dt)
{
	if (current_state->paused) return;
	if (camera_space->pos != move_to) {
		glm::vec4 diff = move_to - camera_space->pos;
		camera_space->pos.x += diff.x * dt;
		camera_space->pos.y += diff.y * dt;
		view = glm::lookAt(
				glm::vec3(camera_space->pos.x, camera_space->pos.y, 10),// Camera is at (4,3,3), in World Space
				glm::vec3(camera_space->pos.x, camera_space->pos.y, 0), // 
				glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);

	}
}

void Camera::ResetCameraToOrigin()
{
	ResetCameraToPoint(vec4(0));
}


void Camera::ResetCameraToPoint(glm::vec4 p)
{
	camera_space->pos.x = p.x;
	camera_space->pos.y = p.y;
	move_to.x = p.x;
	move_to.y = p.y;
	view = glm::lookAt(
		glm::vec3(camera_space->pos.x, camera_space->pos.y, 100),// Camera is at (4,3,3), in World Space
		glm::vec3(camera_space->pos.x, camera_space->pos.y, 0), // 
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
}


void Camera::SetCamera(float width, float height, float x, float y, bool _visable_view)
{
	this->visable_view = _visable_view;
	SetCameraSize(width, height);
	MoveCamera(x, y);
}

bool Camera::IsVisable(GameObject * go)
{
	return IsInCamera(go);
}

void Camera::SetCameraType(std::string type) {
	cam_type = type;
}

bool Camera::IsInCamera(GameObject * go)
{
	if (go == nullptr)
		return false;
	Body* other = static_cast<Body*>(go->GetComponent(BODY));
	if (nullptr == other) return false;
int prev_size = current_state->pCM->mContacts.size();
	current_state->pCM->CheckCollision(
		camera_space->shape, camera_space->pos.x, camera_space->pos.y,
		other->shape, other->pos.x, other->pos.y);
	int new_size = current_state->pCM->mContacts.size();
	return new_size > prev_size;
}

glm::mat4 Camera::GetViewMatrix()
{
	return view;
}
