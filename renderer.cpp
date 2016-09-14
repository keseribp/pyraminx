#include "renderer.h"

Renderer::Renderer(
    uint numFaceVertices, 
    uint numCenters, 
    float* faceVertices, 
    float* centers, 
    GLubyte* colors, 
    Camera* camera) 
{
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
    
    m_programID = loadProgram("faceVertexShader.glsl", "faceFragmentShader.glsl");
    
    m_MVP = camera->getMVP();
    m_matrixID = glGetUniformLocation(m_programID, "MVP");
    
    m_numFaceVertices = numFaceVertices;
    m_numCenters = numCenters;
    m_numVertices = m_numFaceVertices * m_numCenters;
    
    m_vertexData = faceVertices;
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * 4 * sizeof(float), m_vertexData, GL_STATIC_DRAW);
    
    m_centerData = centers;
    glGenBuffers(1, &m_centerBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_centerBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * 4 * sizeof(float), m_centerData, GL_STATIC_DRAW);
    
    m_colorData = colors;
    glGenBuffers(1, &m_colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * 4 * sizeof(GLubyte), m_colorData, GL_STREAM_DRAW);
}

Renderer::~Renderer() {
    //delete[] m_vertexData;
    //delete[] m_centerData;
    //delete[] m_colorData;
    glDeleteBuffers(1, &m_colorBuffer);
    glDeleteBuffers(1, &m_centerBuffer);
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteProgram(m_programID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void Renderer::display() {
    glUseProgram(m_programID);

    glUniformMatrix4fv(m_matrixID, 1, GL_FALSE, &(*m_MVP)[0][0]);

    // color transofrms
    glGenBuffers(1, &m_colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * 4 * sizeof(GLubyte), m_colorData, GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        4,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_centerBuffer);
    glVertexAttribPointer(
        1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
        4,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glVertexAttribPointer(
        2,                  // attribute 2. No particular reason for 2, but must match the layout in the shader.
        4,                  // size
        GL_UNSIGNED_BYTE,   // type
        GL_TRUE,            // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices * 4);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

GLuint Renderer::loadProgram(const char * shaderPath, const char * fragPath) {
    // Create the shaders
	uint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	uint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file.
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(shaderPath, std::ios::in);
	if (vertexShaderStream.is_open()) {
		std::string line = "";
		while (getline(vertexShaderStream, line))
			vertexShaderCode += "\n" + line;
		vertexShaderStream.close();
	} else {
		printf("Failed to open %s.\n", shaderPath);
		return 0;
	}

	// Read the Fragment Shader code from the file.
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragPath, std::ios::in);
	if (fragmentShaderStream.is_open()) {
		std::string line = "";
		while (getline(fragmentShaderStream, line))
			fragmentShaderCode += "\n" + line;
		fragmentShaderStream.close();
	} else {
	    printf("Failed to open %s.\n", fragPath);
	    return 0;
	}

	int result = GL_FALSE;
	int infoLogLength; //TODO uint instead?

	// Compile Vertex Shader.
	if (DEBUG) printf("Compiling shader : %s\n", shaderPath);
	char const* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader.
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader.
	if (DEBUG) printf("Compiling shader : %s\n", fragPath);
	char const* fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader.
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	// Link the program.
	if (DEBUG) printf("Linking program\n");
	GLuint programID;
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program.
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	
	return programID;
}
