// Created as empty project console application (Include precompiled headers)

#include "main.h"

#include "Camera.h"
#include "Shader.h"



#pragma comment( lib, "lib/OPENGL32.lib" )	
#pragma comment( lib, "lib/glew32s.lib" )	
#pragma comment( lib, "lib/glfw3.lib" )	
#pragma comment( lib, "lib/soil.lib" )	

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
const GLuint BORDER_X = 0, BORDER_Y = 0;
const float PI = 3.14159265359;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement();
void FillLightValues();
GLfloat* GenerateDiscVertices(int numOfTrianglesOnCircle, int &textureVerticesArraySize);
void GenerateCircle(GLfloat* retArray, int numOfTrianglesOnCircle, int offset, float y);
GLfloat* GenerateBoxVertices(int &textureVerticesArraySize);
unsigned int loadCubemap(std::vector <std::string> faces);


// Camera
Camera  camera(glm::vec3(0.0f, 1.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];
bool firstMouse = true;

// Directional light
glm::vec3 dirLightDirection;
glm::vec3 dirLightAmbient;
glm::vec3 dirLightDiffuse;
glm::vec3 dirLightSpecular;


// Deltatime
 GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
 GLfloat lastFrame = 0.0f;  	// Time of last frame
GLfloat rotAngle = 0.0f; // in radians

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ZARNO", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// GLFW Options - hidding cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);


	// Build and compile our shader program
	Shader lightingShader("shader.vs", "shader.frag");

	Shader skyShader("shaderSky.vs", "shaderSky.frag"); //new


	int numOfTrianglesOnCircle = 25;
	int discVerticesArraySize;
	GLfloat* discVertices = GenerateDiscVertices(numOfTrianglesOnCircle, discVerticesArraySize);

	GLfloat boxVertices[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	GLfloat boxGrassVertices[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 10.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 10.0f, 10.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 10.0f, 10.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 10.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 10.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 10.0f, 10.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 10.0f, 10.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 10.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 10.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 10.0f, 10.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 10.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 10.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 10.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 10.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 10.0f, 10.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 10.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 10.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 10.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 10.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 10.0f, 10.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 10.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 10.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 10.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f
	};

	GLfloat skyboxVertices[] = { //new
		// positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	// First, set the container's VAO (and VBO)
	GLuint discVBO, boxVBO, boxGrassVBO, discVAO, boxVAO, boxGrassVAO;

	GLuint skyVBO, skyVAO; //new

	glGenVertexArrays(1, &discVAO);
	glGenBuffers(1, &discVBO);
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &boxVBO);
	glGenVertexArrays(1, &boxGrassVAO);
	glGenBuffers(1, &boxGrassVBO);

	glGenVertexArrays(1, &skyVAO); //new
	glGenBuffers(1, &skyVBO);


	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);




	glBindVertexArray(discVAO);
	glBindBuffer(GL_ARRAY_BUFFER, discVBO);
	glBufferData(GL_ARRAY_BUFFER, discVerticesArraySize, discVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	glBindVertexArray(boxGrassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxGrassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxGrassVertices), boxGrassVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);




	std::vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);








	// Load textures
	GLuint grassDiffuseMap, grassSpecularMap, woodDiffuseMap, woodSpecularMap, stoneDiffuseMap, stoneSpecularMap;
	glGenTextures(1, &grassDiffuseMap);
	glGenTextures(1, &grassSpecularMap);
	glGenTextures(1, &woodDiffuseMap);
	glGenTextures(1, &woodSpecularMap);
	glGenTextures(1, &stoneDiffuseMap);
	glGenTextures(1, &stoneSpecularMap);

	int width, height;
	unsigned char* image; // width * height must be divided by 16 !!!!!!!!
	// GRASS
	// Diffuse map
	image = SOIL_load_image("grass.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, grassDiffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Specular map
	//	image = SOIL_load_image("grass1.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, grassSpecularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// WOOD
	// Diffuse map
	image = SOIL_load_image("wood.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, woodDiffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Specular map
	//	image = SOIL_load_image("wood3_1.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, woodSpecularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// STONE
	// Diffuse map
	image = SOIL_load_image("stone.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, stoneDiffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Specular map
	//	image = SOIL_load_image("stone2_1.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, stoneSpecularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassDiffuseMap);
	// Bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grassSpecularMap);

	// Bind diffuse map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, stoneDiffuseMap);
	// Bind specular map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, stoneSpecularMap);

	// Bind diffuse map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, woodDiffuseMap);
	// Bind specular map
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, woodSpecularMap);


	FillLightValues();



	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();

		// Clear the colorbuffer
		glClearColor(dirLightAmbient.x, dirLightAmbient.y, dirLightAmbient.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index 
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), dirLightDirection.x, dirLightDirection.y, dirLightDirection.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), dirLightAmbient.x, dirLightAmbient.y, dirLightAmbient.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), dirLightDiffuse.x, dirLightDiffuse.y, dirLightDiffuse.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), dirLightSpecular.x, dirLightSpecular.y, dirLightSpecular.z);
		
		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		
		
		skyShader.Use();
		 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		GLint viewLoc1 = glGetUniformLocation(skyShader.Program, "view1");
		GLint projLoc1 = glGetUniformLocation(skyShader.Program, "projection1");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc1, 1, GL_FALSE, glm::value_ptr(projection));
		glDepthMask(GL_FALSE);
		
		// ... set view and projection matrix
		glBindVertexArray(skyVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
		


		lightingShader.Use();


		glm::mat4 model;
		
		// Set texture units
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

		// floor
		glBindVertexArray(boxGrassVAO);
		model = glm::mat4();
		model = glm::scale(model, glm::vec3(10.0f, 0.001f, 10.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// Set texture units
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 2);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 3);

		// base
		glBindVertexArray(boxVAO);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.1f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// disc
		glBindVertexArray(discVAO);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 0.4f));
		//model = glm::rotate(model, glm::radians(lastFrame * 30), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));		
		model = glm::scale(model, glm::vec3(0.4f, 0.1f, 0.4f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLE_FAN, 0, numOfTrianglesOnCircle + 2);
		glDrawArrays(GL_TRIANGLE_FAN, numOfTrianglesOnCircle + 2, numOfTrianglesOnCircle + 2);
		glDrawArrays(GL_TRIANGLE_STRIP, (numOfTrianglesOnCircle + 2) * 2, (numOfTrianglesOnCircle + 2) * 2);
		glBindVertexArray(0);


		// Set texture units
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 4);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 5);

		// vertical stick
		glBindVertexArray(boxVAO);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, -0.7f));
		model = glm::scale(model, glm::vec3(0.1f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// horizontal stick
		glBindVertexArray(boxVAO);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.15f, -0.15f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// strut
		glBindVertexArray(boxVAO);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -0.55f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.4f));							// 0.09 instead 0.1 to prevent 2 textures be on exactly the same position => flickering
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// conenction of disc with horizontal stick
		glBindVertexArray(boxVAO);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.65f, 0.6f));

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -cos(glm::radians(70.0f)) / 2));
//		model = glm::rotate(model, glm::radians(lastFrame * 30), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, cos(glm::radians(70.0f)) / 2));

		model = glm::rotate(model, glm::radians(70.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 1.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		
	

		// Swap the screen buffers
		glfwSwapBuffers(window);


		
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.

	glfwTerminate();
	return 0;
}


void FillLightValues()
{
	// Directional light
	dirLightDirection = glm::vec3(-0.3f, -1.0f, -0.3f);
	dirLightAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
	dirLightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	dirLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
}

// Moves/alters the camera positions based on user input
void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (keys[GLFW_KEY_LEFT]){
		rotAngle -= 0.1;
	}
	if (keys[GLFW_KEY_RIGHT]){
		rotAngle += 0.1;
	}
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


GLfloat* GenerateDiscVertices(int numOfTrianglesOnCircle, int &textureVerticesArraySize)
{
	GLfloat* retArray = new GLfloat[(numOfTrianglesOnCircle + 2) * 2 * 8 + ((numOfTrianglesOnCircle + 2) * 2 * 8)];

	float y = 0.5f;

	// first circle
	//GenerateCircle(retArray, numOfTrianglesOnCircle, 0, -y);
	// second circle
	GenerateCircle(retArray, numOfTrianglesOnCircle, (numOfTrianglesOnCircle + 2) * 8, y);

	int offset = (numOfTrianglesOnCircle + 2) * 8 * 2;

	for (int i = 0; i < numOfTrianglesOnCircle + 2; i++)
	{
		retArray[i * 16 + offset + 0] = cos(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 16 + offset + 1] = y;
		retArray[i * 16 + offset + 2] = sin(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 16 + offset + 3] = cos(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 16 + offset + 4] = 0;
		retArray[i * 16 + offset + 5] = sin(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 16 + offset + 6] = i % 2;   // Tex
		retArray[i * 16 + offset + 7] = 1;		 // Tex

		retArray[i * 16 + offset + 8] = cos(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 16 + offset + 9] = -y;
		retArray[i * 16 + offset + 10] = sin(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 16 + offset + 11] = cos(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 16 + offset + 12] = 0;
		retArray[i * 16 + offset + 13] = sin(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 16 + offset + 14] = i % 2; // Tex
		retArray[i * 16 + offset + 15] = 0;		// Tex
	}


	textureVerticesArraySize = 4 * (((numOfTrianglesOnCircle + 2) * 2 * 8) + ((numOfTrianglesOnCircle + 2) * 2 * 8));
	return retArray;
}

void GenerateCircle(GLfloat* retArray, int numOfTrianglesOnCircle, int offset, float y)
{
	retArray[offset + 0 + 0] = 0;
	retArray[offset + 0 + 1] = y;
	retArray[offset + 0 + 2] = 0;
	retArray[offset + 0 + 3] = 0;
	retArray[offset + 0 + 4] = 1;
	retArray[offset + 0 + 5] = 0;
	retArray[offset + 0 + 6] = 0.5f; // Tex
	retArray[offset + 0 + 7] = 0.5f; // Tex

	for (int i = 0; i <= numOfTrianglesOnCircle; i++)
	{
		retArray[i * 8 + offset + 8] = cos(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 8 + offset + 9] = y;
		retArray[i * 8 + offset + 10] = sin(i * 2 * PI / numOfTrianglesOnCircle);
		retArray[i * 8 + offset + 11] = 0;
		retArray[i * 8 + offset + 12] = 1;
		retArray[i * 8 + offset + 13] = 0;
		retArray[i * 8 + offset + 14] = cos(i * 2 * PI / numOfTrianglesOnCircle) / 2 + 0.5f; // Tex
		retArray[i * 8 + offset + 15] = sin(i * 2 * PI / numOfTrianglesOnCircle) / 2 + 0.5f; // Tex
	}
}



unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}