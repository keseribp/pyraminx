#include "camera.h"

Camera::Camera(GLFWwindow* windowPtr) {
    window = windowPtr;
    m_position;
    m_horizontalAngle = 0;
    m_verticalAngle = - PI / 2.0f;
    m_initialFoV = 45.0f;
    m_speed = 3.0f;
    m_mouseSpeed = 0.005f;
    m_modelMatrix = glm::mat4(1.0);
}

Camera::~Camera() {}

void Camera::update() {
    // glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		m_verticalAngle -= deltaTime * m_speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		m_verticalAngle += deltaTime * m_speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		m_horizontalAngle += deltaTime * m_speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		m_horizontalAngle -= deltaTime * m_speed;
	}
	
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		sin(m_verticalAngle) * cos(m_horizontalAngle), 
		sin(m_verticalAngle) * sin(m_horizontalAngle),
		cos(m_verticalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		cos(m_horizontalAngle + PI / 2),	
		sin(m_horizontalAngle + PI / 2),
		0
	);

    // Up vector
	glm::vec3 up = glm::cross(right, direction);
	
	float distance = 20.0f;
	m_position = -distance * direction + glm::vec3(0.0f, 0.0f, 2.5f); //small boos off ground
	
	float FoV = m_initialFoV;// - 5 * glfwGetMouseWheel();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	m_projectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	m_viewMatrix = glm::lookAt(m_position,              // Camera is here
				   m_position + direction,  // and looks here : at the same position, plus "direction"
				   up);                     // Head is up (set to 0,-1,0 to look upside-down)

	m_MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

glm::mat4* Camera::getMVP() {
    return &m_MVP;
}
