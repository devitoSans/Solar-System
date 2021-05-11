#include "libs.h"

int main() {
	//Create Window
	createWindow(&window);

	//Shader
	Shader mainShader("vertex.glsl", "fragment.glsl");
	Shader lightShader("light_Vertex.glsl", "light_Fragment.glsl");
	Shader cubemapShader("cm_Vertex.glsl", "cm_Frag.glsl");

	//Sphere Maker
	vertices = generateSphere(vertices, 1.f);
	indices = generateSphereI(indices);
	ringVertices = generateRing(ringVertices, 30, 25);
	circleVertices = generateCircle(circleVertices, 1.f);

	//VAO, VBO, EBO
	unsigned int VAO, VBO, lightVAO;
	funcVBO(&VBO, vertices);
	funcVAO(&VAO);
	func_lightVAO(&lightVAO);

	unsigned int ringVAO, ringVBO;
	funcVBO(&ringVBO, ringVertices);
	func_ringVAO(&ringVAO);

	unsigned int circleVAO, circleVBO;
	funcVBO(&circleVAO, circleVertices);
	func_circleVAO(&circleVAO);

	/*==================================================*/
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glfwSwapBuffers(window);

	//Loading Texture
	int frames = sizeof(loadingTexture) / 4;
	std::string name;
	for (int i = 0; i < frames; i++) {
		name = std::to_string(i);
		name = "loading_screen/frame_" + name + "_delay-0.1s.gif";

		loadingTexture[i] = loadTexture(name);
	}

	//Texture
	unsigned int texture_planet[7];

	loadingScreen(&mainShader, circleVAO, 0);
	texture_planet[0] = loadTexture("Sun2.jpg");
	loadingScreen(&mainShader, circleVAO, 1);
	texture_planet[1] = loadTexture("Mercury2.jpg");
	loadingScreen(&mainShader, circleVAO, 2);
	texture_planet[2] = loadTexture("Venus2.jpg");
	loadingScreen(&mainShader, circleVAO, 3);
	texture_planet[3] = loadTexture("Mars2.jpg");
	loadingScreen(&mainShader, circleVAO, 4);
	texture_planet[4] = loadTexture("Jupiter2.jpg");
	loadingScreen(&mainShader, circleVAO, 5);
	texture_planet[5] = loadTexture("Uranus2.jpg");
	loadingScreen(&mainShader, circleVAO, 6);
	texture_planet[6] = loadTexture("Neptune2.jpg");
	loadingScreen(&mainShader, circleVAO, 7);

	unsigned int texture_saturn[2];

	texture_saturn[0] = loadTexture("Saturn2.jpg");
	loadingScreen(&mainShader, circleVAO, 0);
	texture_saturn[1] = loadTexture("ring_Saturn2.png");
	loadingScreen(&mainShader, circleVAO, 1);

	unsigned int texture_earth[3];

	texture_earth[0] = loadTexture("Earth2.jpg");
	loadingScreen(&mainShader, circleVAO, 2);
	texture_earth[1] = loadTexture("night_Earth2.jpg");
	loadingScreen(&mainShader, circleVAO, 3);
	texture_earth[2] = loadTexture("cloud_Earth2.jpg");
	loadingScreen(&mainShader, circleVAO, 4);
	unsigned int cubemapTexture = loadCubeMap("stars2.jpg");
	loadingScreen(&mainShader, circleVAO, 5);

	//Solar System
	SolarSystem planet;

	//Main Loop
	while (!glfwWindowShouldClose(window)) {
		//RENDER
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//INPUT
		input();

		//DRAW
		//Orbit
		for (int i = 0; i < sizeof(planetDistance) / 4; i++) {
			mainShader.setBool("begin", false);
			mainShader.useProgram();
			glBindVertexArray(circleVAO);
			mainShader.setBool("no_texture", true);

			glm::mat4 model(1.f);
			model = glm::scale(model, glm::vec3(planetDistance[i]));
			mainShader.setMat4("model", model);

			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 200);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glm::mat4 projection(1.f), camera(1.f);
			mainShader.projection_camera(&projection, &camera);
			mainShader.setBool("no_texture", false);
		}

		//Sun
		planet.sun(lightShader, lightVAO, texture_planet[0]);
		//Mercury
		planet.mercury(mainShader, VAO, texture_planet[1]);
		//Venus
		planet.venus(mainShader, VAO, texture_planet[2]);
		//Earth
		planet.earth(mainShader, VAO, texture_earth);
		//Mars
		planet.mars(mainShader, VAO, texture_planet[3]);
		//Jupiter
		planet.jupiter(mainShader, VAO, texture_planet[4]);
		//Saturn
		planet.saturn(mainShader, VAO, ringVAO, texture_saturn);
		//Uranus
		planet.uranus(mainShader, VAO, texture_planet[5]);
		//Neptune
		planet.neptune(mainShader, VAO, texture_planet[6]);

		//Cubemap
		glDepthFunc(GL_LEQUAL);
		cubemapShader.useProgram();

		glm::mat4 model(1.f);
		model = glm::translate(model, cameraPos);
		model = glm::scale(model, glm::vec3(1000.f));
		cubemapShader.setMat4("model", model);

		glm::mat4 projection(1.f), camera(1.f);
		cubemapShader.projection_camera(&projection, &camera);

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
		glDepthFunc(GL_LESS);


		//Rotation and Revolusion
		theta = glfwGetTime();
		//Planet Rotation
		theta *= (speed * 10.f);
		mercuryRotation = theta / 4208.96f;
		venusRotation = theta / 6991.26f;
		earthRotation = theta / 28.96f;
		marsRotation = theta / 52.64f;
		jupiterRotation = theta;
		saturnRotation = theta / 1.24f;
		uranusRotation = theta / 3.08f;
		neptuneRotation = theta / 4.69f;
		//Planet Revolusion
		theta /= (speed * 10.f);
		theta *= (speed / 20.f);
		float mercuryRev_x = cos(theta / 5.f) * mercuryDistance;
		float mercuryRev_z = sin(theta / 5.f) * mercuryDistance;

		float venusRev_x = cos(theta / (1.367f * 5.f)) * venusDistance;
		float venusRev_z = sin(theta / (1.367f * 5.f)) * venusDistance;

		float earthRev_x = cos(theta / (1.607f * 5.f)) * earthDistance;
		float earthRev_z = sin(theta / (1.607f * 5.f)) * earthDistance;

		float marsRev_x = cos(theta / (1.988f * 5.f)) * marsDistance;
		float marsRev_z = sin(theta / (1.988f * 5.f)) * marsDistance;

		float jupiterRev_x = cos(theta / (3.662f * 5.f)) * jupiterDistance;
		float jupiterRev_z = sin(theta / (3.662f * 5.f)) * jupiterDistance;

		float saturnRev_x = cos(theta / (4.94f * 5.f)) * saturnDistance;
		float saturnRev_z = sin(theta / (4.94f * 5.f)) * saturnDistance;

		float uranusRev_x = cos(theta / (7.029f * 5.f)) * uranusDistance;
		float uranusRev_z = sin(theta / (7.029f * 5.f)) * uranusDistance;

		float neptuneRev_x = cos(theta / (8.816f * 5.f)) * neptuneDistance;
		float neptuneRev_z = sin(theta / (8.816f * 5.f)) * neptuneDistance;
		
		//stop all planets' revolution
		if (!stop) {
			mercuryPos = glm::vec3(mercuryRev_x, 0.f, mercuryRev_z);
			venusPos = glm::vec3(venusRev_x, 0.f, venusRev_z);
			earthPos = glm::vec3(earthRev_x, 0.f, earthRev_z);
			marsPos = glm::vec3(marsRev_x, 0.f, marsRev_z);
			jupiterPos = glm::vec3(jupiterRev_x, 0.f, jupiterRev_z);
			saturnPos = glm::vec3(saturnRev_x, 0.f, saturnRev_z);
			uranusPos = glm::vec3(uranusRev_x, 0.f, uranusRev_z);
			neptunePos = glm::vec3(neptuneRev_x, 0.f, neptuneRev_z);
		}

		//DELTA TIME
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//SWAP BUFFERS
		glfwSwapBuffers(window);
		glFinish();
	}

	//delete
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


