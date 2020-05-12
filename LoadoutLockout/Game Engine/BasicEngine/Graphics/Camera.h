#include "stdafx.h"

#pragma once
class Body;
class GameObject;

class Camera
{
public:
	Camera(float x = 0, float y = 0, float x_pos=0, float y_pos=0,
		bool _visable_view= false, float w = 0, float h = 0);
	~Camera();
	Body* view_space;
	Body* camera_space;
	void SetCameraSize(float width, float height);
	void MoveCamera(float x, float y, bool lerp =false);
	void Update(float dt);
	void ResetCameraToOrigin();
	void ResetCameraToPoint(glm::vec4 p);
	void SetCamera(float width, float height, float x, float y, bool _visable_view = false);
	bool IsVisable(GameObject* go);
	bool IsInCamera(GameObject* go);
	void SetCameraType(std::string type);
	glm::mat4 GetViewMatrix();
private:
	bool visable_view;
	std::string cam_type;
	glm::mat4 view;
	float cam_box_x, cam_box_y, cam_speed;
	glm::vec4 move_to;
};

