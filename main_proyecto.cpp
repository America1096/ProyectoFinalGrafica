/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
/*---------------- Alumnos:                              ------------------*/
/*----------------          Cuevas Salgado Carlos        ------------------*/
/*----------------          Galindo Chavez Lisset America------------------*/
/*----------------------  Version: Visual Studio 2017 ---------------------*/
/* -------------------------- Primer Avance -------------------------------*/
/*----------------------  Instrucciones de uso ----------------------------*/
/*-------------     Tecla     ----    Movimiento   ------------------------*/
/*-------------       D       ----     Derecha     ------------------------*/
/*-------------       A       ----    Izquierda    ------------------------*/
/*-------------       W       ----     Aleja       ------------------------*/
/*-------------       S       ----     Acerca      ------------------------*/
/*------------- Mouse + botDer---- Rota la camara  ------------------------*/
/*------------------- Fecha de entrega: 21/04/2019 ------------------------*/
/*----------------------       2019-2      --------------------------------*/
/*------------- Computación gráfica e interacción humano computadora ---------------*/
/*------------  Se crea primer juego mecánico con primitivas  -------------*/

//glew include
#include <glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <glfw3.h>

// program include
#include "TimeManager.h"
#include "Shader.h"
// Geometry primitives
#include "Sphere.h"
#include "Cylinder.h"
#include "Box.h"
#include "FirstPersonCamera.h"

Sphere sphere(20, 20);
Cylinder cylinder(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinder3(20, 20, 0.5, 0.5);
Box box;

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader shader;
std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX, offsetX;
int lastMousePosY, offsetY;
int	mov = 0;
double deltaTime;
bool play = false;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);
float movX();
float movY();

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	shader.initialize("Shaders/transformaciones.vs", "Shaders/transformaciones.fs");

	cylinder.init();
	cylinder.setShader(&shader);
	cylinder.setColor(glm::vec3(1.0, 1.0, 0.0));

	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setColor(glm::vec3(1.0, 1.0, 0.0));

	cylinder3.init();
	cylinder3.setShader(&shader);
	cylinder3.setColor(glm::vec3(0.0, 0.0, 0.0));

	box.init();
	box.setShader(&shader);
	box.setColor(glm::vec3(0.2, 0.8, 0.4));

	camera->setPosition(glm::vec3(0.0f, 0.0f, 0.4f));
}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();

	shader.destroy();
	cylinder.destroy();
	cylinder2.destroy();
	cylinder3.destroy();
	box.destroy();

}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		play = true;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		play = false;
	glfwPollEvents();
	return continueApplication;
}

void animate(void)
{
	if (play)
	{
		if (mov < 694)
		{
			mov += 1;
		}
		else
		{
			mov = 0;
		}
		printf("%f, %f\n", movX(),movY());
	}
}

void crearTubo(float posx, float posy, glm::mat4 projection, glm::mat4 view) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / 4, posy / 4, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	cylinder.setProjectionMatrix(projection);
	cylinder.setViewMatrix(view);
	cylinder.enableWireMode();
	cylinder.render(model);
}

void crearTubo2(float posx, float posy, float rot, float scale,  glm::mat4 projection, glm::mat4 view) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / 4, posy / 4, 1.0f));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f, scale / 4, 0.5f));
	cylinder2.setProjectionMatrix(projection);
	cylinder2.setViewMatrix(view);
	cylinder2.enableWireMode();
	cylinder2.render(model);
}

void crearTubo3(float posx, float posy, float rot, float scale, glm::mat4 projection, glm::mat4 view) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / 4, posy / 4, -1.0f));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f, scale / 4, 0.5f));
	cylinder2.setProjectionMatrix(projection);
	cylinder2.setViewMatrix(view);
	cylinder2.enableWireMode();
	cylinder2.render(model);
}

void tubosCentrales(glm::mat4 projection, glm::mat4 view) {
	crearTubo(67.7033f, 31.5954, projection, view);
	crearTubo(59.2891, 20.5921, projection, view);
	crearTubo(54.7583, 16.7086, projection, view);
	crearTubo(47.3149, 16.3849, projection, view);
	crearTubo(38.2533, 21.8866, projection, view);
	crearTubo(31.4572, 29.0064, projection, view);
	crearTubo(25.9555, 32.8899, projection, view);
	crearTubo(21.1012, 32.8899, projection, view);
	crearTubo(12.6869, 28.0355, projection, view);
	crearTubo(7.8325, 25.7701, projection, view);
	crearTubo(2.3308, 26.0937, projection, view);
	crearTubo(-3.1708, 28.0355, projection, view);
	crearTubo(-10.6142, 32.5663, projection, view);
	crearTubo(-16.1159, 39.0388, projection, view);
	crearTubo(-22.9120, 44.8641, projection, view);
	crearTubo(-29.0609, 47.1294, projection, view);
	crearTubo(-34.8862, 45.8349, projection, view);
	crearTubo(-42.3296, 41.9514, projection, view);
	crearTubo(-50.4202, 38.0679, projection, view);
	crearTubo(-59.8054, 36.7734, projection, view);
	crearTubo(-70.1615, 39.0388, projection, view);
	crearTubo(-80.5175, 45.5113, projection, view);
	crearTubo(-88.6081, 53.2783, projection, view);
	crearTubo(-96.6988, 58.7800, projection, view);
	crearTubo(-106.7312, 63.3108, projection, view);
	crearTubo(-118.0581, 63.3108, projection, view);
	crearTubo(-124.8543, 59.7509, projection, view);
	crearTubo(-126.7961, 51.6602, projection, view);
	crearTubo(-126.7961, 42.5987, projection, view);
	crearTubo(-123.5598, 30.9481, projection, view);
	crearTubo(-118.3818, 23.1811, projection, view);
	crearTubo(-109.3202, 12.8251, projection, view);
	crearTubo(-97.9933, 4.4108, projection, view);
	crearTubo(-84.7246, -2.0618, projection, view);
	crearTubo(-67.2488, -5.2980, projection, view);
	crearTubo(-51.7148, -3.0326, projection, view);
	crearTubo(-38.7697, 3.4399, projection, view);
	crearTubo(-27.4428, 8.9415, projection, view);
	crearTubo(-19.6758, 11.2069, projection, view);
	crearTubo(-9.6433, 9.9124, projection, view);
	crearTubo(-0.9054, 6.6762, projection, view);
	crearTubo(6.2143, 6.0289, projection, view);
	crearTubo(12.6869, 8.2943, projection, view);
	crearTubo(20.7775, 10.2360, projection, view);
	crearTubo(28.2209, 9.9124, projection, view);
	crearTubo(34.3698, 4.4108, projection, view);
	crearTubo(42.1369, -2.0618, projection, view);
	crearTubo(49.9039, -4.3271, projection, view);
	crearTubo(58.3182, -2.0618, projection, view);
	crearTubo(64.4671, -0.1200, projection, view);
	crearTubo(69.9687, -2.7090, projection, view);
	crearTubo(73.8522, -10.4760, projection, view);
	crearTubo(75.7940, -18.8903, projection, view);
	crearTubo(76.1176, -27.6282, projection, view);
	crearTubo(73.8522, -35.0716, projection, view);
	crearTubo(68.0270, -44.1332, projection, view);
	crearTubo(57.3473, -48.9876, projection, view);
	crearTubo(53.1779, -50.0830, projection, view);
	crearTubo(49.7567, -50.3604, projection, view);
	crearTubo(45.3731, -49.9584, projection, view);
	crearTubo(39.5479, -48.9876, projection, view);
	crearTubo(31.1336, -43.1623, projection, view);
	crearTubo(25.3083, -37.0134, projection, view);
	crearTubo(18.8358, -28.2755, projection, view);
	crearTubo(12.6869, -20.5085, projection, view);
	crearTubo(7.5089, -15.6541, projection, view);
	crearTubo(0.0654, -14.0359, projection, view);
	crearTubo(-11.2615, -13.0650, projection, view);
	crearTubo(-21.6175, -13.3887, projection, view);
	crearTubo(-28.0900, -16.9486, projection, view);
	crearTubo(-32.2972, -25.0392, projection, view);
	crearTubo(-31.6499, -34.4244, projection, view);
	crearTubo(-29.0609, -43.1623, projection, view);
	crearTubo(-26.0803, -47.7138, projection, view);
	crearTubo(-21.6175, -52.2238, projection, view);
	crearTubo(-14.8214, -56.1073, projection, view);
	crearTubo(-6.7307, -57.7255, projection, view);
	crearTubo(2.9781, -58.3727, projection, view);
	crearTubo(15.5995, -59.0200, projection, view);
	crearTubo(26.2792, -59.3436, projection, view);
	crearTubo(39.8715, -59.6672, projection, view);
	crearTubo(54.1110, -59.6672, projection, view);
	crearTubo(69.3215, -59.6672, projection, view);
	crearTubo(77.4121, -57.0782, projection, view);
	crearTubo(80.0011, -50.6057, projection, view);
	crearTubo(80.3248, -39.9260, projection, view);
	crearTubo(80.3248, -28.2755, projection, view);
	crearTubo(78.0594, 12.8251, projection, view);
	crearTubo(76.7649, 21.8866, projection, view);
	crearTubo(73.8522, 29.3300, projection, view);
}

