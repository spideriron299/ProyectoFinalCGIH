#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 0.0f;
	muevey = 0.0f;
	muevez = 0.0f;
	mueveRot = 0.0f;
	escala = 0.0f;
	estadoFlipper1 = false;
	estadoFlipper2 = false;
	estadoFlipper3 = false;
	estadoResorte = false;
	movX = false;
	movMenosX = false;
	movZ = false;
	movMenosZ = false;
	activador = 0;
	LuzFlippe = 0;
	LuzAvatar = 0;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final Lab CGIH", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
	glfwSetMouseButtonCallback(mainWindow, ManejaMouseBotones);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}


static bool keyDown = false;
static bool keyDown2 = false;
static bool keyDown3 = false;
static bool keyDown4 = false;
void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	
	//Para que el personaje active su animacion
	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
		{
			keyDown = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keyDown = false;
		}
	}

	if (keyDown)
	{
		theWindow->movX = true;
	}
	else
	{
		theWindow->movX = false;
	}

	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS)
		{
			keyDown2 = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keyDown2 = false;
		}
	}

	if (keyDown2)
	{
		theWindow->movMenosX = true;
	}
	else
	{
		theWindow->movMenosX = false;
	}


	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS)
		{
			keyDown3 = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keyDown3 = false;
		}
	}

	if (keyDown3)
	{
		theWindow->movZ = true;
	}
	else
	{
		theWindow->movZ = false;
	}

	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS)
		{
			keyDown4 = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keyDown4 = false;
		}
	}

	if (keyDown4)
	{
		theWindow->movMenosZ = true;
	}
	else
	{
		theWindow->movMenosZ = false;
	}



	if (key == GLFW_KEY_D)
	{
		theWindow-> muevex += 0.1;
	}
	if (key == GLFW_KEY_A)
	{
		theWindow-> muevex -= 0.1;
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->muevey += 0.25;
	}
	if (key == GLFW_KEY_G)
	{
		theWindow->muevey -= 0.25;
	}

	if (key == GLFW_KEY_W)
	{
		theWindow->muevez += 0.1;
	}
	if (key == GLFW_KEY_S)
	{
		theWindow->muevez -= 0.1;
	}


	// Boton Flipper1
	if (key == GLFW_KEY_Z)
	{
		if (theWindow->estadoFlipper1 == true)
		{
			theWindow->estadoFlipper1 = false;
		}
		else
		{
			theWindow->estadoFlipper1 = true;
		}
	}
	//Boton Flipper2
	if (key == GLFW_KEY_X)
	{
		if (theWindow->estadoFlipper2 == true)
		{
			theWindow->estadoFlipper2 = false;
		}
		else
		{
			theWindow->estadoFlipper2 = true;
		}
	}	
	//Boton Flipper3
	if (key == GLFW_KEY_C)
	{
		if (theWindow->estadoFlipper3 == true)
		{
			theWindow->estadoFlipper3 = false;
		}
		else
		{
			theWindow->estadoFlipper3 = true;
		}
	}

	// teclas para la camara isometrica
	if (key == GLFW_KEY_I) {
		theWindow->isometrica = 1.0f;
	}
	else if (key == GLFW_KEY_O) {
		theWindow->isometrica = 0.0f;
	}

	//Activador para animacion de la canica
	if (key == GLFW_KEY_M)
	{
		theWindow->activador = 1;
	}

	//Activador para luzFlipper
	if (key == GLFW_KEY_N)
	{
		{
			theWindow->LuzFlippe = true;
		}
	}
	if (key == GLFW_KEY_J)
	{
		{
			theWindow->LuzFlippe = false;
		}
	}


	//Activador para la luz avatar
	if (key == GLFW_KEY_V)
	{
		{
			theWindow->LuzAvatar = true;
		}
	}
	if (key == GLFW_KEY_B)
	{
		{
			theWindow->LuzAvatar = false;
		}
	}


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}


}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

}

void Window::ManejaMouseBotones(GLFWwindow* window, int botones, int accion, int modos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (botones == GLFW_MOUSE_BUTTON_RIGHT )
	{
		if (theWindow->estadoResorte == true)
		{
			theWindow->estadoResorte = false;
		}
		else
		{
			theWindow->estadoResorte = true;
		}
	}

}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
