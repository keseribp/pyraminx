/*
TODO:
    - rendered triangular pyramid with black lines and colored triangular faces
    - minimize includes in other headers
*/
/*
Build:

$ nvcc -std=c++11 main.cpp -o rubikPuzzle -lglfw3 -lGLU -lGL -lGLEW -lX11 -lXxf86vm -lXinerama -lXcursor -lrt -lm -lXrandr -lpthread -lXi -ldl

*/

#include "camera.h"
#include "puzzle.h"
#include "renderer.h"

typedef unsigned int uint;

const /*static*/ uint WINDOW_X = 2 * 1024;
const /*static*/ uint WINDOW_Y = 2 * 768;
const /*static*/ uint WINDOW_CENTER_X = WINDOW_X / 2;
const /*static*/ uint WINDOW_CENTER_Y = WINDOW_Y / 2;
const /*static*/ float CLEAR_COLOR_R = 0.2f;
const /*static*/ float CLEAR_COLOR_B = 0.2f;
const /*static*/ float CLEAR_COLOR_G = 0.2f;

GLFWwindow* window;

int main(void) {
    if (DEBUG) printf("main; just entered\n");
    if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW.\n");
		return -1;
	}

    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_X, WINDOW_Y, "Triangular Pyramid Puzzle", NULL, NULL);
	if(window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
    
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW.\n");
		glfwTerminate();
		return -1;
	}
    if (DEBUG) printf("main; glew init\n");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwPollEvents();
    glfwSetCursorPos(window, WINDOW_CENTER_X, WINDOW_CENTER_Y);

	glClearColor(CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_G, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);    
    if (DEBUG) printf("main; before puzzle\n");
    Puzzle puzzle(window);
    if (DEBUG) printf("main; before camera\n");
    Camera camera(window);
    if (DEBUG) printf("main; before renderer\n");
    Renderer renderer(
        puzzle.getNumFaceVertices(),
        puzzle.getNumFaces(),
        puzzle.getFaceVertices(),
        puzzle.getFaceCenters(),
        puzzle.getFaceColors(),
        &camera
    );
    if (DEBUG) printf("main; before do loop\n");
    
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		puzzle.update();
	    
	    camera.update(); 
	    
		renderer.display();
		
		glfwSwapBuffers(window);
		
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		     glfwWindowShouldClose(window) == 0);
    
	glfwTerminate();

	return 0;
}

