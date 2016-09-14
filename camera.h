#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class Camera {
    private:
        glm::vec3 m_position;
        float m_horizontalAngle;
        float m_verticalAngle;
        float m_initialFoV;
        float m_speed;
        float m_mouseSpeed;
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_modelMatrix;
        glm::mat4 m_MVP;
        GLFWwindow* window;
    public:
        Camera(GLFWwindow* window);
        ~Camera();
        void update();
        glm::mat4* getMVP();
};

#endif
