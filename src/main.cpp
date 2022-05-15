#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"
#include "shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void key_calllback(GLFWwindow *window, int key, int scancode, int action,
				   int mods);

// settings
int shifted = 0;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
int rotate = 0, turn_table = 0;
glm ::mat4 transform(1.0f);
glm ::mat4 model(1.0f);
// camera
Camera camera(glm::vec3(1.0f, 1.0f, 10.0f));

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_center = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 camera_right = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 prism_center = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 view;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "Enter number of sides\n" << std::endl;
		return -1;
	}
	int sides = atoi(argv[1]);
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow *window =
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader ourShader("../src/vertex.shader", "../src/fragment.shader");

	// set up vertex data (and buffer(s)) and configure vertex attributes

	float vertices1[18 * sides], vertices2[18 * sides], vertices[72 * sides];
	std::vector<float> vx, vy;
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// float vertices[] = {
	//      0.5f,  0.5f, 0.0f,  // top right
	//      0.5f, -0.5f, 0.0f,  // bottom right
	//     -0.5f, -0.5f, 0.0f,  // bottom left
	//     -0.5f,  0.5f, 0.0f   // top left
	// };
	for (int x = 0; x < sides; x++) {
		float angle = 2 * M_PI * x / sides;
		vx.push_back(0.5f * cos(angle));
		vy.push_back(0.5f * sin(angle));
	}
	for (int x = 0; x < sides; x++) {
		vertices1[18 * x] = 0.0f;
		vertices1[18 * x + 1] = 0.0f;
		vertices1[18 * x + 2] = 0.5f;
		vertices1[18 * x + 3] = 1.0f;
		vertices1[18 * x + 4] = 0.5f;
		vertices1[18 * x + 5] = 0.0f;

		vertices1[18 * x + 6] = vx[x];
		vertices1[18 * x + 7] = vy[x];
		vertices1[18 * x + 8] = 0.5f;
		vertices1[18 * x + 9] = 1.0f;
		vertices1[18 * x + 10] = 0.5f;
		vertices1[18 * x + 11] = 0.0f;

		vertices1[18 * x + 12] = vx[(x + 1) % sides];
		vertices1[18 * x + 13] = vy[(x + 1) % sides];
		vertices1[18 * x + 14] = 0.5f;
		vertices1[18 * x + 15] = 1.0f;
		vertices1[18 * x + 16] = 0.5f;
		vertices1[18 * x + 17] = 0.0f;

		vertices2[18 * x] = 0.0f;
		vertices2[18 * x + 1] = 0.0f;
		vertices2[18 * x + 2] = -0.5f;
		vertices2[18 * x + 3] = 1.0f;
		vertices2[18 * x + 4] = 0.5f;
		vertices2[18 * x + 5] = 0.0f;

		vertices2[18 * x + 6] = vx[x];
		vertices2[18 * x + 7] = vy[x];
		vertices2[18 * x + 8] = -0.5f;
		vertices2[18 * x + 9] = 1.0f;
		vertices2[18 * x + 10] = 0.5f;
		vertices2[18 * x + 11] = 0.0f;

		vertices2[18 * x + 12] = vx[(x + 1) % sides];
		vertices2[18 * x + 13] = vy[(x + 1) % sides];
		vertices2[18 * x + 14] = -0.5f;
		vertices2[18 * x + 15] = 1.0f;
		vertices2[18 * x + 16] = 0.5f;
		vertices2[18 * x + 17] = 0.0f;
	}

	for (int x = 0; x < 18 * sides; x++) {
		vertices[x] = vertices1[x];
	}
	for (int x = 0; x < 18 * sides; x++) {
		vertices[18 * sides + x] = vertices2[x];
	}

	for (int x = 0; x < sides; x++) {
		float a, b, c;
		a = 1.0f * rand() / RAND_MAX;
		b = 1.0f * rand() / RAND_MAX;
		c = 1.0f * rand() / RAND_MAX;

		vertices[36 * sides + 36 * x + 0] = vx[x];
		vertices[36 * sides + 36 * x + 1] = vy[x];
		vertices[36 * sides + 36 * x + 2] = 0.5f;
		vertices[36 * sides + 36 * x + 3] = a;
		vertices[36 * sides + 36 * x + 4] = b;
		vertices[36 * sides + 36 * x + 5] = c;

		vertices[36 * sides + 36 * x + 6] = vx[x];
		vertices[36 * sides + 36 * x + 7] = vy[x];
		vertices[36 * sides + 36 * x + 8] = -0.5f;
		vertices[36 * sides + 36 * x + 9] = a;
		vertices[36 * sides + 36 * x + 10] = b;
		vertices[36 * sides + 36 * x + 11] = c;

		vertices[36 * sides + 36 * x + 12] = vx[(x + 1) % sides];
		vertices[36 * sides + 36 * x + 13] = vy[(x + 1) % sides];
		vertices[36 * sides + 36 * x + 14] = -0.5f;
		vertices[36 * sides + 36 * x + 15] = a;
		vertices[36 * sides + 36 * x + 16] = b;
		vertices[36 * sides + 36 * x + 17] = c;

		vertices[36 * sides + 36 * x + 18] = vx[x];
		vertices[36 * sides + 36 * x + 19] = vy[x];
		vertices[36 * sides + 36 * x + 20] = 0.5f;
		vertices[36 * sides + 36 * x + 21] = a;
		vertices[36 * sides + 36 * x + 22] = b;
		vertices[36 * sides + 36 * x + 23] = c;

		vertices[36 * sides + 36 * x + 24] = vx[(x + 1) % sides];
		vertices[36 * sides + 36 * x + 25] = vy[(x + 1) % sides];
		vertices[36 * sides + 36 * x + 26] = -0.5f;
		vertices[36 * sides + 36 * x + 27] = a;
		vertices[36 * sides + 36 * x + 28] = b;
		vertices[36 * sides + 36 * x + 29] = c;

		vertices[36 * sides + 36 * x + 30] = vx[(x + 1) % sides];
		vertices[36 * sides + 36 * x + 31] = vy[(x + 1) % sides];
		vertices[36 * sides + 36 * x + 32] = 0.5f;
		vertices[36 * sides + 36 * x + 33] = a;
		vertices[36 * sides + 36 * x + 34] = b;
		vertices[36 * sides + 36 * x + 35] = c;
	}

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s),
	// and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered
	// VBO as the vertex attribute's bound vertex buffer object so afterwards we
	// can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally
	// modify this VAO, but this rarely happens. Modifying other VAOs requires a
	// call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
	// VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// enable depth testing for 3d
	glEnable(GL_DEPTH_TEST);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// print stuff for better understanding
		/*
		printf("\n");
		printf("Camera Position: %0.3f %0.3f %0.3f\n", camera.Position.x,
			   camera.Position.y, camera.Position.z);
		*/
		
		// input
		processInput(window);
		glfwSetKeyCallback(window, key_calllback);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// make transformations
		// glm::mat4 transform = glm::mat4(1.0f);  // identity matrix

		ourShader.use();
		if (rotate){
			model =	glm::rotate(model, 0.05f, glm::vec3(1.0f, 0.0f, 0.0f));
		}
			
		ourShader.setMat4("transform", transform);
		ourShader.setMat4("model", model);

		// view matrix
		if(!shifted)
			view = glm ::lookAt(camera_pos, prism_center, camera_up);
		if (turn_table) {
			camera_pos += glm::normalize(glm::cross(prism_center - camera_pos,
													camera_up)) * 0.05f;
		}
		ourShader.setMat4("view", view);

		// projection matrix
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
			0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		glBindVertexArray(VAO);  // seeing as we only have a single VAO there's
		// no need to bind it every time, but we'll do
		// so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 72 * sides);
		// glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse
		// moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float speed = 0.1f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (shifted) {
			shifted = 0;
		}
		camera_pos += speed * camera_front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (shifted) {
			shifted = 0;
		}
		camera_pos -= speed * camera_front;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (shifted) {
			shifted = 0;
		}
		camera_pos -=
			glm ::normalize(glm ::cross(camera_front, camera_up)) * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (shifted) {
			shifted = 0;
		}
		camera_pos +=
			glm ::normalize(glm ::cross(camera_front, camera_up)) * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		if (shifted) {
			shifted = 0;
		}
		camera_pos += camera_up * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		if (shifted) {
			shifted = 0;
		}
		camera_pos -= camera_up * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
	    shifted = 1;
	    transform = glm ::translate(transform, glm ::vec3(0.0f, -speed, 0.0f));
		prism_center += glm::vec3(0.0f, -speed, 0.0f);
    }
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
        shifted = 1;
		transform = glm ::translate(transform, glm ::vec3(-speed, 0.00f, 0.0f));
		prism_center += glm::vec3(-speed, 0.0f, 0.0f);
    }	
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
        shifted = 1;
		transform = glm ::translate(transform, glm ::vec3(speed, 0.0f, 0.0f));
		prism_center += glm::vec3(speed, 0.0f, 0.0f);
    }   
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        shifted = 1;
		transform = glm ::translate(transform, glm ::vec3(0.0f, speed, 0.0f));
		prism_center += glm::vec3(0.0f, speed, 0.0f);
    }
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS){
        shifted = 1;
		transform = glm ::translate(transform, glm ::vec3(0.0f, 0.0f, speed));
		prism_center += glm::vec3(0.0f, 0.0f, speed);
    }
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS){
        shifted = 1;
		transform = glm ::translate(transform, glm ::vec3(0.0f, 0.0f, -speed));
		prism_center += glm::vec3(0.0f, 0.0f, -speed);
    }
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
        shifted = 0;
        // transform = glm ::mat4(1.0f);
		camera_pos = glm::vec3(2.0f, 3.0f, 4.0f);
    }
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS){
        shifted = 0;
        // transform = glm ::mat4(1.0f);
		camera_pos = glm::vec3(-2.0f, 3.0f, -4.0f);
    }
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        if (shifted) {
			shifted = 0;
		}
        
	    // camera_pos += glm::normalize(glm::cross(prism_center - camera_pos,
	    // camera_up)) * speed;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width
	// and height will be significantly larger than specified on retina
	// displays.
	glViewport(0, 0, width, height);
}

void key_calllback(GLFWwindow *window, int key, int scancode, int action,
				   int mods) {
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		rotate ^= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		turn_table ^= 1;
	}
}