void tubosFrontales(glm::mat4 projection, glm::mat4 view) {
	crearTubo2(63.4962, 26.09375, 90.0f + (52.59493328717978f), 13.851764238897516, projection, view);
	crearTubo2(57.0237, 18.65035, 90.0f + (40.60098221686725f), 5.967388112901655, projection, view);
	crearTubo2(51.0366, 16.54675, 90.0f + 2.49f, 7.4504352389642285, projection, view);
	crearTubo2(42.7841, 19.13575, 90.0f - 31.26379769313227f, 10.601004549098166, projection, view);
	crearTubo2(34.85525, 25.4465, 90.0f - 46.33252609109743f, 9.842689025362937, projection, view);
	crearTubo2(28.70635, 30.94815, 90.0f - 35.21722498224841f, 6.734261291337008, projection, view);
	crearTubo2(23.52835, 32.8899, 90.0f - 0.0f, 4.854300000000002, projection, view);
	crearTubo2(16.89405, 30.4627, 90.0f + 29.981619719268817f, 9.714198054909112, projection, view);
	crearTubo2(10.2597, 26.9028, 90.0f + 25.01702270630387f, 5.35698016796777, projection, view);
	crearTubo2(5.08165, 25.9319, 90.0f - 3.3661554196722325f, 5.511208565278581, projection, view);
	crearTubo2(-0.42, 27.0646, 90.0f - 19.4405795787848f, 5.834225724121411, projection, view);
	crearTubo2(-6.8925, 30.3009, 90.0f - 31.32888822534934f, 8.713917155906406, projection, view);
	crearTubo2(-13.36505, 35.80255, 90.0f - 49.6350779928133f, 8.49481954722995, projection, view);
	crearTubo2(-19.51395, 41.95145, 90.0f - 40.601641789061766f, 8.951038783292137, projection, view);
	crearTubo2(-25.98645, 45.99675, 90.0f + (-20.224168528517907f), 6.552904340824761, projection, view);
	crearTubo2(-31.97355, 46.48215, 90.0f + (12.528703566628387f), 5.96739895934569, projection, view);
	crearTubo2(-38.6079, 43.89315, 90.0f + (27.55273265698377f), 8.395580730956015, projection, view);
	crearTubo2(-46.3749, 40.00965, 90.0f + (25.64107489225664f), 8.974373549724795, projection, view);
	crearTubo2(-55.1128, 37.42065, 90.0f + (7.853251326826877f), 9.474054532775286, projection, view);
	crearTubo2(-64.98345, 37.9061, 90.0f + (-12.33910377804815f), 10.600983179403697, projection, view);
	crearTubo2(-75.3395, 42.27505, 90.0f + (-32.005383208083494f), 12.212288575447268, projection, view);
	crearTubo2(-84.5628, 49.3948, 90.0f + (-43.83094912039673f), 11.215350969095883, projection, view);
	crearTubo2(-92.65345, 56.02915, 90.0f + (-34.215818352471494f), 9.784075295090497, projection, view);
	crearTubo2(-101.715, 61.0454, 90.0f + (-24.304732888795808f), 11.008051526042195, projection, view);
	crearTubo2(-112.39465, 63.3108, 90.0f + (-0.0f), 11.326899999999995, projection, view);
	crearTubo2(-121.4562, 61.53085, 90.0f + (27.645880857366844f), 7.672106780409145, projection, view);
	crearTubo2(-125.8252, 55.70555, 90.0f + (76.50405244757053f), 8.320457543308539, projection, view);
	crearTubo2(-126.7961, 47.129456, 90.0f + (-90.0f), 9.061500000000002, projection, view);
	crearTubo2(-125.17795, 36.7734, 90.0f + (-74.47578754643733f), 12.091737594324483, projection, view);
	crearTubo2(-120.9708, 27.0646, 90.0f + (-56.30993247402022f), 9.334772252176267, projection, view);
	crearTubo2(-113.851, 18.0031, 90.0f + (-48.813761484042274f), 13.760789605251581, projection, view);
	crearTubo2(-103.65675, 8.61795, 90.0f + (-36.60717725733272f), 14.110248335872758, projection, view);
	crearTubo2(-91.35895, 1.1745, 90.0f + (-26.00356704117095f), 14.763229675447047, projection, view);
	crearTubo2(-75.9867, -3.6799, 90.0f + (-10.491289167940792f), 17.77291692660492, projection, view);
	crearTubo2(-59.4818, -4.1653, 90.0f + (8.297235259864937f), 15.698318163421208, projection, view);
	crearTubo2(-45.24225, 0.203651, 90.0f + (26.564874134434728f), 14.47303942715558, projection, view);
	crearTubo2(-33.10625, 6.1907, 90.0f + (25.906355980592835f), 12.592309802812192, projection, view);
	crearTubo2(-23.5593, 10.0742, 90.0f + (16.2603746701528f), 8.090632000035596, projection, view);
	crearTubo2(-14.65955, 10.55965, 90.0f + (-7.352288755200629f), 10.115670343580794, projection, view);
	crearTubo2(-5.27435, 8.2943, 90.0f + (-20.32279512906578f), 9.31793361480967, projection, view);
	crearTubo2(2.65445, 6.35255, 90.0f + (-5.194864263944052f), 7.149064650707811, projection, view);
	crearTubo2(9.4506, 7.1616, 90.0f + (19.28996735901617f), 6.857593449600231, projection, view);
	crearTubo2(16.7322, 9.26515, 90.0f + (13.495438653029938f), 8.320336967334917, projection, view);
	crearTubo2(24.4992, 10.0742, 90.0f + (-2.4893524957693316f), 7.450430894921448, projection, view);
	crearTubo2(31.29535, 7.1616, 90.0f + (-41.819924742711514f), 8.250852911669192, projection, view);
	crearTubo2(38.25335, 1.1745, 90.0f + (-39.805643650163205f), 10.110509046037198, projection, view);
	crearTubo2(46.0204, -3.19445, 90.0f + (-16.259694821024816f), 8.090604000320374, projection, view);
	crearTubo2(54.11105, -3.19445, 90.0f + (15.067926505352949f), 8.713898586740607, projection, view);
	crearTubo2(61.39265, -1.0909, 90.0f + (17.5259251347696f), 6.448221340028586, projection, view);
	crearTubo2(67.2179, -1.4145, 90.0f + (-25.20122395391477f), 6.080339099096363, projection, view);
	crearTubo2(71.91045, -6.5925, 90.0f + (-63.43494882292203f), 8.683769990620434, projection, view);
	crearTubo2(74.8231, -14.68315, 90.0f + (-77.00513455287557f), 8.63545202812221, projection, view);
	crearTubo2(75.9558, -23.25925, 90.0f + (-87.87907337083882f), 8.743890059350015, projection, view);
	crearTubo2(74.9849, -31.3499, 90.0f + (73.07236441472665f), 7.780503885996071, projection, view);
	crearTubo2(70.9396, -39.6024, 90.0f + (57.265285042836226f), 10.77244399382053, projection, view);
	crearTubo2(62.68715, -46.5604, 90.0f + (24.443914359742816f), 11.73120588217597, projection, view);
	crearTubo2(55.2626, -49.5353, 90.0f + (14.720291560195822f), 4.310892891269738, projection, view);
	crearTubo2(51.4673, -50.2217, 90.0f + (4.635553361462997f), 3.4324277414098603, projection, view);
	crearTubo2(47.5649, -50.1594, 90.0f + (-5.239679526304752f), 4.40199420263135, projection, view);
	crearTubo2(42.4605, -49.473, 90.0f + (-9.461684205915338f), 5.90554042234917, projection, view);
	crearTubo2(35.34075, -46.07495, 90.0f + (-34.695224348247514f), 10.233990647836254, projection, view);
	crearTubo2(28.22095, -40.08785, 90.0f + (-46.54802847077413f), 8.47012935556477, projection, view);
	crearTubo2(22.07205, -32.64445, 90.0f + (-53.471223040193884f), 10.87401272116232, projection, view);
	crearTubo2(15.76135, -24.392, 90.0f + (-51.632401246860944f), 9.906324253223291, projection, view);
	crearTubo2(10.0979, -18.0813, 90.0f + (-43.15253696336998f), 7.097667459102323, projection, view);
	crearTubo2(3.78715, -14.845, 90.0f + (-12.26512525240675f), 7.617365915459227, projection, view);
	crearTubo2(-5.59805, -13.55045, 90.0f + (-4.899207230654043f), 11.368434827187075, projection, view);
	crearTubo2(-16.4395, -13.22685, 90.0f + (1.7903251495862291f), 10.361057749573641, projection, view);
	crearTubo2(-24.85375, -15.16865, 90.0f + (28.810963650313397f), 7.3868900262559745, projection, view);
	crearTubo2(-30.1936, -20.9939, 90.0f + (62.52507782499277f), 9.11911948600302, projection, view);
	crearTubo2(-31.97355, -29.7318, 90.0f + (-86.05454139824778f), 9.40749575232431, projection, view);
	crearTubo2(-30.3554, -38.79335, 90.0f + (-73.49568326976372f), 9.113386714608355, projection, view);
	crearTubo2(-27.5706, -45.438, 90.0f + (-56.78077326733756f), 5.440600022975405, projection, view);
	crearTubo2(-23.8489, -49.9688, 90.0f + (-45.30139263653787f), 6.3448155087441265, projection, view);
	crearTubo2(-18.21945, -54.16555, 90.0f + (-29.744972088854233f), 7.82742278531063, projection, view);
	crearTubo2(-10.77605, -56.9164, 90.0f + (-11.310341032121075f), 8.250939202902904, projection, view);
	crearTubo2(-1.8763, -58.0491, 90.0f + (-3.813761484042297f), 9.73034764435475, projection, view);
	crearTubo2(9.2888, -58.69635, 90.0f + (-2.9358940250233654f), 12.637987784849297, projection, view);
	crearTubo2(20.93935, -59.1818, 90.0f + (-1.7355584070412438f), 10.684601492334657, projection, view);
	crearTubo2(33.07535, -59.5054, 90.0f + (-1.3638171931305818f), 13.596151523501051, projection, view);
	crearTubo2(46.99125, -59.6672, 90.0f + (0.0f), 14.2395, projection, view);
	crearTubo2(61.71625, -59.6672, 90.0f + (0.0f), 15.210500000000003, projection, view);
	crearTubo2(73.3668, -58.3727, 90.0f + (17.74472301670676f), 8.49474716280596, projection, view);
	crearTubo2(78.7066, -53.84195, 90.0f + (68.19859051364821f), 6.971095842835618, projection, view);
	crearTubo2(80.16295, -45.26585, 90.0f + (88.26390559281641f), 10.684604521459834, projection, view);
	crearTubo2(80.3248, -34.10075, 90.0f + (90.0f), 11.650500000000001, projection, view);
	crearTubo2(79.1921, -7.72521, 90.0f + (-86.84513962572451f), 41.16298528435468, projection, view);
	crearTubo2(77.41215, 17.35585, 90.0f + (-81.86989764584403f), 9.153497282459858, projection, view);
	crearTubo2(75.30855, 25.6083, 90.0f + (-68.62894237996083f), 7.9929984893029955, projection, view);
	crearTubo2(70.77775, 30.4627, 90.0f + (-20.2249889747316f), 6.55293891090097, projection, view);
}

