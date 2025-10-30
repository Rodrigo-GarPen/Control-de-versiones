/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
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

#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
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
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
							//P    R	  O		 Y	    E	   C     T      O      -       C     G      E     I      H     C 
float toffsetletrasu[] = { 0.66, 0.88f, 0.55f, 0.66f, 0.44f, 0.22f, 0.11f, 0.55f, 0.88f, 0.22f, 0.66f, 0.44, 0.88f, 0.77f, 0.22};

float toffsetletrasv[] = {-0.33f,-0.33f,-0.33f,-0.65f, 0.0f, 0.0f, -0.65f, -0.33f,-0.65, 0.0f,  0.0f,  0.0f, 0.0f,  0.0f,  0.0f};

float toffsetletrasuAux[] = { 0.66, 0.88f, 0.55f, 0.66f, 0.44f, 0.22f, 0.11f, 0.55f, 0.88f, 0.22f, 0.66f, 0.44, 0.88f, 0.77f, 0.22 };

float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float dragonavance = 0.0f;
float puertamov = 0.0f;

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
Texture dado8Texture;


Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Dragon_Ala_Der_M;
Model Dragon_Ala_Izq_M;
Model Tiamat_M;
Model Arco;
Model PuertaI;
Model PuertaD;
Model Letrero;
Skybox skybox;

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
		0.5f, 0.0f, 0.5f,		0.066f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.066f, 0.93f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 0.93f,		0.0f, -1.0f, 0.0f,

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
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado8Caras()
{

	GLfloat vertices[] = {
		// Cara 1 (arriba - frente)
0.0f, 1.0f, 0.0f,    0.51f, 0.74f,    0.0f, -1.0f, 0.0f,
1.0f, 0.0f, 0.0f,   0.74f, 0.51f,  -1.0f,  0.0f, 0.0f,
0.0f, 0.0f, 1.0f,   0.26f, 0.51f,   0.0f,  0.0f, -1.0f,

// Cara 2 (arriba - izquierda)
0.0f, 1.0f, 0.0f,    0.01f, 0.26f,   0.0f, -1.0f, 0.0f,
0.0f, 0.0f, 1.0f,    0.25f, 0.49f,   0.0f,  0.0f, -1.0f,
-1.0f,0.0f, 0.0f,   0.5f, 0.25f,   1.0f,  0.0f, 0.0f,

// Cara 3 (arriba - atrás)
0.0f, 1.0f, 0.0f,   0.01f, 0.74f,   0.0f, -1.0f, 0.0f,
-1.0f,0.0f, 0.0f,   0.49f, 0.74f,   1.0f,  0.0f, 0.0f,
0.0f, 0.0f,-1.0f,    0.24f, 0.51f,   0.0f,  0.0f, 1.0f,

// Cara 4 (arriba - derecha)
0.0f, 1.0f, 0.0f,   0.5f, 0.26f,   0.0f, -1.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.26f, 0.49f,   0.0f,  0.0f, 1.0f,
1.0f, 0.0f, 0.0f,   0.74f, 0.49f,  -1.0f,  0.0f, 0.0f,

// Cara 5 (abajo - frente)
0.0f,-1.0f, 0.0f,  0.75f, 0.01f,   0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f,    0.99f, 0.24f,   0.0f, 0.0f, -1.0f,
1.0f, 0.0f, 0.0f,    0.51f, 0.24f,   -1.0f, 0.0f, 0.0f,

// Cara 6 (abajo - izquierda)
0.0f,-1.0f, 0.0f,  0.99f, 0.74f,    0.0f, 1.0f, 0.0f,
-1.0f,0.0f, 0.0f,   0.75f, 0.51f,   1.0f, 0.0f, 0.0f,
0.0f, 0.0f, 1.0f,  0.51f, 0.74f,   0.0f, 0.0f, -1.0f,

// Cara 7 (abajo - atrás)
0.0f,-1.0f, 0.0f,   0.98f, 0.26f,   0.0f, 1.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.51f, 0.26f,    0.0f, 0.0f, 1.0f,
-1.0f,0.0f, 0.0f,    0.75f, 0.49f,   1.0f, 0.0f, 0.0f,

// Cara 8 (abajo - derecha)
0.0f,-1.0f, 0.0f,   0.75f, 0.99f,   0.0f, 1.0f, 0.0f,
1.0f, 0.0f, 0.0f,   0.99f, 0.76f,   -1.0f, 0.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.51f, 0.76f,   0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0,  1,  2,   // Cara 1
		3,  4,  5,   // Cara 2
		6,  7,  8,   // Cara 3
		9, 10, 11,   // Cara 4
		12,13,14,    // Cara 5
		15,16,17,    // Cara 6
		18,19,20,    // Cara 7
		21,22,23,    // Cara 8
	};

	Mesh* dado8 = new Mesh();
	dado8->CreateMesh(vertices, indices, sizeof(vertices) / sizeof(GLfloat), sizeof(indices) / sizeof(unsigned int));
	meshList.push_back(dado8);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado8Caras();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/letrero.png");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	dado8Texture = Texture("Textures/dado8caras.png");
	dado8Texture.LoadTextureA();

	
	Arco = Model();
	Arco.LoadModel("Models/Arco.obj");
	PuertaI= Model();
	PuertaI.LoadModel("Models/Puerta.obj");
	PuertaD = Model();
	PuertaD.LoadModel("Models/PuertaD.obj");
	Letrero = Model();
	Letrero.LoadModel("Models/Letrero.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.5f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	avanza = true;
	bool dragonAvanza = true;

	int contadorNum = 0;
	bool esUno = true;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelauxDado(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	
	srand(static_cast<unsigned int>(time(nullptr)));

	// Generar número aleatorio entero entre 0 y N-1

	GLfloat rotX = 0.0f;
	GLfloat rotY = 0.0f;
	bool caer = false, rotar = false;
	float move = 0.0f;
	GLfloat caida = 0.0f;

	GLfloat angulos[] = { 125.6f, 125.6, 125.6f, 125.6f, 54.74f, 54.74f, 54.74f, 54.74f};

	glm::vec3 ejes[] = {
	  glm::vec3(1.0f * 1 / glm::sqrt(2), 0.0f, -1.0f * 1 / glm::sqrt(2)),

	  glm::vec3(-1.0f * 1 / glm::sqrt(2), 0.0f, -1.0f * 1 / glm::sqrt(2)),

	  glm::vec3(-1.0f * 1 / glm::sqrt(2), 0.0f, 1.0f * 1 / glm::sqrt(2)),

	  glm::vec3(1.0f * 1 / glm::sqrt(2), 0.0f, 1.0f * 1 / glm::sqrt(2)),

	  glm::vec3(1.0f * 1 / glm::sqrt(2), 0.0f, -1.0f * 1 / glm::sqrt(2)),

	  glm::vec3(-1.0f * 1 / glm::sqrt(2), 0.0f, 1.0f * 1 / glm::sqrt(2)),

	  glm::vec3(-1.0f * 1 / glm::sqrt(2), 0.0f, -1.0f * 1 / glm::sqrt(2)),

	  glm::vec3(1.0f * 1 / glm::sqrt(2), 0.0f, 1.0f * 1 / glm::sqrt(2)),
	};
	
	while (!mainWindow.getShouldClose())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 3.14f * deltaTime;
	

		if (mainWindow.abrirPuerta()) {
			if (puertamov <= 90) {
				puertamov += 0.5 * deltaTime;
			}
		}
		else {
			if (puertamov > 0)
				puertamov -= 0.5 * deltaTime;
		}
		
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, 1);


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -2.0f, 0.0));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arco.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(1.0f, 2.8f, -2.0));
		model = glm::rotate(model, puertamov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaI.RenderModel();
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0 + (puertamov / 45)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaD.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.0f, -2.0f, 0.0));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero.RenderModel();



		// Octaedro
		if (mainWindow.shouldSwitch1()) {
			if (dragonavance >= 0.0f && dragonavance <= 10.5) {
				dragonavance -= caida * deltaTime;
				caida *= 1.01;
			}
			else {
				caer = true;
			}
			
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(1.5f, -0.5f + dragonavance, 2.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			modelauxDado = model;
			if (caer) {
				
				if (move <= rotX && move >= 0) {
					move += 0.1 * deltaTime;
					rotY += 0.1 * deltaTime;
				}
				model = modelauxDado;
				model = glm::translate(model, glm::vec3(-move, 0.0f, 0.0f));
				model = glm::rotate(model, move + rotY  * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, move + rotY  * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, move + rotY  * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
				modelauxDado = model;
				if (move >= rotX) {
					caer = !caer;
					rotar = true;
				}
			}
			if (rotar) {
				model[0] = glm::vec4(1, 0, 0, 0); // eje X al estado base
				model[1] = glm::vec4(0, 1, 0, 0); // eje Y al estado base
				model[2] = glm::vec4(0, 0, 1, 0); // eje Z al estado base
				int index = (int)rotY % 8;
				model = glm::rotate(model, angulos[index] * toRadians, ejes[index]);
				rotar = !rotar;
			}


			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			dado8Texture.UseTexture();
			meshList[7]->RenderMesh();

		}
		else {
			
			rotX = (float)rand() / 1000 * deltaTime;
			rotY = (float)rand() / 100 * deltaTime;
			dragonavance = 10.5f;
			move = 0.0f;
			caida = 0.005f;

		}
		

		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetnumerou += 0.001;
		toffsetnumerov = 0.000;
		//para que no se desborde la variable
		if (toffsetnumerou > 1.0)
			toffsetnumerou = 0.0;
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.88f, 6.0f, -0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.3f, 1.2f, 1.5f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
		


	

		glDisable(GL_BLEND);


		


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
