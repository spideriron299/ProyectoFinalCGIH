#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevey() { return muevey; }
	GLfloat getmuevez() { return muevez; }
	GLfloat getmueveRot() { return mueveRot; }
	GLfloat getmueveEscala() { return escala; }
	GLboolean moverFlipper1() { return estadoFlipper1; }
	GLboolean moverFlipper2() { return estadoFlipper2; }
	GLboolean moverFlipper3() { return estadoFlipper3; }
	GLboolean mueveEnX() { return movX; }
	GLboolean mueveEnMenosX() { return movMenosX; }
	GLboolean mueveEnZ() { return movZ; }
	GLboolean mueveEnMenosZ() { return movMenosZ; }
	GLboolean moverResorte() { return estadoResorte; }
	GLfloat getIsometrica() { return isometrica; };
	GLint activar() { return activador; }
	GLint activarLuzFlippe() { return LuzFlippe; }
	GLint activarAvatar() { return LuzAvatar; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat muevey;
	GLfloat muevez;
	GLfloat mueveRot;
	GLfloat escala;
	GLboolean estadoFlipper1;
	GLboolean estadoFlipper2;
	GLboolean estadoFlipper3;
	GLboolean estadoResorte;
	GLboolean movX;
	GLboolean movMenosX;
	GLboolean movZ;
	GLboolean movMenosZ;
	GLint activador;
	GLint LuzFlippe;
	GLint LuzAvatar;
	GLfloat isometrica = 0.0f;
	
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
	static void ManejaMouseBotones(GLFWwindow* window, int botones, int accion, int modos);

};