/*   ===== Function =====   */
//Loading Screen
void loadingScreen(Shader* mainShader, unsigned int circleVAO, int loadingAt) {
	glfwPollEvents();
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainShader->useProgram();
	glBindVertexArray(circleVAO);

	glm::mat4 model, projection, camera;

	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0.f, 0.f, -3.f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
	mainShader->setMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, loadingTexture[loadingAt]);
	mainShader->setBool("begin", true);
	mainShader->setInt("texSampler", 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 200);

	projection = glm::mat4(1.f), camera = glm::mat4(1.f);
	mainShader->projection_camera(&projection, &camera);
	glfwSwapBuffers(window);
}

//Create Window
void createWindow(GLFWwindow** window) {
	//GLFW
	glfwInit();

	//Window Setting
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Create Window
	*window = glfwCreateWindow(640, 480, "test", NULL, NULL);
	glfwMakeContextCurrent(*window);
	//Create Icon
	int icon_w = 0;
	int icon_h = 0;
	GLFWimage icon[1];
	icon[0].pixels = SOIL_load_image("image/icon2.png", &icon_w, &icon_h, NULL, SOIL_LOAD_RGBA);
	icon[0].width = icon_w;
	icon[0].height = icon_h;
	glfwSetWindowIcon(*window, 1, icon);
	//Frame buffer
	glfwSetFramebufferSizeCallback(*window, frameBuffer);
	//Mouse
	glfwSetCursorPosCallback(*window, mouse_callBack);
	//Mouse scroll back
	glfwSetScrollCallback(*window, mouseScroll_callBack);

	//GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//OpenGL Setting
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

//Frame Buffer
void frameBuffer(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

//VAO, VBO, EBO
void funcVAO(unsigned int* VAO) {
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
void func_lightVAO(unsigned int* VAO) {
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
void func_ringVAO(unsigned int* VAO) {
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
void func_circleVAO(unsigned int* VAO) {
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
void funcVBO(unsigned int* VBO, std::vector<float> vertices_what) {
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices_what.size() * sizeof(float), &vertices_what[0], GL_STATIC_DRAW);
}

//Texture
unsigned int loadTexture(std::string filename) {
	unsigned int texture;

	std::string file = "image/" + filename;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int image_w, image_h;
	unsigned char* image = SOIL_load_image(file.c_str(), &image_w, &image_h, NULL, SOIL_LOAD_RGBA);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::MAIN.CPP::FAILED_TO_LOAD_IMAGE\n";
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return texture;
}

//Cube Map
unsigned int loadCubeMap(std::string filename) {
	//generate texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	//texture's option
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//attach texture
	int image_w, image_h, faces_size = 6;
	std::string file = "image/" + filename;
	for (int i = 0; i < faces_size; i++) {
		unsigned char* image = SOIL_load_image(file.c_str(), &image_w, &image_h, NULL, SOIL_LOAD_RGBA);
		if (image) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
		
		else std::cout << "ERROR::MAIN.CPP::Cubemap tex failed to load at path:" << file << std::endl;
		SOIL_free_image_data(image);
	}

	return texture;
}

//Input
void input() {
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);

	//Keyboard
	glm::vec3 cameraRight = glm::cross(cameraFront, cameraUp);
	float movementSpeed = 50.f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += movementSpeed * glm::normalize(glm::cross(cameraUp, cameraRight));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= movementSpeed * glm::normalize(glm::cross(cameraUp, cameraRight));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= movementSpeed * glm::normalize(cameraRight);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += movementSpeed * glm::normalize(cameraRight);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos.y += movementSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraPos.y -= movementSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || q_continue == 1) {
		q_continue = 1;
		if (q_stop == 1) {
			if (q_switch == 0)
				stop = false;
			else 
				stop = true;
			q_continue = 0;
			q_stop = 0;
		}
		else if (!glfwGetKey(window, GLFW_KEY_Q)) {
			q_stop = 1;
			if (q_switch == 0)
				q_switch = 1;
			else if (q_switch == 1)
				q_switch = 0;
		}
	}

	//Mouse
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = direction;
}

//Look Around
void mouse_callBack(GLFWwindow* window, double x_currMouse, double y_currMouse) {
	if (firstMouse) {
		x_lastMouse = x_currMouse;
		y_lastMouse = y_currMouse;
		firstMouse = false;
	}

	float x_mouse = x_currMouse - x_lastMouse;
	float y_mouse = y_lastMouse - y_currMouse;
	x_lastMouse = x_currMouse;
	y_lastMouse = y_currMouse;

	float sensivity = 0.04f;
	x_mouse *= sensivity;
	y_mouse *= sensivity;

	yaw = glm::mod(yaw + x_mouse, 360.f);
	pitch += y_mouse;

	if (pitch > 89.f) pitch = 89.f;
	if (pitch < -89.f) pitch = -89.f;
}

//Planet's speed
void mouseScroll_callBack(GLFWwindow* window, double x_scroll, double y_scroll) {
	speed += (float)y_scroll * 1.f;

	if (speed < 1.f) speed = 1.f;
	else if (speed > 200.f) speed = 200.f;
}

//Sphere Maker
std::vector<float> generateSphere(std::vector<float> vertices, float radius) {
	double incrementSector = (2 * Pi) / sector;
	double incrementStack = Pi / stack;
	float angleStack, angleSector;
	float x = 0.f, y = 0.f, z = 0.f, xz = 0.f;
	float nx = 0.f, ny = 0.f, nz = 0.f, lengthInv = 1.f / radius;
	float tx = 0.f, ty = 0.f;

	for (double j = 0; j <= stack; j++) {
		angleStack = Pi / 2 - j * incrementStack;
		xz = radius * cos(angleStack);
		y = radius * sin(angleStack);
	
		for (double i = 0; i <= sector; i++) {
			angleSector = i * incrementSector;
			x = xz * cos(angleSector);
			z = xz * sin(angleSector);

			tx = float(i) / sector;
			ty = float(j) / stack;

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			vertices.push_back(nx);
			vertices.push_back(ny);
			vertices.push_back(nz);

			vertices.push_back(tx);
			vertices.push_back(ty);
		}
	}

	return vertices;
}
std::vector<unsigned int> generateSphereI(std::vector<unsigned int> indices) {
	int k1, k2;

	for (int i = 0; i < stack; i++) {
		k1 = i * (sector + 1);     // beginning of current stack
		k2 = k1 + sector + 1;      // beginning of next stack

		for (int j = 0; j < sector; j++, k1++, k2++) {
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stack - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	return indices;
}
std::vector<float> generateRing(std::vector<float> ringvertices, float radius1, float radius2) {
	double increment = 2 * Pi / 50;
	float x, y = 0.f, z, x_Temp, z_Temp;

	for (double i = 0; i < 2 * Pi; i += increment) {
		x_Temp = cos(i) * radius2;
		z_Temp = sin(i) * radius2;

		ringvertices.push_back(x_Temp);
		ringvertices.push_back(y);
		ringvertices.push_back(z_Temp);
		ringvertices.push_back(x_Temp * (1.f / radius2));
		ringvertices.push_back(0.f);
		ringvertices.push_back(z_Temp * (1.f / radius2));
		ringvertices.push_back(cos(i) * 0.42f + 0.499f);
		ringvertices.push_back(sin(i) * 0.42f + 0.508f);

		x = cos(i) * radius1;
		z = sin(i) * radius1;

		ringvertices.push_back(x);
		ringvertices.push_back(y);
		ringvertices.push_back(z);
		ringvertices.push_back(x * (1.f / radius1));
		ringvertices.push_back(0.f);
		ringvertices.push_back(z * (1.f / radius1));
		ringvertices.push_back(cos(i) * 0.42f + 0.499f);
		ringvertices.push_back(sin(i) * 0.42f + 0.508f);
	}

	ringvertices.push_back(radius2);
	ringvertices.push_back(0.f);
	ringvertices.push_back(0.f);	//-> z

	ringvertices.push_back(radius2 * (1.f / radius2));
	ringvertices.push_back(0.f);
	ringvertices.push_back(0.f);

	ringvertices.push_back(0.42f + 0.499f);
	ringvertices.push_back(0.508f);

	ringvertices.push_back(radius1);
	ringvertices.push_back(0.f);
	ringvertices.push_back(0.f);	//-> z

	ringvertices.push_back(radius1 * (1.f / radius1));
	ringvertices.push_back(0.f);
	ringvertices.push_back(0.f);

	ringvertices.push_back(0.42f + 0.499f);
	ringvertices.push_back(0.508f);

	return ringvertices;
}
std::vector<float> generateCircle(std::vector<float> circleVertices, float radius) {
	float increment = 2 * Pi / 200;
	float x, y, z;
	float tx, ty;

	for (float i = 0; i < 2 * Pi; i += increment) {
		x = cos(i) * radius;
		y = 0.f;
		z = sin(i) * radius;

		tx = cos(i) * 0.42f + 0.499f;
		ty = sin(i) * 0.42f + 0.499f;

		circleVertices.push_back(x);
		circleVertices.push_back(y);
		circleVertices.push_back(z);
		circleVertices.push_back(tx);
		circleVertices.push_back(ty);
	}

	return circleVertices;
}


/*   ===== SolarSystem Class' Function =====   */
/*== Private ==*/
void SolarSystem::makeSaturn(Shader mainShader, unsigned int& VAO, unsigned int& texture_saturn) {
	mainShader.useProgram();
	glBindVertexArray(VAO);
	mainShader.setVec3("lightColor", glm::vec3(1.f));
	mainShader.setVec3("lightPos", glm::vec3(0.f));

	//texture
	glActiveTexture(GL_TEXTURE0);
	mainShader.setInt("texSampler", 0);
	mainShader.setInt("set_nightOff", 0);
	mainShader.setBool("set_cloudOff", 0);
	glBindTexture(GL_TEXTURE_2D, texture_saturn);

	//Attenuation
	mainShader.setFloat("att.constant", attenuation.x);
	mainShader.setFloat("att.linear", attenuation.y);
	mainShader.setFloat("att.quadratic", attenuation.z);

	//model matrix
	glm::mat4 model(1.f);
	model = glm::translate(model, saturnPos);
	model = glm::scale(model, glm::vec3(15.f));
	model = glm::rotate(model, saturnRotation - (theta / (4.94f * 5.f)), glm::vec3(0.f, 1.f, 0.f));
	mainShader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	mainShader.projection_camera(&projection, &camera);
}
void SolarSystem::makeRing_Saturn(Shader mainShader, unsigned int& ringVAO, unsigned int& texture_ringSaturn) {
	mainShader.useProgram();
	glBindVertexArray(ringVAO);

	//Texture
	glActiveTexture(GL_TEXTURE0);
	mainShader.setInt("texSampler", 0);
	glBindTexture(GL_TEXTURE_2D, texture_ringSaturn);

	mainShader.setInt("set_nightOff", 0);
	mainShader.setBool("set_cloudOff", 0);
	mainShader.setVec3("lightColor", glm::vec3(1.f));
	mainShader.setVec3("lightPos", glm::vec3(0.f));

	//Attenuation
	mainShader.setFloat("att.constant", attenuation.x);
	mainShader.setFloat("att.linear", attenuation.y);
	mainShader.setFloat("att.quadratic", attenuation.z);

	for (int i = 0; i < 2; i++) {
		//model matrix
		glm::mat4 model(1.f);
		model = glm::translate(model, saturnPos);
		if (i == 1) model = glm::scale(model, glm::vec3(1.3f, 1.f, 1.3f));
		model = glm::rotate(model, ((saturnRotation - (theta / (4.94f * 5.f))) / 4), glm::vec3(0.f, 1.f, 0.f));
		mainShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 102 * 2);

	}

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	mainShader.projection_camera(&projection, &camera);
}

/*== Public ==*/
//Solar System
void SolarSystem::sun(Shader lightShader, unsigned int& lightVAO, unsigned int& texture_sun) {
	lightShader.useProgram();
	glBindVertexArray(lightVAO);

	//texture
	glActiveTexture(GL_TEXTURE0);
	lightShader.setInt("texSampler", 0);
	glBindTexture(GL_TEXTURE_2D, texture_sun);

	//model matrix
	glm::mat4 model(1.f);
	model = glm::translate(model, sunPos);
	model = glm::scale(model, glm::vec3(100.f));
	lightShader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	lightShader.projection_camera(&projection, &camera);
}
void SolarSystem::mercury(Shader mainShader, unsigned int& VAO, unsigned int& texture_mercury) {
	mainShader.useProgram();
	glBindVertexArray(VAO);

	//texture
	glActiveTexture(GL_TEXTURE0);
	mainShader.setInt("texSampler", 0);
	glBindTexture(GL_TEXTURE_2D, texture_mercury);

	mainShader.setInt("set_nightOff", 0);
	mainShader.setBool("set_cloudOff", 0);
	mainShader.setVec3("lightColor", glm::vec3(1.f));
	mainShader.setVec3("lightPos", glm::vec3(0.f));

	//Attenuation
	mainShader.setFloat("att.constant", attenuation.x);
	mainShader.setFloat("att.linear", attenuation.y);
	mainShader.setFloat("att.quadratic", attenuation.z);

	//model matrix
	glm::mat4 model(1.f);
	model = glm::translate(model, mercuryPos);
	model = glm::scale(model, glm::vec3(0.6f));
	model = glm::rotate(model, mercuryRotation + (theta / 5.f), glm::vec3(0.f, 1.f, 0.f));
	mainShader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	mainShader.projection_camera(&projection, &camera);
}
void SolarSystem::venus(Shader mainShader, unsigned int& VAO, unsigned int& texture_venus) {
	mainShader.useProgram();
	glBindVertexArray(VAO);
	mainShader.setVec3("lightColor", glm::vec3(1.f));
	mainShader.setVec3("lightPos", glm::vec3(0.f));

	//texture
	glActiveTexture(GL_TEXTURE0);
	mainShader.setInt("texSampler", 0);
	mainShader.setInt("set_nightOff", 0);
	mainShader.setBool("set_cloudOff", 0);
	glBindTexture(GL_TEXTURE_2D, texture_venus);

	//Attenuation
	mainShader.setFloat("att.constant", attenuation.x);
	mainShader.setFloat("att.linear", attenuation.y);
	mainShader.setFloat("att.quadratic", attenuation.z);

	//model matrix
	glm::mat4 model(1.f);
	model = glm::translate(model, venusPos);
	model = glm::scale(model, glm::vec3(2.f));
	model = glm::rotate(model, venusRotation + (theta / (1.367f * 5.f)), glm::vec3(0.f, 1.f, 0.f));
	mainShader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	mainShader.projection_camera(&projection, &camera);
}
void SolarSystem::earth(Shader mainShader, unsigned int& VAO, unsigned int* texture_earth) {
	mainShader.useProgram();
	glBindVertexArray(VAO);
	mainShader.setVec3("lightColor", glm::vec3(1.f));
	mainShader.setVec3("lightPos", glm::vec3(0.f));

	//texture
	glActiveTexture(GL_TEXTURE0);
	mainShader.setInt("texSampler", 0);
	glBindTexture(GL_TEXTURE_2D, texture_earth[0]);

	glActiveTexture(GL_TEXTURE1);
	mainShader.setInt("texSampler_night", 1);
	mainShader.setInt("set_nightOff", 1);
	glBindTexture(GL_TEXTURE_2D, texture_earth[1]);

	glActiveTexture(GL_TEXTURE2);
	mainShader.setInt("texSampler", 2);
	mainShader.setBool("set_cloudOff", 1);
	glBindTexture(GL_TEXTURE_2D, texture_earth[2]);

	//Attenuation
	mainShader.setFloat("att.constant", attenuation.x);
	mainShader.setFloat("att.linear", attenuation.y);
	mainShader.setFloat("att.quadratic", attenuation.z);

	//model matrix
	glm::mat4 model(1.f);
	model = glm::translate(model, earthPos);
	model = glm::scale(model, glm::vec3(2.f));
	model = glm::rotate(model, earthRotation - (theta / (1.607f * 5.f)), glm::vec3(0.f, 1.f, 0.f));
	mainShader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	mainShader.projection_camera(&projection, &camera);
}
void SolarSystem::mars(Shader mainShader, unsigned int& VAO, unsigned int& texture_mars) {
	mainShader.useProgram();
	glBindVertexArray(VAO);
	mainShader.setVec3("lightColor", glm::vec3(1.f));
	mainShader.setVec3("lightPos", glm::vec3(0.f));

	//texture
	glActiveTexture(GL_TEXTURE0);
	mainShader.setInt("texSampler", 0);
	mainShader.setInt("set_nightOff", 0);
	mainShader.setBool("set_cloudOff", 0);
	glBindTexture(GL_TEXTURE_2D, texture_mars);

	//Attenuation
	mainShader.setFloat("att.constant", attenuation.x);
	mainShader.setFloat("att.linear", attenuation.y);
	mainShader.setFloat("att.quadratic", attenuation.z);

	//model matrix
	glm::mat4 model(1.f);
	model = glm::translate(model, marsPos);
	model = glm::scale(model, glm::vec3(0.8f));
	model = glm::rotate(model, marsRotation - (theta / (1.988f * 5.f)), glm::vec3(0.f, 1.f, 0.f));
	mainShader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	mainShader.projection_camera(&projection, &camera);
}
void SolarSystem::jupiter(Shader mainShader, unsigned int& VAO, unsigned int& texture_jupiter) {
	mainShader.useProgram();
	glBindVertexArray(VAO);
	mainShader.setVec3("lightColor", glm::vec3(1.f));
	mainShader.setVec3("lightPos", glm::vec3(0.f));

	//texture
	glActiveTexture(GL_TEXTURE0);
	mainShader.setInt("texSampler", 0);
	mainShader.setInt("set_nightOff", 0);
	mainShader.setBool("set_cloudOff", 0);
	glBindTexture(GL_TEXTURE_2D, texture_jupiter);

	//Attenuation
	mainShader.setFloat("att.constant", attenuation.x);
	mainShader.setFloat("att.linear", attenuation.y);
	mainShader.setFloat("att.quadratic", attenuation.z);

	//model matrix
	glm::mat4 model(1.f);
	model = glm::translate(model, jupiterPos);
	model = glm::scale(model, glm::vec3(25.f));
	model = glm::rotate(model, jupiterRotation - (theta / (3.662f * 5.f)), glm::vec3(0.f, 1.f, 0.f));
	mainShader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	mainShader.projection_camera(&projection, &camera);
}
void SolarSystem::saturn(Shader mainShader, unsigned int& VAO, unsigned int& ringVAO, unsigned int* texture_saturn) {
	makeSaturn(mainShader, VAO, texture_saturn[0]);
	makeRing_Saturn(mainShader, ringVAO, texture_saturn[1]);
}
void SolarSystem::uranus(Shader mainShader, unsigned int& VAO, unsigned int& texture_uranus) {
	mainShader.useProgram();
	glBindVertexArray(VAO);
	mainShader.setVec3("lightColor", glm::vec3(1.f));
	mainShader.setVec3("lightPos", glm::vec3(0.f));

	//texture
	glActiveTexture(GL_TEXTURE0);
	mainShader.setInt("texSampler", 0);
	mainShader.setInt("set_nightOff", 0);
	mainShader.setBool("set_cloudOff", 0);
	glBindTexture(GL_TEXTURE_2D, texture_uranus);

	//Attenuation
	mainShader.setFloat("att.constant", attenuation.x);
	mainShader.setFloat("att.linear", attenuation.y);
	mainShader.setFloat("att.quadratic", attenuation.z);

	//model matrix
	glm::mat4 model(1.f);
	model = glm::translate(model, uranusPos);
	model = glm::scale(model, glm::vec3(10.f));
	model = glm::rotate(model, uranusRotation - (theta / (7.029f * 5.f)), glm::vec3(0.f, 1.f, 0.f));
	mainShader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	mainShader.projection_camera(&projection, &camera);
}
void SolarSystem::neptune(Shader mainShader, unsigned int& VAO, unsigned int& texture_neptune) {
	mainShader.useProgram();
	glBindVertexArray(VAO);
	mainShader.setVec3("lightColor", glm::vec3(1.f));
	mainShader.setVec3("lightPos", glm::vec3(0.f));

	//texture
	glActiveTexture(GL_TEXTURE0);
	mainShader.setInt("texSampler", 0);
	mainShader.setInt("set_nightOff", 0);
	mainShader.setBool("set_cloudOff", 0);
	glBindTexture(GL_TEXTURE_2D, texture_neptune);

	//Attenuation
	mainShader.setFloat("att.constant", attenuation.x);
	mainShader.setFloat("att.linear", attenuation.y);
	mainShader.setFloat("att.quadratic", attenuation.z);

	//model matrix
	glm::mat4 model(1.f);
	model = glm::translate(model, neptunePos);
	model = glm::scale(model, glm::vec3(7.f));
	model = glm::rotate(model, neptuneRotation - (theta / (8.816f * 5.f)), glm::vec3(0.f, 1.f, 0.f));
	mainShader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	//PROJECTION and CAMERA
	glm::mat4 projection(1.f), camera(1.f);
	mainShader.projection_camera(&projection, &camera);
}


/*   ===== Shader Class' Function =====   */
/*== Private ==*/
//Shader
std::string Shader::readFile(char& type) {
	std::ifstream file;
	std::string src, srcTemp;

	if (type == 'v')
		file.open(vertexSource);
	else
		file.open(fragmentSource);

	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, srcTemp);
			src.append("\n" + srcTemp);
		}
	}
	else {
		std::cout << "ERROR::MAIN.CPP::CANNOT_OPEN_FILE_TYPE: " << type << std::endl;
	}

	return src;
}
unsigned int Shader::compileShader(char type) {
	std::string src;
	const char* srcChar;
	unsigned int shader;
	int success;
	char infoLog[516];

	src = readFile(type);
	srcChar = src.c_str();
	if (type == 'v')
		shader = glCreateShader(GL_VERTEX_SHADER);
	else
		shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(shader, 1, &srcChar, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 516, NULL, infoLog);
		std::cout << infoLog << std::endl;
		std::cout << "ERROR::MAIN.CPP::FAILED_TO_COMPILED_SHADER_TYPE: " << type << std::endl;
	}

	return shader;
}

/*== Public ==*/
//Projection and Camera
void Shader::projection_camera(glm::mat4* projection, glm::mat4* camera) {
	//Projection
	glfwGetFramebufferSize(window, &frameBuffer_w, &frameBuffer_h);
	if (frameBuffer_w <= 0 && frameBuffer_h <= 0) {
		frameBuffer_w = 640;
		frameBuffer_h = 480;
	}
	float aspect = (float)frameBuffer_w / frameBuffer_h;
	*projection = glm::perspective(glm::radians(45.f), aspect, 0.01f, 100000.f);	//-> 100.000
	setMat4("projection", *projection);

	//Camera
	*camera = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	setMat4("camera", *camera);
}

//Shader
Shader::Shader(std::string vertexSrc, std::string fragmentSrc) {
	vertexSource = vertexSrc;
	fragmentSource = fragmentSrc;

	core_program = glCreateProgram();
	unsigned int vertexShader = compileShader('v');
	unsigned int fragmentShader = compileShader('f');

	glAttachShader(core_program, vertexShader);
	glAttachShader(core_program, fragmentShader);
	glLinkProgram(core_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}