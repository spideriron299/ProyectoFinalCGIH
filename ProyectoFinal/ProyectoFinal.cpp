/* Semestre 2024-1
Animación por keyframes
La textura del skybox fue conseguida desde la página https://opengameart.org/content/elyvisions-skyboxes?page=1
y edité en Gimp rotando 90 grados en sentido antihorario la imagen  sp2_up.png para poder ver continuidad.
Fuentes :
	https ://www.khronos.org/opengl/wiki/Keyframe_Animation
	http ://what-when-how.com/wp-content/uploads/2012/07/tmpcd0074_thumb.png
	*/
	//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

//Para el audio
#include "irrKlang.h"
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


const float toRadians = 3.14159265f / 180.0f;


//variables para keyframes
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

int cambioNumero = 0;
int cambioNumero1 = 1;
int cambioNumero2 = 2;
int cambioNumero3 = 3;
int cambioNumero4 = 4;
int cambioNumero5 = 5;
int cambioNumero6 = 6;
int cambioNumero7 = 7;
int cambioNumero8 = 8;

float rotCamara = 0.0f;
int rotCamaraAux = 0;

int numero = 000000000;

float gradosFlipper1 = 213;
float auxGradosFlipper1 = 0.0f;
int auxFlipper1 = 0, moverFlipper1 = 0;

float gradosFlipper2 = 213;
float auxGradosFlipper2 = 0.0f;
int auxFlipper2 = 0, moverFlipper2 = 0;

float gradosFlipper3 = 140;
float auxGradosFlipper3 = 0.0f;
int auxFlipper3 = 0, moverFlipper3 = 0;

float angAux0 = 0.0f;
float angAux1 = 0.0f;
float angAux2 = 0.0f;

int varAngAux0 = 0;
int varAngAux1 = 0;
int varAngAux2 = 0;

float zEnResorte = 50.0f;
float auxResorte = 0.0f;
int moverResorte = 0;
int varAuxResorte = 0;

float canica1X = 0.0f;
float canica1Y = 0.0f;
float canica1Z = 0.0f;
int varAuxCanica1 = 0;


bool ciclioDia;

// Variables para posicion y a donde mira la camara
// Declaramos posiciones y vistas iniciales
float vistaInicialX;
float vistaInicialY;
float vistaInicialZ;
float posInicialX;
float posInicialY;
float posInicialZ;

// Declaramos e inicializamos 
float vistaFinalX = -90.0f;
float vistaFinalY = -45.0f;
float posFinalX = 0.0f;
float posFinalY = 130.0f;
float posFinalZ = 80.0f;

// incPos de nuestras variables para pos y vista (velocidad)
float incPos = 0.3f;
float incVista = 0.3f;
// Nos ayudara a asignar valores al set
float auxCam;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture Numero3Texture;
Texture Numero4Texture;
Texture Numero5Texture;
Texture Numero6Texture;
Texture Numero7Texture;
Texture Numero8Texture;
Texture Numero9Texture;
Texture Numero0Texture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
//////////////
Model granada;
Model katana;
Model pinball;
Model canica;
Model torso_cabeza;
Model pierna_izquierda;
Model pierna_derecha;
Model brazo_izquierdo;
Model brazo_derecho;
Model caja;
Model resorte;
Model palanca;
Model base_camara;
Model camara;
//////////////

Skybox skybox;

Skybox skybox2;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////

bool animacion = false;

//NEW// Keyframes
////////////////////////////////////////////////////////////
float posXcanica = -13.0, posYcanica = 46.0, posZcanica = 37.0;
float movCanica_x = 0.0f, movCanica_y = 0.0f, movCanica_z = 0.0f;
////////////////////////////////////////////////////////////
float giroAvion = 0;

#define MAX_FRAMES 100
int i_max_steps = 90;
////////////////////
int i_curr_steps = 8;
////////////////////
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	/////////////////////////////////////////////////////////
	float movCanica_x;		//Variable para PosicionX
	float movCanica_z;		//Variable para PosicionY
	float movCanica_xInc;		//Variable para IncrementoX
	float movCanica_zInc;		//Variable para IncrementoY
	/////////////////////////////////////////////////////////
	float giroAvion;
	float giroAvionInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
///////////////////////////////////////////////
int FrameIndex = 12;			//introducir datos
///////////////////////////////////////////////
bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{
	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].movCanica_x = movCanica_x;
	KeyFrame[FrameIndex].movCanica_z = movCanica_z;
	KeyFrame[FrameIndex].giroAvion = giroAvion;//completar
	//no volatil, agregar una forma de escribir a un archivo para guardar los frames
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{
	//////////////////////////////////////
	movCanica_x = KeyFrame[0].movCanica_x;
	movCanica_z = KeyFrame[0].movCanica_z;
	//////////////////////////////////////
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movCanica_xInc = (KeyFrame[playIndex + 1].movCanica_x - KeyFrame[playIndex].movCanica_x) / i_max_steps;
	KeyFrame[playIndex].movCanica_zInc = (KeyFrame[playIndex + 1].movCanica_z - KeyFrame[playIndex].movCanica_z) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;
}


