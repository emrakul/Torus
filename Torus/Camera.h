#ifndef CAMERA_H
#define CAMERA_H

#include "Utility.h"

using namespace glm;

struct Camera{

	vec3 position;
	vec3 direction;

	vec3 upVector;
	vec3 viewVector;
	vec3 strVector;
	vec3 previousPosition;
	vec3 previousDirection;


	mat4 viewMatrix;
	mat4 projectionMatrix;	
	mat4 inverseMatrix;
	mat4 previousViewMatrix;

	float zNear;
	float zFar;
	float kSpeed;
	float currentSpeed;
	Camera();

	vec3 Position() { return position; }
	vec3 View()     { return viewVector; }
	vec3 UpVector() { return upVector; }
	vec3 Strafe()   { return strVector; }



	void MoveCamera(float speed);

	void RotateView(float angle, float x, float y, float z);
	void SetViewByMouse();
	void StrafeCamera(float speed);
	void CheckForMovement();
	void Update();
	void Look();
};

#endif