void tubosTraseros(glm::mat4 projection, glm::mat4 view) {
	crearTubo3(63.4962, 26.09375, 90.0f + (52.59493328717978f), 13.851764238897516, projection, view);
	crearTubo3(57.0237, 18.65035, 90.0f + (40.60098221686725f), 5.967388112901655, projection, view);
	crearTubo3(51.0366, 16.54675, 90.0f + (2.490120795985931f), 7.4504352389642285, projection, view);
	crearTubo3(42.7841, 19.13575, 90.0f + (-31.26379769313227f), 10.601004549098166, projection, view);
	crearTubo3(34.85525, 25.4465, 90.0f + (-46.33252609109743f), 9.842689025362937, projection, view);
	crearTubo3(28.70635, 30.94815, 90.0f + (-35.21722498224841f), 6.734261291337008, projection, view);
	crearTubo3(23.52835, 32.8899, 90.0f + (-0.0f), 4.854300000000002, projection, view);
	crearTubo3(16.89405, 30.4627, 90.0f + (29.981619719268817f), 9.714198054909112, projection, view);
	crearTubo3(10.2597, 26.9028, 90.0f + (25.01702270630387f), 5.35698016796777, projection, view);
	crearTubo3(5.08165, 25.9319, 90.0f + (-3.3661554196722325f), 5.511208565278581, projection, view);
	crearTubo3(-0.42, 27.0646, 90.0f + (-19.4405795787848f), 5.834225724121411, projection, view);
	crearTubo3(-6.8925, 30.3009, 90.0f + (-31.32888822534934f), 8.713917155906406, projection, view);
	crearTubo3(-13.36505, 35.80255, 90.0f + (-49.6350779928133f), 8.49481954722995, projection, view);
	crearTubo3(-19.51395, 41.95145, 90.0f + (-40.601641789061766f), 8.951038783292137, projection, view);
	crearTubo3(-25.98645, 45.99675, 90.0f + (-20.224168528517907f), 6.552904340824761, projection, view);
	crearTubo3(-31.97355, 46.48215, 90.0f + (12.528703566628387f), 5.96739895934569, projection, view);
	crearTubo3(-38.6079, 43.89315, 90.0f + (27.55273265698377f), 8.395580730956015, projection, view);
	crearTubo3(-46.3749, 40.00965, 90.0f + (25.64107489225664f), 8.974373549724795, projection, view);
	crearTubo3(-55.1128, 37.42065, 90.0f + (7.853251326826877f), 9.474054532775286, projection, view);
	crearTubo3(-64.98345, 37.9061, 90.0f + (-12.33910377804815f), 10.600983179403697, projection, view);
	crearTubo3(-75.3395, 42.27505, 90.0f + (-32.005383208083494f), 12.212288575447268, projection, view);
	crearTubo3(-84.5628, 49.3948, 90.0f + (-43.83094912039673f), 11.215350969095883, projection, view);
	crearTubo3(-92.65345, 56.02915, 90.0f + (-34.215818352471494f), 9.784075295090497, projection, view);
	crearTubo3(-101.715, 61.0454, 90.0f + (-24.304732888795808f), 11.008051526042195, projection, view);
	crearTubo3(-112.39465, 63.3108, 90.0f + (-0.0f), 11.326899999999995, projection, view);
	crearTubo3(-121.4562, 61.53085, 90.0f + (27.645880857366844f), 7.672106780409145, projection, view);
	crearTubo3(-125.8252, 55.70555, 90.0f + (76.50405244757053f), 8.320457543308539, projection, view);
	crearTubo3(-126.7961, 47.12945, 90.0f + (90.0f), 9.061500000000002, projection, view);
	crearTubo3(-125.17795, 36.7734, 90.0f + (-74.47578754643733f), 12.091737594324483, projection, view);
	crearTubo3(-120.9708, 27.0646, 90.0f + (-56.30993247402022f), 9.334772252176267, projection, view);
	crearTubo3(-113.851, 18.0031, 90.0f + (-48.813761484042274f), 13.760789605251581, projection, view);
	crearTubo3(-103.65675, 8.61795, 90.0f + (-36.60717725733272f), 14.110248335872758, projection, view);
	crearTubo3(-91.35895, 1.1745, 90.0f + (-26.00356704117095f), 14.763229675447047, projection, view);
	crearTubo3(-75.9867, -3.6799, 90.0f + (-10.491289167940792f), 17.77291692660492, projection, view);
	crearTubo3(-59.4818, -4.1653, 90.0f + (8.297235259864937f), 15.698318163421208, projection, view);
	crearTubo3(-45.24225, 0.20365, 90.0f + (26.564874134434728f), 14.47303942715558, projection, view);
	crearTubo3(-33.10625, 6.1907, 90.0f + (25.906355980592835f), 12.592309802812192, projection, view);
	crearTubo3(-23.5593, 10.0742, 90.0f + (16.2603746701528f), 8.090632000035596, projection, view);
	crearTubo3(-14.65955, 10.55965, 90.0f + (-7.352288755200629f), 10.115670343580794, projection, view);
	crearTubo3(-5.27435, 8.2943, 90.0f + (-20.32279512906578f), 9.31793361480967, projection, view);
	crearTubo3(2.65445, 6.35255, 90.0f + (-5.194864263944052f), 7.149064650707811, projection, view);
	crearTubo3(9.4506, 7.1616, 90.0f + (19.28996735901617f), 6.857593449600231, projection, view);
	crearTubo3(16.7322, 9.26515, 90.0f + (13.495438653029938f), 8.320336967334917, projection, view);
	crearTubo3(24.4992, 10.0742, 90.0f + (-2.4893524957693316f), 7.450430894921448, projection, view);
	crearTubo3(31.29535, 7.1616, 90.0f + (-41.819924742711514f), 8.250852911669192, projection, view);
	crearTubo3(38.25335, 1.1745, 90.0f + (-39.805643650163205f), 10.110509046037198, projection, view);
	crearTubo3(46.0204, -3.19445, 90.0f + (-16.259694821024816f), 8.090604000320374, projection, view);
	crearTubo3(54.11105, -3.19445, 90.0f + (15.067926505352949f), 8.713898586740607, projection, view);
	crearTubo3(61.39265, -1.0909, 90.0f + (17.5259251347696f), 6.448221340028586, projection, view);
	crearTubo3(67.2179, -1.4145, 90.0f + (-25.20122395391477f), 6.080339099096363, projection, view);
	crearTubo3(71.91045, -6.5925, 90.0f + (-63.43494882292203f), 8.683769990620434, projection, view);
	crearTubo3(74.8231, -14.68315, 90.0f + (-77.00513455287557f), 8.63545202812221, projection, view);
	crearTubo3(75.9558, -23.25925, 90.0f + (-87.87907337083882f), 8.743890059350015, projection, view);
	crearTubo3(74.9849, -31.3499, 90.0f + (73.07236441472665f), 7.780503885996071, projection, view);
	crearTubo3(70.9396, -39.6024, 90.0f + (57.265285042836226f), 10.77244399382053, projection, view);
	crearTubo3(62.68715, -46.5604, 90.0f + (24.443914359742816f), 11.73120588217597, projection, view);
	crearTubo3(55.2626, -49.5353, 90.0f + (14.720291560195822f), 4.310892891269738, projection, view);
	crearTubo3(51.4673, -50.2217, 90.0f + (4.635553361462997f), 3.4324277414098603, projection, view);
	crearTubo3(47.5649, -50.1594, 90.0f + (-5.239679526304752f), 4.40199420263135, projection, view);
	crearTubo3(42.4605, -49.473, 90.0f + (-9.461684205915338f), 5.90554042234917, projection, view);
	crearTubo3(35.34075, -46.07495, 90.0f + (-34.695224348247514f), 10.233990647836254, projection, view);
	crearTubo3(28.22095, -40.08785, 90.0f + (-46.54802847077413f), 8.47012935556477, projection, view);
	crearTubo3(22.07205, -32.64445, 90.0f + (-53.471223040193884f), 10.87401272116232, projection, view);
	crearTubo3(15.76135, -24.392, 90.0f + (-51.632401246860944f), 9.906324253223291, projection, view);
	crearTubo3(10.0979, -18.0813, 90.0f + (-43.15253696336998f), 7.097667459102323, projection, view);
	crearTubo3(3.78715, -14.845, 90.0f + (-12.26512525240675f), 7.617365915459227, projection, view);
	crearTubo3(-5.59805, -13.55045, 90.0f + (-4.899207230654043f), 11.368434827187075, projection, view);
	crearTubo3(-16.4395, -13.22685, 90.0f + (1.7903251495862291f), 10.361057749573641, projection, view);
	crearTubo3(-24.85375, -15.16865, 90.0f + (28.810963650313397f), 7.3868900262559745, projection, view);
	crearTubo3(-30.1936, -20.9939, 90.0f + (62.52507782499277f), 9.11911948600302, projection, view);
	crearTubo3(-31.97355, -29.7318, 90.0f + (-86.05454139824778f), 9.40749575232431, projection, view);
	crearTubo3(-30.3554, -38.79335, 90.0f + (-73.49568326976372f), 9.113386714608355, projection, view);
	crearTubo3(-27.5706, -45.43805, 90.0f + (-56.78077326733756f), 5.440600022975405, projection, view);
	crearTubo3(-23.8489, -49.9688, 90.0f + (-45.30139263653787f), 6.3448155087441265, projection, view);
	crearTubo3(-18.21945, -54.16555, 90.0f + (-29.744972088854233f), 7.82742278531063, projection, view);
	crearTubo3(-10.77605, -56.9164, 90.0f + (-11.310341032121075f), 8.250939202902904, projection, view);
	crearTubo3(-1.8763, -58.0491, 90.0f + (-3.813761484042297f), 9.73034764435475, projection, view);
	crearTubo3(9.2888, -58.69635, 90.0f + (-2.9358940250233654f), 12.637987784849297, projection, view);
	crearTubo3(20.93935, -59.1818, 90.0f + (-1.7355584070412438f), 10.684601492334657, projection, view);
	crearTubo3(33.07535, -59.5054, 90.0f + (-1.3638171931305818f), 13.596151523501051, projection, view);
	crearTubo3(46.99124, -59.6672, 90.0f + (0.0f), 14.2395, projection, view);
	crearTubo3(61.71625, -59.6672, 90.0f + (0.0f), 15.210500000000003, projection, view);
	crearTubo3(73.3668, -58.3727, 90.0f + (17.74472301670676f), 8.49474716280596, projection, view);
	crearTubo3(78.7066, -53.84195, 90.0f + (68.19859051364821f), 6.971095842835618, projection, view);
	crearTubo3(80.16295, -45.26585, 90.0f + (88.26390559281641f), 10.684604521459834, projection, view);
	crearTubo3(80.3248, -34.10075, 90.0f + (90.0f), 11.650500000000001, projection, view);
	crearTubo3(79.1921, -7.7252, 90.0f + (-86.84513962572451f), 41.16298528435468, projection, view);
	crearTubo3(77.41215, 17.35585, 90.0f + (-81.86989764584403f), 9.153497282459858, projection, view);
	crearTubo3(75.30855, 25.6083, 90.0f + (-68.62894237996083f), 7.9929984893029955, projection, view);
	crearTubo3(70.77775, 30.4627, 90.0f + (-20.2249889747316f), 6.55293891090097, projection, view);
}

