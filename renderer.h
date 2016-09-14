#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"

class Renderer {
    private:
        GLuint m_programID;
        GLuint m_matrixID;
        GLuint m_vertexArrayID;
        GLuint m_vertexBuffer;
        GLuint m_centerBuffer;
        GLuint m_colorBuffer;
        float* m_vertexData;
        float* m_centerData;
        GLubyte* m_colorData;
        uint m_numFaceVertices;
        uint m_numCenters;
        uint m_numVertices;
        glm::mat4* m_MVP;
    private:
        GLuint loadProgram(const char * shaderPath, const char * fragPath);
    public:
        Renderer(uint numFaceVertices,
                 uint numCenters,
                 float* faceVertices,
                 float* centers,
                 GLubyte* colors,
                 Camera* camera);
        ~Renderer();
        void display();
};

#endif