void animate(void)
{
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex )	//Fin de toda la animación con último frame?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolación del próximo cuadro
			{

				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			movCanica_x += KeyFrame[playIndex].movCanica_xInc;
			movCanica_z += KeyFrame[playIndex].movCanica_zInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

///////////////* FIN KEYFRAMES*////////////////////////////



int main()
{
	mainWindow = Window(1280, 1024); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	Numero3Texture = Texture("Textures/numero3.tga");
	Numero3Texture.LoadTextureA();
	Numero4Texture = Texture("Textures/numero4.tga");
	Numero4Texture.LoadTextureA();
	Numero5Texture = Texture("Textures/numero5.tga");
	Numero5Texture.LoadTextureA();
	Numero6Texture = Texture("Textures/numero6.tga");
	Numero6Texture.LoadTextureA();
	Numero7Texture = Texture("Textures/numero7.tga");
	Numero7Texture.LoadTextureA();
	Numero8Texture = Texture("Textures/numero8.tga");
	Numero8Texture.LoadTextureA();
	Numero9Texture = Texture("Textures/numero9.tga");
	Numero9Texture.LoadTextureA();
	Numero0Texture = Texture("Textures/numero0.tga");
	Numero0Texture.LoadTextureA();	

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	///////////////////////////////////////////////////
	katana = Model();
	katana.LoadModel("Models/katana3.obj");
	granada = Model();
	granada.LoadModel("Models/granada.obj");
	pinball = Model();
	pinball.LoadModel("Models/pinball.obj");
	canica = Model();
	canica.LoadModel("Models/canica.obj");
	caja = Model();
	caja.LoadModel("Models/caja.obj");
	resorte = Model();
	resorte.LoadModel("Models/resorte.obj");
	palanca = Model();
	palanca.LoadModel("Models/palanca_pinball.obj");
	
	camara = Model();
	camara.LoadModel("Models/camara.obj");
	base_camara = Model();
	base_camara.LoadModel("Models/base_camara.obj");

	torso_cabeza = Model();
	torso_cabeza.LoadModel("Models/torso_cabeza.obj");
	pierna_izquierda = Model();
	pierna_izquierda.LoadModel("Models/pierna_izquierda.obj");
	pierna_derecha = Model();
	pierna_derecha.LoadModel("Models/pierna_derecha.obj");
	brazo_izquierdo = Model();
	brazo_izquierdo.LoadModel("Models/brazo_izquierdo.obj");
	brazo_derecho = Model();
	brazo_derecho.LoadModel("Models/brazo_derecho.obj");

	///////////////////////////////////////////////////


	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	Numero3Texture = Texture("Textures/numero3.tga");
	Numero3Texture.LoadTextureA();
	Numero4Texture = Texture("Textures/numero4.tga");
	Numero4Texture.LoadTextureA();
	Numero5Texture = Texture("Textures/numero5.tga");
	Numero5Texture.LoadTextureA();
	Numero6Texture = Texture("Textures/numero6.tga");
	Numero6Texture.LoadTextureA();
	Numero7Texture = Texture("Textures/numero7.tga");
	Numero7Texture.LoadTextureA();
	Numero8Texture = Texture("Textures/numero8.tga");
	Numero8Texture.LoadTextureA();
	Numero9Texture = Texture("Textures/numero9.tga");
	Numero9Texture.LoadTextureA();
	Numero0Texture = Texture("Textures/numero0.tga");
	Numero0Texture.LoadTextureA();



	std::vector<std::string> skyboxFaces;
	/*
	skyboxFaces.push_back("Textures/Skybox/sp2_rt.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_lf.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_dn.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_up.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_bk.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_ft.png");
	*/
	skyboxFaces.push_back("Textures/Skybox/negx.jpg");
	skyboxFaces.push_back("Textures/Skybox/posx.jpg"); 
	skyboxFaces.push_back("Textures/Skybox/negy.jpg"); //Noche
	skyboxFaces.push_back("Textures/Skybox/posy.jpg");
	skyboxFaces.push_back("Textures/Skybox/negz.jpg");
	skyboxFaces.push_back("Textures/Skybox/posz.jpg");
	skybox = Skybox(skyboxFaces);


	std::vector<std::string> skyboxFaces2;

	skyboxFaces2.push_back("Textures/Skybox/negx_dia.jpg");
	skyboxFaces2.push_back("Textures/Skybox/posx_dia.jpg");
	skyboxFaces2.push_back("Textures/Skybox/negy_dia.jpg"); //Dia
	skyboxFaces2.push_back("Textures/Skybox/posy_dia.jpg");
	skyboxFaces2.push_back("Textures/Skybox/negz_dia.jpg");
	skyboxFaces2.push_back("Textures/Skybox/posz_dia.jpg");
	skybox2 = Skybox(skyboxFaces2);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;


	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::vec3 posCanica = glm::vec3(0.0f, 0.0f, 0.0f);

	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ
	///////////////////////////////////
	KeyFrame[0].movCanica_x = 26.0f;
	KeyFrame[0].movCanica_z = 0.0f;

	KeyFrame[1].movCanica_x = 26.0f;
	KeyFrame[1].movCanica_z = -51.0f;

	KeyFrame[2].movCanica_x = 11.0f;
	KeyFrame[2].movCanica_z = -51.0f;

	KeyFrame[3].movCanica_x = 20.0f;
	KeyFrame[3].movCanica_z = -36.0f;

	KeyFrame[4].movCanica_x = 16.0f;
	KeyFrame[4].movCanica_z = -39.0f;

	KeyFrame[5].movCanica_x = 19.0f;
	KeyFrame[5].movCanica_z = -22.0f;

	KeyFrame[6].movCanica_x = 4.0f;
	KeyFrame[6].movCanica_z = -33.0f;

	KeyFrame[7].movCanica_x = 5.0f;
	KeyFrame[7].movCanica_z = -20.0f;

	KeyFrame[8].movCanica_x = 10.0f;
	KeyFrame[8].movCanica_z = -13.0f;

	KeyFrame[9].movCanica_x = 11.0f;
	KeyFrame[9].movCanica_z = -4.0f;

	KeyFrame[10].movCanica_x = 16.0f;
	KeyFrame[10].movCanica_z = -35.0f;

	KeyFrame[11].movCanica_x = 13.0f;
	KeyFrame[11].movCanica_z = -2.0f;

	KeyFrame[12].movCanica_x = 10.0f;
	KeyFrame[12].movCanica_z = 2.0f;
	//////////////////////////////////

	//Se agregan nuevos frames 

	printf("\nTeclas para uso de Keyframes:\n1.-Presionar barra espaciadora para reproducir animacion.\n2.-Presionar 0 para volver a habilitar reproduccion de la animacion\n");
	printf("3.-Presiona L para guardar frame\n4.-Presiona P para habilitar guardar nuevo frame\n5.-Presiona 1 para mover en X\n6.-Presiona 2 para mover en -X\n");
	printf("7.-Presiona 3 para mover en Z\n8.-Presiona 4 para mover en -Z");


	//Para apagar y prender las luces cada 10 segundos:
	ciclioDia = true;
	//Obtenemos tiempo inicial
	double tiempoInicial = glfwGetTime();
	double tiempoUltimoCambio = tiempoInicial;
	
	
	//Audio

	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	if (!engine) return 1; // could not start engine

	engine->play2D("MGSAmbiente.mp3", true); // play some mp3 file, looped

	////Loop mientras no se cierra la ventana///////////////////////////////////////////////////////////////////////////
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		cambioNumero = numero % 10;
		cambioNumero1 = (numero / 10) % 10;
		cambioNumero2 = (numero / 100) % 10;

		cambioNumero3 = (numero / 1000) % 10;
		cambioNumero4 = (numero / 10000) % 10;
		cambioNumero5 = (numero / 100000) % 10;

		cambioNumero6 = (numero / 1000000) % 10;
		cambioNumero7 = (numero / 10000000) % 10;
		cambioNumero8 = (numero / 100000000) % 10;

		//PARA PARAR LA MUSICA

		//std::cin.get(); // wait until user presses a key

		//engine->drop(); // delete engine

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//-------Para Keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Obtenemos el tiempo actual
		double tiempoActual = glfwGetTime();
		if (tiempoActual - tiempoUltimoCambio >= 3.0)
		{
			ciclioDia = (ciclioDia == true) ? false : true;
			tiempoUltimoCambio = tiempoActual;
		}
		if (ciclioDia == true)
		{
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else
		{
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
		}


		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());




		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);




		if (rotCamaraAux == 0)
		{
			if (rotCamara < 200)
			{
				rotCamara += 1.0 * deltaTime;
			}
			else
			{
				rotCamaraAux = 1;
			}
		}
		else
		{
			if (rotCamara > 0)
			{
				rotCamara -= 1.0 * deltaTime;
			}
			else
			{
				rotCamaraAux = 0;
			}
		}

		if (varAngAux0 == 1)
		{
			if (angAux0 < 31) {
				angAux0 += 1.0f * deltaTime;
			}
		}

		////////////////////////Obstaculos /////////////////////////////////////////////////
		//Camara 1////////////////////////////////////////////////////////////////////////////
		// base camara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.5f, 46.0f, 13.75f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, (0 + rotCamara  + 90 ) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		modelaux = model;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		base_camara.RenderModel();

		// camara
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.94f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, (0 + angAux0) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		camara.RenderModel();

		//Camara 2//////////////////////////////////////////////////////////////////////
		// base camara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.75f, 46.0f, -10.0f ));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, (0 + rotCamara + 15 ) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		base_camara.RenderModel();

		if (varAngAux1 == 1)
		{
			if (angAux1 < 31) {
				angAux1 += 1.0f * deltaTime;
			}
		}

		// camara
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.94f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, (0 + angAux1) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		camara.RenderModel();

		//Camara3/////////////////////////////////////////////////////////////////////
		// base camara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.75f, 46.0f, 8.75f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, (0 + rotCamara + 25) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		base_camara.RenderModel();
		if (varAngAux2 == 1)
		{
			if (angAux2 < 31) {
				angAux2 += 1.0f * deltaTime;
			}
		}
		// camara
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.94f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, (0 + angAux2) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		camara.RenderModel();

		// Caja obstaculo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 51.0f, 18.50f));
		model = glm::scale(model, glm::vec3(-11.0f, -11.0f, -11.0f));
		model = glm::rotate(model, -77 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caja.RenderModel();
		
		// granada obstaculo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.25f, 46.75f, -5.75f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		granada.RenderModel();

		// granada obstaculo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f,46.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		granada.RenderModel();

		// granada obstaculo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0f, 46.75f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		granada.RenderModel();

		if (mainWindow.moverFlipper1() == true)
		{
			moverFlipper1 = 1;
		}

		if (moverFlipper1 == 1)
		{
			if (auxFlipper1 == 0)
			{
				if ((gradosFlipper1 + auxGradosFlipper1) > 145)
				{
					auxGradosFlipper1 -= 3.0 * deltaTime;
				}
				else
				{
					auxFlipper1 = 1;
				}
			}
			else
			{
				if ((gradosFlipper1 + auxGradosFlipper1) < 213)
				{
					auxGradosFlipper1 += 3.0 * deltaTime;
				}				
				else
				{
					auxFlipper1 = 0;
					moverFlipper1 = 0;
				}
			}
		}
	
		// flipper katana derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.5f, 48.0f, 31.0f));
		model = glm::scale(model, glm::vec3(0.75f ,0.75f, 0.75f));
		model = glm::rotate(model, (gradosFlipper1 + auxGradosFlipper1 ) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		katana.RenderModel();

		if (mainWindow.moverFlipper2() == true)
		{
			moverFlipper2 = 1;
		}

		if (moverFlipper2 == 1)
		{
			if (auxFlipper2 == 0)
			{
				if ((gradosFlipper2 + auxGradosFlipper2) > 145)
				{
					auxGradosFlipper2 -= 3.0 * deltaTime;
				}
				else
				{
					auxFlipper2 = 1;
				}
			}
			else
			{
				if ((gradosFlipper2 + auxGradosFlipper2) < 213)
				{
					auxGradosFlipper2 += 3.0 * deltaTime;
				}
				else
				{
					auxFlipper2 = 0;
					moverFlipper2 = 0;
				}
			}
		}
		// flipper katana derecha arriba
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.75f, 48.0f, 7.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::rotate(model, (gradosFlipper2 + auxGradosFlipper2) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		katana.RenderModel();

		if (mainWindow.moverFlipper3() == true)
		{
			moverFlipper3 = 1;
		}
		if (moverFlipper3 == 1)
		{
			if (auxFlipper3 == 0)
			{
				if ((gradosFlipper3 + auxGradosFlipper3) < 215)
				{
					auxGradosFlipper3 += 3.0 * deltaTime;
				}
				else
				{
					auxFlipper3 = 1;
				}
			}
			else
			{
				if ((gradosFlipper3 + auxGradosFlipper3) > 140)
				{
					auxGradosFlipper3 -= 3.0 * deltaTime;
				}
				else
				{
					auxFlipper3 = 0;
					moverFlipper3 = 0;
				}
			}

		}

		// flipper katana izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.75f, 48.0f, 30.5f));
		model = glm::scale(model, glm::vec3(-0.75f, -0.75f, -0.75f));
		model = glm::rotate(model, (gradosFlipper3 + auxGradosFlipper3) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		katana.RenderModel();
		///Avatar ///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// torso cabeza
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-13.5f , 48.0f , 33.0f ));
		model = glm::translate(model, glm::vec3(-13.5f + mainWindow.getmuevex(), 48.0f + mainWindow.getmuevey(), 33.0f + mainWindow.getmuevez()));

		model = glm::scale(model, glm::vec3(0.75 , 0.75f, 0.75f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		torso_cabeza.RenderModel();

		// brazo izquierdo
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.38f, 0.66f, -0.07f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_izquierdo.RenderModel();

		// brazo derecho
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.38f, 0.66f, -0.07f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_derecho.RenderModel();

		// pierna izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.13f, -0.03f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pierna_izquierda.RenderModel();

		// pierna derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.13f, -0.03f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pierna_derecha.RenderModel();

		if (mainWindow.activar())
		{
			if (varAuxCanica1 == 0)
			{
				if (-11.5 + canica1X < 13.25)
				{
					canica1X += 1.0f * deltaTime;

				}
				else
				{
					moverResorte = 1;
					varAuxCanica1 = 1;
				}
				
			}
			if (varAuxCanica1 == 1)
			{
				
				if (35.0 + canica1Z > -15)
				{
					canica1Z -= 1.0f * deltaTime;
				}
				else
				{
					
					varAuxCanica1 = 2;
				}

			}
			if (varAuxCanica1 == 2)
			{
				if (-11.5 + canica1X > -2)
				{
					canica1X -= 1.0f * deltaTime;
				}
				else
				{

					varAuxCanica1 = 3;
				}
			}
			if (varAuxCanica1 == 3)
			{
				if (-11.5 + canica1X < 0.5)
				{
					canica1X += 0.25f * deltaTime;

				}
				if (35.0 + canica1Z < -6.5)
				{
					canica1Z += 0.25f * deltaTime;
				}
				else
				{
					numero += 300;
					varAuxCanica1 = 4;
				}
			}
			if (varAuxCanica1 == 4)
			{
				if (-11.5 + canica1X > -6.5)
				{
					canica1X -= 0.25f * deltaTime;

				}
				if (35.0 + canica1Z > -9.5)
				{
					canica1Z -= 0.25f * deltaTime;
				}
				if ((-11.5 + canica1X < -6.5)&& (35.0 + canica1Z < -9.5))
				{
					numero += 10000;
					varAngAux1 = 1;
					varAuxCanica1 = 5;
				}
			}
			
			if (varAuxCanica1 == 5)
			{
				if (-11.5 + canica1X < -5.0)
				{
					canica1X += 0.25f * deltaTime;

				}
				if (35.0 + canica1Z < 7.25)
				{
					canica1Z += 0.25f * deltaTime;
				}
				if((-11.5 + canica1X > -5.0) && (35.0 + canica1Z > 7.25))
				{
					varAngAux2 = 1;
					varAuxCanica1 = 6;
					numero += 1000;
				}
			}
			
			if (varAuxCanica1 == 6)
			{
				if (-11.5 + canica1X > -9.5)
				{
					canica1X -= 0.25f * deltaTime;

				}
				if (35.0 + canica1Z > 4.25)
				{
					canica1Z -= 0.25f * deltaTime;
				}
				if((-11.5 + canica1X < -9.5) && (35.0 + canica1Z < 4.25))
				{
					
					varAuxCanica1 = 7;
				}
			}


			if (varAuxCanica1 == 7)
			{
				if (-11.5 + canica1X < -7.5)
				{
					canica1X += 0.25f * deltaTime;

				}
				if (35.0 + canica1Z < 15.25)
				{
					canica1Z += 0.25f * deltaTime;
				}
				if ((-11.5 + canica1X > -7.5) && (35.0 + canica1Z > 15.25))
				{

					varAuxCanica1 = 8;
					numero += 1000;
				}
			}

			if (varAuxCanica1 == 8)
			{
				if (-11.5 + canica1X < -4.0)
				{
					canica1X += 0.25f * deltaTime;

				}
				if (35.0 + canica1Z < 21.5)
				{
					canica1Z += 0.25f * deltaTime;
				}
				if ((-11.5 + canica1X > -4.0) && (35.0 + canica1Z > 21.5))
				{

					varAuxCanica1 = 9;
					numero += 50000;
				}
			}

			if (varAuxCanica1 == 9)
			{
				if (-11.5 + canica1X < -2.5)
				{
					canica1X += 0.25f * deltaTime;


				}
				if (35.0 + canica1Z < 31)
				{
					canica1Z += 0.25f * deltaTime;
				}
				if ((-11.5 + canica1X > -2.5) && (35.0 + canica1Z > 31))
				{

					varAuxCanica1 = 10;
					moverFlipper3 = 1;
				}
			}

			if (varAuxCanica1 == 10)
			{
				if (-11.5 + canica1X < 6.5)
				{
					canica1X += 0.125f * deltaTime;
					
				}
				if (35.0 + canica1Z > 3.5)
				{
					canica1Z -= 0.5f * deltaTime;
				}
				if ((-11.5 + canica1X > 6.5) && (35.0 + canica1Z <3.5))
				{
					numero += 500;
					varAuxCanica1 = 11;
				}
			}

			if (varAuxCanica1 == 11)
			{
				if (-11.5 + canica1X < 7.0)
				{
					canica1X += 0.0125f * deltaTime;
					
				}
				if (35.0 + canica1Z < 7.5)
				{
					canica1Z += 0.25f * deltaTime;
				}
				if ((-11.5 + canica1X > 7.0) && (35.0 + canica1Z > 7.5))
				{
					moverFlipper2 = 1;
					varAuxCanica1 = 12;
				}
			}

			if (varAuxCanica1 == 12)
			{
				if (-11.5 + canica1X > 2.5)
				{
					canica1X -= 0.5f * deltaTime;
					
				}
				if (35.0 + canica1Z > -3.0)
				{
					canica1Z -= 0.25f * deltaTime;
				}
				if ((-11.5 + canica1X < 2.5) && (35.0 + canica1Z < -3.0))
				{
					numero += 5000;
					varAuxCanica1 = 13;
				}
			}

			if (varAuxCanica1 == 13)
			{
				if (-11.5 + canica1X > 1.0)
				{
					canica1X -= 0.25f * deltaTime;
					

				}
				if (35.0 + canica1Z < 37.25)
				{
					canica1Z += 0.5f * deltaTime;
				}
				if ((-11.5 + canica1X < 2.5) && (35.0 + canica1Z < -3.0))
				{
					varAuxCanica1 = 14;
				}
			}

		}

		// canica1
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-11.5f + canica1X , 47.5f , 35.0f + canica1Z));
		model = glm::translate(model, glm::vec3(-11.5f +canica1X, 47.5f , 35.0f + canica1Z));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		canica.RenderModel();

		// canica2 por keyframes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.5f, 47.75, 35.0));
		posCanica = glm::vec3(movCanica_x, movCanica_y, movCanica_z);
		model = glm::translate(model, posCanica);

		model = glm::scale(model, glm::vec3(0.25f , 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		canica.RenderModel();

		// resorte
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.77f, 47.07f, 44.07f));
		
		if (mainWindow.moverResorte() == true)
		{
			moverResorte = 1;
		}

		if (moverResorte == 1)
		{
			if (varAuxResorte == 0)
			{
				if ((zEnResorte + auxResorte) > 41.5)
				{
					auxResorte -= 3.0 * deltaTime;
				}
				else
				{
					varAuxResorte = 1;
				}
			}
			if (varAuxResorte == 1)
			{
				if ((zEnResorte + auxResorte) < 100)
				{
					auxResorte += 3.0 * deltaTime;
				}
				else
				{
					varAuxResorte = 2;

				}
			}
			if (varAuxResorte == 2)
			{
				if ((zEnResorte + auxResorte) > 50)
				{
					auxResorte -= 3.0 * deltaTime;
				}
				else
				{
					varAuxResorte = 0;
					moverResorte = 0;
				}
			}
			{

			}

		}

		model = glm::scale(model, glm::vec3(50.0f, 50.0f, zEnResorte + auxResorte));
		

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		resorte.RenderModel();

		// palanca
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.77f, 46.49f, 42.82f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		palanca.RenderModel();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pinball.RenderModel();



		////////////////////////////////////////////Numeros///////////////////////////////////////////////////////////


		//Primer Digito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.25f, 91.75f, -16.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		switch (cambioNumero)
		{
		case 0:
			Numero0Texture.UseTexture();
			break;
		case 1:
			Numero1Texture.UseTexture();
			break;
		case 2:
			Numero2Texture.UseTexture();
			break;
		case 3:
			Numero3Texture.UseTexture();
			break;
		case 4:
			Numero4Texture.UseTexture();
			break;
		case 5:
			Numero5Texture.UseTexture();
			break;
		case 6:
			Numero6Texture.UseTexture();
			break;
		case 7:
			Numero7Texture.UseTexture();
			break;
		case 8:
			Numero8Texture.UseTexture();
			break;
		case 9:
			Numero9Texture.UseTexture();
			break;
		}


		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Segundo Digito Digito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0, 91.75, -16.5));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		switch (cambioNumero1)
		{
		case 0:
			Numero0Texture.UseTexture();
			break;
		case 1:
			Numero1Texture.UseTexture();
			break;
		case 2:
			Numero2Texture.UseTexture();
			break;
		case 3:
			Numero3Texture.UseTexture();
			break;
		case 4:
			Numero4Texture.UseTexture();
			break;
		case 5:
			Numero5Texture.UseTexture();
			break;
		case 6:
			Numero6Texture.UseTexture();
			break;
		case 7:
			Numero7Texture.UseTexture();
			break;
		case 8:
			Numero8Texture.UseTexture();
			break;
		case 9:
			Numero9Texture.UseTexture();
			break;
		}


		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Tercero Digito Digito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.75, 91.75, -16.5));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		switch (cambioNumero2)
		{
		case 0:
			Numero0Texture.UseTexture();
			break;
		case 1:
			Numero1Texture.UseTexture();
			break;
		case 2:
			Numero2Texture.UseTexture();
			break;
		case 3:
			Numero3Texture.UseTexture();
			break;
		case 4:
			Numero4Texture.UseTexture();
			break;
		case 5:
			Numero5Texture.UseTexture();
			break;
		case 6:
			Numero6Texture.UseTexture();
			break;
		case 7:
			Numero7Texture.UseTexture();
			break;
		case 8:
			Numero8Texture.UseTexture();
			break;
		case 9:
			Numero9Texture.UseTexture();
			break;
		}


		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Cuatro Digito Digito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.5, 91.75, -16.5));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		switch (cambioNumero3)
		{
		case 0:
			Numero0Texture.UseTexture();
			break;
		case 1:
			Numero1Texture.UseTexture();
			break;
		case 2:
			Numero2Texture.UseTexture();
			break;
		case 3:
			Numero3Texture.UseTexture();
			break;
		case 4:
			Numero4Texture.UseTexture();
			break;
		case 5:
			Numero5Texture.UseTexture();
			break;
		case 6:
			Numero6Texture.UseTexture();
			break;
		case 7:
			Numero7Texture.UseTexture();
			break;
		case 8:
			Numero8Texture.UseTexture();
			break;
		case 9:
			Numero9Texture.UseTexture();
			break;
		}


		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		//Quinto Digito Digito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25, 91.75, -16.5));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		switch (cambioNumero4)
		{
		case 0:
			Numero0Texture.UseTexture();
			break;
		case 1:
			Numero1Texture.UseTexture();
			break;
		case 2:
			Numero2Texture.UseTexture();
			break;
		case 3:
			Numero3Texture.UseTexture();
			break;
		case 4:
			Numero4Texture.UseTexture();
			break;
		case 5:
			Numero5Texture.UseTexture();
			break;
		case 6:
			Numero6Texture.UseTexture();
			break;
		case 7:
			Numero7Texture.UseTexture();
			break;
		case 8:
			Numero8Texture.UseTexture();
			break;
		case 9:
			Numero9Texture.UseTexture();
			break;
		}


		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();



		//Sexto Digito Digito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0, 91.75, -16.5));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		switch (cambioNumero5)
		{
		case 0:
			Numero0Texture.UseTexture();
			break;
		case 1:
			Numero1Texture.UseTexture();
			break;
		case 2:
			Numero2Texture.UseTexture();
			break;
		case 3:
			Numero3Texture.UseTexture();
			break;
		case 4:
			Numero4Texture.UseTexture();
			break;
		case 5:
			Numero5Texture.UseTexture();
			break;
		case 6:
			Numero6Texture.UseTexture();
			break;
		case 7:
			Numero7Texture.UseTexture();
			break;
		case 8:
			Numero8Texture.UseTexture();
			break;
		case 9:
			Numero9Texture.UseTexture();
			break;
		}
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		//Septimo Digito Digito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.25, 91.75, -16.5));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		switch (cambioNumero6)
		{
		case 0:
			Numero0Texture.UseTexture();
			break;
		case 1:
			Numero1Texture.UseTexture();
			break;
		case 2:
			Numero2Texture.UseTexture();
			break;
		case 3:
			Numero3Texture.UseTexture();
			break;
		case 4:
			Numero4Texture.UseTexture();
			break;
		case 5:
			Numero5Texture.UseTexture();
			break;
		case 6:
			Numero6Texture.UseTexture();
			break;
		case 7:
			Numero7Texture.UseTexture();
			break;
		case 8:
			Numero8Texture.UseTexture();
			break;
		case 9:
			Numero9Texture.UseTexture();
			break;
		}
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		//Octavo Digito Digito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5, 91.75, -16.5));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		switch (cambioNumero7)
		{
		case 0:
			Numero0Texture.UseTexture();
			break;
		case 1:
			Numero1Texture.UseTexture();
			break;
		case 2:
			Numero2Texture.UseTexture();
			break;
		case 3:
			Numero3Texture.UseTexture();
			break;
		case 4:
			Numero4Texture.UseTexture();
			break;
		case 5:
			Numero5Texture.UseTexture();
			break;
		case 6:
			Numero6Texture.UseTexture();
			break;
		case 7:
			Numero7Texture.UseTexture();
			break;
		case 8:
			Numero8Texture.UseTexture();
			break;
		case 9:
			Numero9Texture.UseTexture();
			break;
		}
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		//Noveno Digito Digito
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.75, 91.75, -16.5));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		switch (cambioNumero8)
		{
		case 0:
			Numero0Texture.UseTexture();
			break;
		case 1:
			Numero1Texture.UseTexture();
			break;
		case 2:
			Numero2Texture.UseTexture();
			break;
		case 3:
			Numero3Texture.UseTexture();
			break;
		case 4:
			Numero4Texture.UseTexture();
			break;
		case 5:
			Numero5Texture.UseTexture();
			break;
		case 6:
			Numero6Texture.UseTexture();
			break;
		case 7:
			Numero7Texture.UseTexture();
			break;
		case 8:
			Numero8Texture.UseTexture();
			break;
		case 9:
			Numero9Texture.UseTexture();
			break;
		}
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		///////////////////////////////////////////////////////////////////////////


		/*
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.0f + mainWindow.getmuevey(), 0.0f + mainWindow.getmuevez()));
		printf("X: %f, Y: %f Z: %f \n", 0.0f + mainWindow.getmuevex(), 0.0f + mainWindow.getmuevey(), 0.0f + mainWindow.getmuevez());
		model = glm::scale(model, glm::vec3(0.5f + mainWindow.getmueveEscala(), 0.5f + mainWindow.getmueveEscala(), 0.5f + mainWindow.getmueveEscala()));
		printf("Escala: %f\n", 0.5 + mainWindow.getmueveEscala());
		model = glm::rotate(model, (0 + mainWindow.getmueveRot()) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		printf("Rotacion: %f\n", 0 + mainWindow.getmueveRot());
*/
		glDisable(GL_BLEND);

		if (mainWindow.getIsometrica() == 1.0f) {
			// 1. obtenemos posiciones iniciales de la camara
			// 2. los ifs se encargaran de igualar el valor inicial al valor final, sin importar si es mayor o meno
			// 3. se le asignara dicho valor al set

			/////////////////////// Para vista ///////////////////////
			vistaInicialX = camera.getVistaActualX();
			vistaInicialY = camera.getVistaActualY();

			if (vistaInicialX > vistaFinalX) {
				auxCam = vistaInicialX - incVista;
			}
			else if (vistaInicialX == vistaFinalX) {
				auxCam = auxCam;
			}
			else {
				auxCam = vistaInicialX + incVista;
			}
			camera.setVistaActualX(auxCam);

			if (vistaInicialY > vistaFinalY) {
				auxCam = vistaInicialY - incVista;
			}
			else if (vistaInicialY == vistaFinalY) {
				auxCam = auxCam;
			}
			else {
				auxCam = vistaInicialY + incVista;
			}
			camera.setVistaActualY(auxCam);
			/////////////////////////////////////////////////////////

			/////////////////////// Para pos ///////////////////////
			posInicialX = camera.getPosActualX();
			posInicialY = camera.getPosActualY();
			posInicialZ = camera.getPosActualZ();

			if (posInicialX > posFinalX) {
				auxCam = posInicialX - incPos;
			}
			else if (posInicialX == posFinalX) {
				auxCam = auxCam;
			}
			else {
				auxCam = posInicialX + incPos;
			}
			camera.setPosActualX(auxCam);

			if (posInicialY > posFinalY) {
				auxCam = posInicialY - incPos;
			}
			else if (posInicialY == posFinalY) {
				auxCam = auxCam;
			}
			else {
				auxCam = posInicialY + incPos;
			}
			camera.setPosActualY(auxCam);

			if (posInicialZ > posFinalZ) {
				auxCam = posInicialZ - incPos;
			}
			else if (posInicialZ == posFinalZ) {

			}
			else {
				auxCam = posInicialZ + incPos;
			}
			camera.setPosActualZ(auxCam);
		}
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1 && reproduciranimacion>0)
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora\n");
			reproduciranimacion = 0;

		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("\n/////////////////\n");
			printf("movCanica_x es: %f\n", movCanica_x);
			printf("movCanica_z es: %f\n", movCanica_z);
			printf("\n/////////////////\n");
			printf("presiona P para habilitar guardar otro frame\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1 && guardoFrame>0)
		{
			guardoFrame = 0;
			printf("Ya puedes guardar otro frame presionando la tecla L\n");
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			movCanica_x += 1.0f;
			printf("\n movCanica_x es: %f\n", movCanica_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_2])
	{
		if (ciclo < 1)
		{
			movCanica_x -= 1.0f;
			printf("\n movCanica_x es: %f\n", movCanica_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_3])
	{
		if (ciclo < 1)
		{
			movCanica_z += 1.0f;
			printf("\n movCanica_z es: %f\n", movCanica_z);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_4])
	{
		if (ciclo < 1)
		{
			movCanica_z -= 1.0f;
			printf("\n movCanica_z es: %f\n", movCanica_z);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_5])
	{
		if (ciclo < 1)
		{
			movCanica_y += 1.0f;
			printf("\n movCanica_y es: %f\n", movCanica_y);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_6])
	{
		if (ciclo < 1)
		{
			movCanica_y -= 1.0f;
			printf("\n movCanica_y es: %f\n", movCanica_y);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_0])
	{
		if (ciclo2 < 1 && ciclo>0)
		{
			ciclo = 0;
			printf("\n Ya puedes modificar tu variable\n");
		}
	}

}