float movX() {
	float x[] = {	
		15.5995, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
		26.2792, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39.8715, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
		54.111, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
		69.3215, 70, 71, 72, 73, 74, 75, 76,
		77.4121, 78, 79,
		80.0011, 80.01945867018736, 80.04976851035141, 80.08007835051546, 80.11038819067952, 80.14069803084355, 80.1710078710076, 80.20131771117165, 80.2316275513357, 80.26193739149976, 80.29224723166381, 80.32255707182786,
		80.3248, 80.30961487618185, 80.1993780426563, 80.08914120913076, 79.97890437560523, 79.86866754207968, 79.75843070855413, 79.64819387502858, 79.53795704150303, 79.4277202079775, 79.31748337445195, 79.2072465409264, 79.09700970740086, 78.98677287387531, 78.87653604034978, 78.76629920682423, 78.65606237329868, 78.54582553977313, 78.4355887062476, 78.32535187272205, 78.2151150391965, 78.10487820567096,
		78.0594, 78.0, 77.5, 77,
		76.7649, 76.0, 75.5, 75.0, 74.5, 74,
		73.8522, 73.0, 72.5, 72.0, 71.5, 71.0, 70.5, 70.0, 69.5, 69.0, 68.5, 68,
		67.7033, 66, 65, 64, 63, 62, 61, 60, 59.2891, 58, 57, 56, 55,
		54.7583, 53, 52, 51, 50, 49, 48, 47.3149, 46, 45, 44, 43, 42, 41, 40, 39,
		38.2533, 37, 36, 35, 34, 33, 32, 31.4572, 30, 29, 28, 27, 26,
		25.9555, 24, 23, 22, 21.1012, 20, 19, 18, 17, 16, 15, 14, 13,
		12.6869, 11, 10, 9, 8, 7.8325, 6, 5, 4, 3, 2.3308, 1, 0, -1, -2,
		-3.1708, -4, -5, -6, -7, -8, -9, -10.6142, -11, -12, -13, -14, -15,
		-16.1159, -17, -18, -19, -20, -21, -22.912, -23, -24, -25, -26, -27, -28,
		-29.0609, -30, -31, -32, -33, -34.8862, -35, -36, -37, -38, -39, -40, -41,
		-42.3296, -43, -44, -45, -46, -47, -48, -49, -50.4202, -51, -52, -53, -54, -55, -56, -57, -58,
		-59.8054, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69, -70.1615, -71, -72, -73, -74, -75, -76, -77, -78, -79,
		-80.5175, -81, -82, -83, -84, -85, -86, -87, -88.6081, -89, -90, -91, -92, -93, -94, -95,
		-96.6988, -97, -98, -99, -100, -101, -102, -103, -104, -105,
		-106.7312, -107, -108, -109, -110, -111, -112, -113, -114, -115, -116, -117,
		-118.0581, -119, -120, -121, -122, -123,
		-124.8543, -125, -125.27452292508683, -125.51452688024521, -125.75453083540361, -125.994534790562, -126.23453874572039, -126.47454270087879, -126.71454665603717
		-126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961,
		-126.7961, -125, -124,
		-123.5598, -122, -121, -120, -119,
		-118.3818, -117, -116, -115, -114, -113, -112, -111, -110,
		-109.3202, -108, -107, -106, -105, -104, -103, -102, -101, -100, -99, -98,
		-97.9933, -96, -95, -94, -93, -92, -91, -90, -89, -88, -87, -86, -85,
		-84.7246, -83, -82, -81, -80, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70, -69, -68,
		-67.2488, -66, -65, -64, -63, -62, -61, -60, -59, -58, -57, -56, -55, -54, -53, -52,
		-51.7148, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40, -39,
		-38.7697, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28,
		-27.4428, -26, -25, -24, -23, -22, -21, -20,
		-19.6758, -18, -17, -16, -15, -14, -13, -12, -11, -10,
		-9.6433, -8, -7, -6, -5, -4, -3, -2, -1,
		-0.9054, 1, 2, 3, 4, 5,
		6.2143, 7, 8, 9, 10, 11,
		12.6869, 13, 14, 15, 16, 17, 18, 19,
		20.7775, 21, 22, 23, 24, 25, 26, 27,
		28.2209, 29, 30, 31, 32, 33,
		34.3698, 35, 36, 37, 38, 39, 40, 41,
		42.1369, 43, 44, 45, 46, 47, 48,
		49.9039, 50, 51, 52, 53, 54, 55, 56, 57,
		58.3182, 59, 60, 61, 62, 63,
		64.4671, 65, 66, 67, 68,
		69.9687, 70, 71, 72,
		73.8522, 74, 74, 74.5, 75.0,
		75.794, 76,
		76.1176, 75, 74,
		73.8522, 72, 71, 70, 69,
		68.027, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58,
		57.3473, 56, 55, 54,
		53.1779, 52, 51, 50,
		49.7567, 48, 47, 46,
		45.3731, 44, 43, 42, 41, 40,
		39.5479, 38, 37, 36, 35, 34, 33, 32,
		31.1336, 30, 29, 28, 27, 26,
		25.3083, 24, 23, 22, 21, 20, 19,
		18.8358, 17, 16, 15, 14, 13,
		12.6869, 11, 10, 9, 8,
		7.5089, 6, 5, 4, 3, 2, 1,
		0.0654, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10,
		-11.2615, -12, -13, -14, -15, -16, -17, -18, -19, -20,
		-21.6175, -22, -23, -24, -25, -26, -27,
		-28.09, -29, -30, -31,
		-32.2972, -32.2309, -32.16196, -32.09299, -32.0240,
		-31.6499, -30,
		-29.0609, -28, -27,
		-26.0803, -25, -24, -23, -22,
		-21.6175, -20, -19, -18, -17, -16, -15,
		-14.8214, -13, -12, -11, -10, -9, -8, -7,
		-6.7307, -5, -4, -3, -2, -1, 0, 1,
		2.9781, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14

	};
	return x[mov];
}

