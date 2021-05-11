#pragma once

/*   ===== Library(Include) =====   */
//Standard Library
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

//GLFW and GLEW Library
#include <glew.h>
#include <glfw3.h>

//GLM Library
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//SOIL Library
#include <SOIL2.h>


/*   ===== Class =====   */
class Shader {
	private :
        //Core Program
		unsigned int core_program;

        //Read File
		std::string vertexSource, fragmentSource;
		std::string readFile(char& type);
		unsigned int compileShader(char type);
	public : 
        //Shader
		Shader(std::string vertexSrc, std::string fragmentSrc);

        //Projection and Camera
        void projection_camera(glm::mat4* projection, glm::mat4* camera);

        //Use Program
		void useProgram() {
			glUseProgram(core_program);
		}
        // utility uniform functions 
        // ------------------------------------------------------------------------
        void setBool(const std::string& name, bool value) {
            glUniform1i(glGetUniformLocation(core_program, name.c_str()), (int)value);
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string& name, int value) {
            glUniform1i(glGetUniformLocation(core_program, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string& name, float value) {
            glUniform1f(glGetUniformLocation(core_program, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string& name, const glm::vec2& value) {
            glUniform2fv(glGetUniformLocation(core_program, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string& name, float x, float y) {
            glUniform2f(glGetUniformLocation(core_program, name.c_str()), x, y);
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string& name, const glm::vec3& value) {
            glUniform3fv(glGetUniformLocation(core_program, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string& name, float x, float y, float z) {
            glUniform3f(glGetUniformLocation(core_program, name.c_str()), x, y, z);
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string& name, const glm::vec4& value) {
            glUniform4fv(glGetUniformLocation(core_program, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string& name, float x, float y, float z, float w) {
            glUniform4f(glGetUniformLocation(core_program, name.c_str()), x, y, z, w);
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string& name, const glm::mat2& mat) {
            glUniformMatrix2fv(glGetUniformLocation(core_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string& name, const glm::mat3& mat) {
            glUniformMatrix3fv(glGetUniformLocation(core_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string& name, const glm::mat4& mat) {
            glUniformMatrix4fv(glGetUniformLocation(core_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
};

class SolarSystem {
    private:
        //Saturn
        void makeSaturn(Shader mainShader, unsigned int& VAO, unsigned int& texture_saturn);
        void makeRing_Saturn(Shader mainShader, unsigned int& ringVAO, unsigned int& texture_ringSaturn);


    public :
        void sun(Shader lightShader, unsigned int& lightVAO, unsigned int& texture_sun);
        void mercury(Shader mainShader, unsigned int& VAO, unsigned int& texture_mercury);
        void venus(Shader mainShader, unsigned int& VAO, unsigned int& texture_venus);
        void earth(Shader mainShader, unsigned int& VAO, unsigned int* texture_earth);
        void mars(Shader mainShader, unsigned int& VAO, unsigned int& texture_mars);
        void jupiter(Shader mainShader, unsigned int& VAO, unsigned int& texture_jupiter);
        void saturn(Shader mainShader, unsigned int& VAO, unsigned int& ringVAO, unsigned int* texture_saturn);
        void uranus(Shader mainShader, unsigned int& VAO, unsigned int& texture_uranus);
        void neptune(Shader mainShader, unsigned int& VAO, unsigned int& texture_neptune);
};


/*   ===== Function =====   */
//Loading Screen
void loadingScreen(Shader* mainShader, unsigned int circleVAO, int loadingAt);

//Create Window
GLFWwindow* window;
void createWindow(GLFWwindow** window);

//Frame Buffer
int frameBuffer_w = 0, frameBuffer_h = 0;
void frameBuffer(GLFWwindow* window, int w, int h);

//Input
void input();

//Mouse call back (Look around)
void mouse_callBack(GLFWwindow* window, double x_currMouse, double y_currMouse);

//Mouse scroll call back (Change Planet's Speed)
void mouseScroll_callBack(GLFWwindow* window, double x_scroll, double y_scroll);

//VAO, VBO, EBO
void funcVAO(unsigned int* VAO);
void func_lightVAO(unsigned int* VAO);
void func_ringVAO(unsigned int* VAO);
void func_circleVAO(unsigned int* VAO);
void funcVBO(unsigned int* VBO, std::vector<float> vertices);

//Texture
unsigned int loadTexture(std::string filename);

//Cube Map
unsigned int loadCubeMap(std::string filename);

//Sphere Maker
#define Pi 3.14159265
int stack = 70, sector = 70;
std::vector<float> generateSphere(std::vector<float> vertices, float radius);
std::vector<unsigned int> generateSphereI(std::vector<unsigned int> indices);
std::vector<float> generateRing(std::vector<float> ringvertices, float radius1, float radius2);
std::vector<float> generateCircle(std::vector<float> circleVertices, float radius);


/*   ===== Vertex and Index =====   */
std::vector<float> vertices;
std::vector<unsigned int> indices;
std::vector<float> ringVertices;
std::vector<float> circleVertices;


/*   ===== Variables =====   */
//Rotation and Revolusion
float mercuryDistance = 150.f;
float venusDistance = 180.f;
float earthDistance = 230.f;
float marsDistance = 265.f;
float jupiterDistance = 350.f;
float saturnDistance = 480.f;
float uranusDistance = 580.f;
float neptuneDistance = 650.f;

//Planet Distance
float planetDistance[8] = {
    mercuryDistance,
    venusDistance,
    earthDistance,
    marsDistance,
    jupiterDistance,
    saturnDistance,
    uranusDistance,
    neptuneDistance
};

float theta;

glm::vec3 sunPos(0.f);
glm::vec3 mercuryPos(0.f, 0.f, 150.f);
glm::vec3 venusPos(0.f, 0.f, 180.f);
glm::vec3 earthPos(0.f, 0.f, 230.f);
glm::vec3 marsPos(0.f, 0.f, 265.f);
glm::vec3 jupiterPos(0.f, 0.f, 350.f);
glm::vec3 saturnPos(0.f, 0.f, 480.f);
glm::vec3 uranusPos(0.f, 0.f, 580.f);
glm::vec3 neptunePos(0.f, 0.f, 650.f);

//Rotation
float mercuryRotation;
float venusRotation;
float earthRotation;
float marsRotation;
float jupiterRotation;
float saturnRotation;
float uranusRotation;
float neptuneRotation;

//Camera
glm::vec3 cameraPos(0.f);
glm::vec3 cameraFront(0.f, 0.f, -1.f);
glm::vec3 cameraUp(0.f, 1.f, 0.f);

//Look Around (Mouse)
glm::vec3 direction(1.f);
float x_lastMouse = 400.f, y_lastMouse = 300.f;
float yaw = 0.f;
float pitch = 0.f;
bool firstMouse = true;

//Loading Texture
unsigned int loadingTexture[8];

//Delta Time
float deltaTime = 0;
float lastFrame = 0;

//Attenuation
glm::vec3 attenuation(1.0, 0.0014, 0.000007);

//Planet's Speed
float speed = 1.f;

//Planet Stop
int stop = false;
int q_stop = 0;
int q_continue = 0;
int q_switch = 0;
