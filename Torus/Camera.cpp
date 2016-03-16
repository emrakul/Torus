#include "Camera.h"
#include "main.h"

Camera::Camera() {
	position = vec3(0, 0, 0);
	viewVector = vec3(1, 0, 0);
	upVector = vec3(0, 1, 0);
	zNear = 0.5;
	zFar = 50;
	viewMatrix = lookAt(viewVector, position, upVector);
	projectionMatrix = perspective<float>(67.3801, (GLfloat)1600/(GLfloat)900, zNear, zFar);
	

	inverseMatrix = inverse(projectionMatrix * viewMatrix);
	previousPosition = position;
	previousDirection = direction;

	previousViewMatrix = viewMatrix;
	kSpeed = 0.05;
	currentSpeed = 0.001;


}


void Camera::RotateView(float angle, float x, float y, float z)
{
	vec3 vNewView;
	vec3 vView;


	vView.x = viewVector.x - position.x;
	vView.y = viewVector.y - position.y;
	vView.z = viewVector.z - position.z;


	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);


	vNewView.x = (cosTheta + (1 - cosTheta) * x * x)   * vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)   * vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)   * vView.z;


	vNewView.y = ((1 - cosTheta) * x * y + z * sinTheta)   * vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)   * vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)   * vView.z;


	vNewView.z = ((1 - cosTheta) * x * z - y * sinTheta)   * vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)   * vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)   * vView.z;


	viewVector.x = position.x + vNewView.x;
	viewVector.y = position.y + vNewView.y;
	viewVector.z = position.z + vNewView.z;
}




void Camera::MoveCamera(float speed)
{
	vec3 vVector = viewVector - position;
	//vVector.y = 1.0f;
	vVector = normalize(vVector);
	position.x += vVector.x * speed;
	position.z += vVector.z * speed;
	position.y += vVector.y * speed;

	viewVector.x += vVector.x * speed;
	viewVector.z += vVector.z * speed;
	viewVector.y += vVector.y * speed;

}


void Camera::SetViewByMouse()
{
	POINT mousePos;


	int middleX = 1600 >> 1;
	int middleY = 900 >> 1;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	static float currentRotX = 0.0f;

	//glfwGetCursorPos (GLFWwindow *window, double *xpos, double *ypos);//
	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;
	lastRotX = currentRotX;

	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;
		if (lastRotX != 1.0f)
		{
			vec3 vAxis = cross(viewVector - position, upVector);
			vAxis = normalize(vAxis);
			RotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}


	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		if (lastRotX != -1.0f)
		{
			vec3 vAxis = cross(viewVector - position, upVector);
			vAxis = normalize(vAxis);
			RotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	else
	{
		vec3 vAxis = cross(viewVector - position, upVector);
		vAxis = normalize(vAxis);
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	RotateView(angleY, 0, 1, 0);
}


void Camera::StrafeCamera(float speed)
{

	position.x += strVector.x * speed*0.5;
	position.z += strVector.z * speed*0.5;


	viewVector.x += strVector.x * speed*0.5;
	viewVector.z += strVector.z * speed*0.5;
}





void Camera::CheckForMovement()
{
	float speed = kSpeed;

	if (GetKeyState('W') & 0x80) {

		MoveCamera(speed);
	}
	// ���� ��� S
	if (GetKeyState('S') & 0x80) {
		MoveCamera(-speed);
	}

	if (GetKeyState('A') & 0x80) {
		StrafeCamera(-speed);
	}

	if (GetKeyState('D') & 0x80) {
		StrafeCamera(speed);
	}


}

void Camera::Update()
{
	vec3 vCross = cross(viewVector - position, upVector);
	strVector = normalize(vCross);
	//position.y -= currentSpeed;
	//viewVector.y -= currentSpeed;
	//if(currentSpeed == 0.0)
	//	currentSpeed = 0.005;
	//currentSpeed += 0.01*currentSpeed;

	previousViewMatrix = viewMatrix;
	previousDirection = direction;
	previousPosition = position;


	SetViewByMouse();
	CheckForMovement();


	viewMatrix = glm::lookAt(viewVector, position, upVector);
	inverseMatrix = inverse(projectionMatrix * viewMatrix);





	direction = viewVector - position;

}