float movY() {
	float y[] = {
		-59.02, -59.032135340880366, -59.062435817485515, -59.09273629409066, -59.12303677069581, -59.15333724730095, -59.1836377239061, -59.21393820051125, -59.2442386771164, -59.27453915372155, -59.3048396303267,
		-59.3436, -59.360760515880315, -59.38456811282859, -59.40837570977686, -59.432183306725136, -59.4559909036734, -59.479798500621676, -59.50360609756995, -59.52741369451822, -59.5512212914665, -59.575028888414764, -59.59883648536304, -59.62264408231131,
		-59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672,
		-59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672,
		-59.6672, -59.45007932909797, -59.130078340296144, -58.81007735149433, -58.490076362692506, -58.17007537389069, -57.85007438508887, -57.53007339628705,
		-57.0782, -55.60844999999999, -53.10844999999998,
		-50.6057, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40,
		//
		-28.2755, -28, -26, -24, -22, -20, -18, -16, -14, -12, -10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10, 12,
		12.8251, 13.240899999999977, 16.74089999999998, 20.240899999999982,
		21.8866, 23.841300676348396, 25.119049843787543, 26.396799011226687, 27.67454817866583, 28.952297346104977,
		29.33, 29.643970609377284, 29.82818241961977, 30.01239422986225, 30.196606040104733, 30.380817850347217, 30.565029660589698, 30.74924147083218, 30.933453281074662, 31.117665091317146, 31.30187690155963, 31.48608871180211,
		31.5954, 29.36798, 28.0603, 26.7526, 25.4449, 24.1372, 22.8295, 21.5217,
		20.5921, 19.487169336541008, 18.630035938465614, 17.77290254039022, 16.915769142314826,
		16.7086, 16.63213471, 16.58864652, 16.545158332, 16.501670, 16.458182, 16.414694,
		16.3849, 17.183234215811776, 17.79037864946588, 18.397523083119978,
		21.8866, 23.19959500301644, 24.247225261547065, 25.294855520077693, 26.34248577860832, 27.39011603713895, 28.437746295669577,
		29.0064, 30.034997742515948, 30.740870472762964, 31.446743203009976, 32.15261593325699, 32.858488663504005,
		32.8899, 32.8899, 32.8899, 32.8899,
		32.8899, 32.25459281104786, 31.677670191222084, 31.10074757139631, 30.52382495157054, 29.946902331744766, 29.369979711918994, 28.79305709209322, 28.21613447226745,
		28.0355, 27.248275366677653, 26.781605953361897, 26.314936540046144, 25.848267126730388,
		25.7701, 25.877884321209805, 25.93670250468037, 25.995520688150933, 26.054338871621496,
		26.0937, 26.563408346662786, 26.91636021521012, 27.269312083757452, 27.622263952304785,
		28.0355, 28.540234309589703, 29.148934634709942, 29.757634959830185, 30.366335284950424, 30.975035610070666, 31.583735935190905,
		32.5663, 33.020176165548826, 34.196630715960524, 35.373085266372215, 36.549539816783906, 37.7259943671956,
		39.0388, 39.796609292094, 40.65376265946646, 41.51091602683893, 42.36806939421139, 43.22522276158385,
		44.8641, 44.89651984745239, 45.26492720486591, 45.63333456227943, 46.001741919692954, 46.37014927710647, 46.738556634519995,
		47.1294, 46.920712902339794, 46.69849258750622, 46.47627227267265, 46.25405195783908,
		45.8349, 45.77552628637451, 45.253788908294595, 44.73205153021468, 44.21031415213478, 43.688576774054866, 43.166839395974954, 42.64510201789505,
		41.9514, 41.62960700566089, 41.14960552245816, 40.66960403925543, 40.189602556052705, 39.70960107284998, 39.22959958964724, 38.749598106444516,
		38.0679, 37.98792822529089, 37.849998293057155, 37.71206836082342, 37.5741384285897, 37.43620849635597, 37.298278564122235, 37.16034863188851, 37.02241869965478,
		36.7734, 36.81596880872143, 37.034719110475955, 37.25346941223047, 37.472219713985, 37.690970015739516, 37.90972031749404, 38.12847061924856, 38.347220921003085, 38.565971222757604, 38.78472152451212,
		39.0388, 39.5628625, 40.1878625, 40.8128625, 41.4378625, 42.0628625, 42.6878625, 43.3128625, 43.9378625, 44.5628625,
		45.5113, 45.97450143129063, 46.934504397696095, 47.89450736410155, 48.85451033050701, 49.81451329691247, 50.77451626331793, 51.73451922972339,
		53.2783, 53.54479316251994, 54.22479612888873, 54.90479909525752, 55.58480206162631, 56.26480502799511, 56.9448079943639, 57.62481096073269,
		58.78, 58.916026968621665, 59.36764373031378, 59.8192604920059, 60.27087725369802, 60.72249401539013, 61.174110777082255, 61.62572753877437, 62.077344300466486, 62.52896106215861,
		63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108,
		63.3108, 62.81742578941173, 62.2936183676172, 61.76981094582266, 61.24600352402813, 60.722196102233596,
		59.7509, 59.14382667112986, 58, 57, 56, 55, 54, 53, 52,
		51.6602, 50, 49, 48, 47, 46, 45, 44, 43, 42,
		42.5987, 36.13278439885055, 32.53280911843774,
		30.9481, 28.608400000000007, 27.108400000000003, 25.608400000000003, 24.108400000000003,
		23.1811, 21.601917427385896, 20.4590728966187, 19.316228365851508, 18.173383835084316, 17.03053930431712, 15.887694773549928, 14.744850242782734, 13.60200571201554,
		12.8251, 11.844376336861806, 11.10151641932038, 10.35865650177895, 9.615796584237522, 8.872936666696095, 8.130076749154664, 7.387216831613237, 6.644356914071809, 5.901496996530381, 5.158637078988952, 4.415777161447524,
		4.4108, 3.4384490100763436, 2.950639352762515, 2.4628296954486864, 1.9750200381348582, 1.48721038082103, 0.9994007235072013, 0.5115910661933731, 0.02378140887954494, -0.4640282484342837, -0.9518379057481123, -1.439647563061941, -1.9274572203757687,
		-2.0618, -2.3811645223680737, -2.5663463166206975, -2.751528110873321, -2.936709905125945, -3.1218916993785686, -3.3070734936311923, -3.492255287883816, -3.6774370821364397, -3.8626188763890634, -4.047800670641687, -4.232982464894311, -4.418164259146934, -4.603346053399559, -4.7885278476521815, -4.973709641904806, -5.158891436157429,
		-5.298, -5.115881323548345, -4.970046380842024, -4.824211438135702, -4.67837649542938, -4.532541552723059, -4.386706610016738, -4.240871667310416, -4.095036724604094, -3.9492017818977727, -3.803366839191451, -3.6575318964851298, -3.5116969537788085, -3.365862011072487, -3.220027068366165, -3.0741921256598435,
		-3.0326, -2.1752066233555567, -1.675210485820891, -1.1752143482862252, -0.6752182107515594, -0.17522207321689365, 0.3247740643177721, 0.8247702018524379, 1.3247663393871032, 1.824762476921769, 2.3247586144564347, 2.8247547519911005, 3.3247508895257663,
		3.4399, 4.299462768277287, 4.785173774819236, 5.270884781361184, 5.756595787903133, 6.242306794445081, 6.728017800987031, 7.213728807528979, 7.699439814070928, 8.185150820612877, 8.670861827154825,
		8.9415, 9.36232131067336, 9.653991196086004, 9.945661081498647, 10.23733096691129, 10.529000852323934, 10.820670737736577, 11.112340623149219,
		11.2069, 10.99067043608273, 10.861639785696486, 10.732609135310241, 10.603578484923997, 10.474547834537752, 10.345517184151507, 10.216486533765263, 10.087455883379018, 9.958425232992774,
		9.9124, 9.303781515009327, 8.93341792650408, 8.563054337998832, 8.192690749493584, 7.822327160988338, 7.45196357248309, 7.081599983977843, 6.711236395472596,
		6.6762, 6.502967220528955, 6.412050468418613, 6.321133716308271, 6.23021696419793, 6.139300212087588,
		6.0289, 6.303893786113772, 6.653892241139573, 7.003890696165374, 7.353889151191176, 7.703887606216976,
		8.2943, 8.369442297233826, 8.60943685882382, 8.849431420413813, 9.089425982003807, 9.3294205435938, 9.569415105183793, 9.809409666773787,
		10.236, 10.226326866754441, 10.182852110594622, 10.139377354434801, 10.095902598274982, 10.052427842115163, 10.008953085955342, 9.965478329795523,
		9.9124, 9.215316528159509, 8.320587389614403, 7.425858251069297, 6.531129112524191, 5.636399973979084,
		4.4108, 3.885631981048266, 3.0522965019119086, 2.2189610227755514, 1.3856255436391942, 0.5522900645028366, -0.2810454146335202, -1.1143808937698783,
		-2.0618, -2.3135291657010435, -2.6051861761297803, -2.896843186558517, -3.188500196987254, -3.4801572074159908, -3.7718142178447276,
		-4.3271, -4.301227933399094, -4.03200767740632, -3.7627874214135457, -3.4935671654207714, -3.224346909427997, -2.9551266534352227, -2.685906397442449, -2.4166861414496745,
		-2.0618, -1.8464900681422685, -1.5306937468490296, -1.2148974255557903, -0.8991011042625512, -0.5833047829693121,
		-0.12, -0.3707776101497738, -0.8413679838592405, -1.3119583575687073, -1.7825487312781738,
		-2.709, -2.7716000000000034, -4.771600000000005, -6.7716000000000065,
		-10.476, -11.116453980842534, -11.116453980842534, -13.283077732001251, -15.44970148315997,
		-18.8903, -24.452745611866604,
		-27.6282, -31.300286095170815, -34.5859751567052,
		-35.0716, -37.95285652681453, -39.50844260111241, -41.064028675410285, -42.61961474970816,
		-44.1332, -44.60001730760228, -45.054561910915105, -45.50910651422793, -45.963651117540756, -46.41819572085358, -46.87274032416641, -47.32728492747923, -47.781829530792066, -48.23637413410489, -48.69091873741772,
		-48.9876, -49.34156757806878, -49.604291231352235, -49.86701488463568,
		-50.083, -50.178507266456215, -50.25958992751082, -50.34067258856541,
		-50.3604, -50.1993010402409, -50.107595592663564, -50.015890145086225,
		-49.9584, -49.72956571448191, -49.56291049234361, -49.396255270205316, -49.22960004806702, -49.062944825928724,
		-48.9876, -47.91597409291326, -47.22366457221635, -46.53135505151944, -45.83904553082253, -45.14673601012562, -44.45442648942871, -43.7621169687318,
		-43.1623, -41.96572762776166, -40.910176840677735, -39.854626053593805, -38.79907526650988, -37.74352447942595,
		-37.0134, -35.24718994669757, -33.897186084202396, -32.547182221707224, -31.197178359212053, -29.84717449671688, -28.49717063422171,
		-28.2755, -25.9566041649726, -24.69345140594253, -23.43029864691246, -22.167145887882388, -20.903993128852317,
		-20.5085, -18.927023105446118, -17.989518277327154, -17.05201344920819, -16.114508621089225,
		-15.6541, -15.326068565862833, -15.108670836300128, -14.891273106737422, -14.673875377174715, -14.45647764761201, -14.239079918049304,
		-14.0359, -13.944577850073719, -13.858861546407224, -13.773145242740732, -13.68742893907424, -13.601712635407745, -13.515996331741253, -13.43028002807476, -13.344563724408268, -13.258847420741773, -13.17313111707528,
		-13.065, -13.088083473348783, -13.11934071552723, -13.150597957705678, -13.181855199884124, -13.213112442062572, -13.24436968424102, -13.275626926419466, -13.306884168597914, -13.33814141077636,
		-13.3887, -13.599076477404404, -14.149080339899575, -14.699084202394747, -15.249088064889918, -15.79909192738509, -16.349095789880263,
		-16.9486, -18.698563396082907, -20.621600095075113, -22.54463679406732,
		-25.0392, -26, -27, -28, -29, -30, -31, -32, -33,
		-34.4244, -39.99282843182696,
		-43.1623, -44.78233836475877, -46.30937990002013,
		-47.7138, -48.805525598279104, -49.81610191807833, -50.826678237877566, -51.83725455767679,
		-52.2238, -53.14808911434499, -53.719519787819486, -54.290950461293974, -54.86238113476847, -55.433811808242965, -56.00524248171746,
		-56.1073, -56.4715935073603, -56.671600923282284, -56.87160833920427, -57.07161575512625, -57.27162317104824, -57.47163058697022, -57.67163800289221,
		-57.7255, -57.84087049274884, -57.907531666117336, -57.97419283948582, -58.04085401285432, -58.107515186222805, -58.1741763595913, -58.24083753295979,
		-58.3727, -58.37382316145594, -58.42510907268608, -58.47639498391621, -58.52768089514634, -58.57896680637647, -58.63025271760661, -58.68153862883674, -58.73282454006687, -58.784110451297, -58.83539636252714, -58.88668227375727, -58.937968184987405,

	};

	return y[mov];
}

