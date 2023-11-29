 #pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	// Metodos get
	float getVistaActualX() { return yaw; }
	float getVistaActualY() { return pitch; }
	float getVistaActualZ() { }
	float getPosActualX() { return position.x; }
	float getPosActualY() { return position.y; }
	float getPosActualZ() { return position.z; }

	// Metodos set
	void setVistaActualX(GLfloat vistaActualX) { yaw = vistaActualX; }
	void setVistaActualY(GLfloat vistaActualY) { pitch = vistaActualY; }
	void setVistaActualZ() { }
	void setPosActualX(float posActualX) { position.x = posActualX; }
	void setPosActualY(float posActualY) { position.y = posActualY; }
	void setPosActualZ(float posActualZ) { position.z = posActualZ; }

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