void crearCarrito(glm::mat4 projection, glm::mat4 view) {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 tmp = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(movX()/4, movY()/4, 0));
	//model = glm::translate(model, glm::vec3(2, -61.6672 / 4, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 3.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	cylinder3.setProjectionMatrix(projection);
	cylinder3.setViewMatrix(view);
	cylinder3.enableWireMode();
	cylinder3.render(model);

/*	model = glm::mat4(1.0f);
	model = glm::translate(tmp, glm::vec3(6, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 3.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	cylinder3.setProjectionMatrix(projection);
	cylinder3.setViewMatrix(view);
	cylinder3.enableWireMode();
	cylinder3.render(model);

	model = glm::mat4(1.0f);
	model = glm::translate(tmp, glm::vec3(4, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 4, 0.5f));
	cylinder3.setProjectionMatrix(projection);
	cylinder3.setViewMatrix(view);
	cylinder3.enableWireMode();
	cylinder3.render(model);
*/
	box.setProjectionMatrix(projection);
	box.setViewMatrix(view);
	//box.setPosition(glm::vec3(6.25, -60.6672 / 4, 2.0f));
	box.setPosition(glm::vec3(movX() / 4, movY() / 4, 2.0f));
	box.setScale(glm::vec3(0.25, 0.5, 1.0));
	box.enableWireMode();
	box.render();

	box.setProjectionMatrix(projection);
	box.setViewMatrix(view);
//	box.setPosition(glm::vec3(5.85, -60.6672 / 4, 2.0f));
	box.setPosition(glm::vec3(movX() / 4, movY() / 4, 2.0f));
	box.setScale(glm::vec3(0.5, 0.15, 1.0));
	box.enableWireMode();
	box.render();

	box.setProjectionMatrix(projection);
	box.setViewMatrix(view);
	//box.setPosition(glm::vec3(6.25, -60.6672 / 4, -2.0f));
	box.setPosition(glm::vec3(movX() / 4, movY() / 4, -2.0f));
	box.setScale(glm::vec3(0.25, 0.5, 1.0));
	box.enableWireMode();
	box.render();

	box.setProjectionMatrix(projection);
	box.setViewMatrix(view);
//	box.setPosition(glm::vec3(5.85, -60.6672 / 4, -2.0f));
	box.setPosition(glm::vec3(movX() / 4, movY() / 4, -2.0f));
	box.setScale(glm::vec3(0.5, 0.15, 1.0));
	box.enableWireMode();
	box.render();

/*	box.setProjectionMatrix(projection);
	box.setViewMatrix(view);
	box.setPosition(glm::vec3(1.65, -60.6672 / 4, 2.0f));
	box.setScale(glm::vec3(0.25, 0.5, 1.0));
	box.enableWireMode();
	box.render();

	box.setProjectionMatrix(projection);
	box.setViewMatrix(view);
	box.setPosition(glm::vec3(2.05, -60.6672 / 4, 2.0f));
	box.setScale(glm::vec3(0.5, 0.15, 1.0));
	box.enableWireMode();
	box.render();

	box.setProjectionMatrix(projection);
	box.setViewMatrix(view);
	box.setPosition(glm::vec3(1.65, -60.6672 / 4, -2.0f));
	box.setScale(glm::vec3(0.25, 0.5, 1.0));
	box.enableWireMode();
	box.render();

	box.setProjectionMatrix(projection);
	box.setViewMatrix(view);
	box.setPosition(glm::vec3(2.05, -60.6672 / 4, -2.0f));
	box.setScale(glm::vec3(0.5, 0.15, 1.0));
	box.enableWireMode();
	box.render();*/
}
void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		psi = processInput(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		animate();

		crearTubo(0, 0, projection, view);
		tubosCentrales(projection, view);
		tubosFrontales(projection, view);
		tubosTraseros(projection, view);
		crearCarrito(projection, view);
		
		glfwSwapBuffers(window);

	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}