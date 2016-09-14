#include "puzzle.h"

void rotX(float* v, uint i, float theta) {
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);
    float y = v[i + 1];
    float z = v[i + 2];
    v[i + 1] = y * cosTheta - z * sinTheta;
    v[i + 2] = y * sinTheta + z * cosTheta;
}

void rotY(float* v, uint i, float theta) {
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);
    float x = v[i];
    float z = v[i + 2];
    v[i] = x * cosTheta + z * sinTheta;
    v[i + 2] = -x * sinTheta + z * cosTheta;
}

void rotZ(float* v, uint i, float theta) {
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);
    float x = v[i];
    float y = v[i + 1];
    v[i] = x * cosTheta - y * sinTheta;
    v[i + 1] = x * sinTheta + y * cosTheta;
}

Puzzle::Puzzle(GLFWwindow* windowPtr) {
    window = windowPtr;
    m_timer = 0;
    m_timerRefresh = 80; // tunable
    uint numFacesOneSide = 9;
    uint numSides = 4;
    m_numFaceVertices = 3;
    uint dim = 4;
    float a = INV_SQRT_3;
    float b = INV_SQRT_3_OVER_2;
    float c = a + b;
    m_numFaces = numSides * numFacesOneSide;
    uint dataLength = m_numFaces * m_numFaceVertices * dim;
    
    float faceVertices[] = {
        0,b,0,1, -1,-a,0,1, 1,-a,0,1
    };
    m_faceVertices = new float[dataLength];
    for (uint i = 0; i < dataLength; i += (m_numFaceVertices * dim)) {
        m_faceVertices[i] = faceVertices[0];
        m_faceVertices[i + 1] = faceVertices[1];
        m_faceVertices[i + 2] = faceVertices[2];
        m_faceVertices[i + 3] = faceVertices[3];
        
        m_faceVertices[i + 4] = faceVertices[4];
        m_faceVertices[i + 5] = faceVertices[5];
        m_faceVertices[i + 6] = faceVertices[6];
        m_faceVertices[i + 7] = faceVertices[7];
        
        m_faceVertices[i + 8] = faceVertices[8];
        m_faceVertices[i + 9] = faceVertices[9];
        m_faceVertices[i + 10] = faceVertices[10];
        m_faceVertices[i + 11] = faceVertices[11];
    }
    
    float faceCentersBottom4[] = {
        0,c+a,0,1, 0,c+a,0,1, 0,c+a,0,1,
        -1,a,0,1, -1,a,0,1, -1,a,0,1,
        0,b,0,-1, 0,b,0,-1, 0,b,0,-1,
        1,a,0,1, 1,a,0,1, 1,a,0,1,
        -2,-b,0,1, -2,-b,0,1, -2,-b,0,1,
        -1,-a,0,-1, -1,-a,0,-1, -1,-a,0,-1, 
        0,-b,0,1, 0,-b,0,1, 0,-b,0,1, 
        1,-a,0,-1, 1,-a,0,-1, 1,-a,0,-1, 
        2,-b,0,1, 2,-b,0,1, 2,-b,0,1,
        
        0,c+a,0,1, 0,c+a,0,1, 0,c+a,0,1,
        -1,a,0,1, -1,a,0,1, -1,a,0,1,
        0,b,0,-1, 0,b,0,-1, 0,b,0,-1,
        1,a,0,1, 1,a,0,1, 1,a,0,1,
        -2,-b,0,1, -2,-b,0,1, -2,-b,0,1,
        -1,-a,0,-1, -1,-a,0,-1, -1,-a,0,-1, 
        0,-b,0,1, 0,-b,0,1, 0,-b,0,1, 
        1,-a,0,-1, 1,-a,0,-1, 1,-a,0,-1, 
        2,-b,0,1, 2,-b,0,1, 2,-b,0,1,
        
        0,c+a,0,1, 0,c+a,0,1, 0,c+a,0,1,
        -1,a,0,1, -1,a,0,1, -1,a,0,1,
        0,b,0,-1, 0,b,0,-1, 0,b,0,-1,
        1,a,0,1, 1,a,0,1, 1,a,0,1,
        -2,-b,0,1, -2,-b,0,1, -2,-b,0,1,
        -1,-a,0,-1, -1,-a,0,-1, -1,-a,0,-1, 
        0,-b,0,1, 0,-b,0,1, 0,-b,0,1, 
        1,-a,0,-1, 1,-a,0,-1, 1,-a,0,-1, 
        2,-b,0,1, 2,-b,0,1, 2,-b,0,1,
        
        0,c+a,0,1, 0,c+a,0,1, 0,c+a,0,1,
        -1,a,0,1, -1,a,0,1, -1,a,0,1,
        0,b,0,-1, 0,b,0,-1, 0,b,0,-1,
        1,a,0,1, 1,a,0,1, 1,a,0,1,
        -2,-b,0,1, -2,-b,0,1, -2,-b,0,1,
        -1,-a,0,-1, -1,-a,0,-1, -1,-a,0,-1, 
        0,-b,0,1, 0,-b,0,1, 0,-b,0,1, 
        1,-a,0,-1, 1,-a,0,-1, 1,-a,0,-1, 
        2,-b,0,1, 2,-b,0,1, 2,-b,0,1
    };
    m_faceCenters = new float[dataLength];
    float tmpVec[] = {0.0f, 0.0f, 0.0f, 0.0f};
    float theta = acos(1.0f / 3.0f);
    float sinTheta = sin(theta);
    float translationVector[] = {
    	0.0f, 
        -(float) sqrt(abs(6.0f * c * c - 9.0f * c * c * sinTheta * sinTheta)),
        3.0f * c * sinTheta - 1.5f,
        0.0f
    };
    
    for (uint i = 0; i < 1 * dim * numFacesOneSide * m_numFaceVertices; i += dim) { //TODO why is this m_numFaces here?
        m_faceCenters[i] = faceCentersBottom4[i];
        m_faceCenters[i + 1] = faceCentersBottom4[i + 1];
        m_faceCenters[i + 2] = faceCentersBottom4[i + 2];
        m_faceCenters[i + 3] = faceCentersBottom4[i + 3];
        
        if (m_faceCenters[i + 3] == -1.0f)
            m_faceVertices[i + 1] = -m_faceVertices[i + 1];
    }
    for (
    	uint i = 1 * dim * numFacesOneSide * m_numFaceVertices; 
    	i < 2 * dim * numFacesOneSide * m_numFaceVertices; 
    	i += di
    ) {
        tmpVec[0] = faceCentersBottom4[i];
        tmpVec[1] = faceCentersBottom4[i + 1];
        tmpVec[2] = faceCentersBottom4[i + 2];
        tmpVec[3] = faceCentersBottom4[i + 3];
        rotX(tmpVec, 0, theta);
        m_faceCenters[i] = tmpVec[0];
        m_faceCenters[i + 1] = tmpVec[1] + translationVector[1];
        m_faceCenters[i + 2] = tmpVec[2] + translationVector[2];
        m_faceCenters[i + 3] = faceCentersBottom4[i + 3];
        
        if (m_faceCenters[i + 3] == -1.0f)
            m_faceVertices[i + 1] = -m_faceVertices[i + 1];
        
        tmpVec[0] = m_faceVertices[i];
        tmpVec[1] = m_faceVertices[i + 1];
        tmpVec[2] = m_faceVertices[i + 2];
        tmpVec[3] = m_faceVertices[i + 3];
        rotX(tmpVec, 0, theta);
        m_faceVertices[i] = tmpVec[0];
        m_faceVertices[i + 1] = tmpVec[1];
        m_faceVertices[i + 2] = tmpVec[2];
        m_faceVertices[i + 3] = m_faceCenters[i + 3];
    }
    for (
    	uint i = 2 * dim * numFacesOneSide * m_numFaceVertices; 
    	i < 3 * dim * numFacesOneSide * m_numFaceVertices; 
    	i += dim
    ) {
        tmpVec[0] = faceCentersBottom4[i];
        tmpVec[1] = faceCentersBottom4[i + 1];
        tmpVec[2] = faceCentersBottom4[i + 2];
        tmpVec[3] = faceCentersBottom4[i + 3];
        rotX(tmpVec, 0, theta);
        tmpVec[1] += translationVector[1];
        tmpVec[2] += translationVector[2];
        rotZ(tmpVec, 0, 2 * PI / 3);
        
        m_faceCenters[i] = tmpVec[0];
        m_faceCenters[i + 1] = tmpVec[1];
        m_faceCenters[i + 2] = tmpVec[2];
        m_faceCenters[i + 3] = faceCentersBottom4[i + 3];
        
        if (m_faceCenters[i + 3] == -1.0f)
            m_faceVertices[i + 1] = -m_faceVertices[i + 1];
        
        tmpVec[0] = m_faceVertices[i];
        tmpVec[1] = m_faceVertices[i + 1];
        tmpVec[2] = m_faceVertices[i + 2];
        tmpVec[3] = m_faceVertices[i + 3];
        rotX(tmpVec, 0, theta);
        rotZ(tmpVec, 0, 2 * PI / 3);
        
        m_faceVertices[i] = tmpVec[0];
        m_faceVertices[i + 1] = tmpVec[1];
        m_faceVertices[i + 2] = tmpVec[2];
        m_faceVertices[i + 3] = m_faceCenters[i + 3];
    }
    for (
    	uint i = 3 * dim * numFacesOneSide * m_numFaceVertices; 
        i < dataLength; 
        i += dim
    ) {
        tmpVec[0] = faceCentersBottom4[i];
        tmpVec[1] = faceCentersBottom4[i + 1];
        tmpVec[2] = faceCentersBottom4[i + 2];
        tmpVec[3] = faceCentersBottom4[i + 3];
        rotX(tmpVec, 0, theta);
        tmpVec[1] += translationVector[1];
        tmpVec[2] += translationVector[2];
        rotZ(tmpVec, 0, 4 * PI / 3);
        
        m_faceCenters[i] = tmpVec[0];
        m_faceCenters[i + 1] = tmpVec[1];
        m_faceCenters[i + 2] = tmpVec[2];
        m_faceCenters[i + 3] = faceCentersBottom4[i + 3];
        
        if (m_faceCenters[i + 3] == -1.0f)
            m_faceVertices[i + 1] = -m_faceVertices[i + 1];
        
        tmpVec[0] = m_faceVertices[i];
        tmpVec[1] = m_faceVertices[i + 1];
        tmpVec[2] = m_faceVertices[i + 2];
        tmpVec[3] = m_faceVertices[i + 3];
        rotX(tmpVec, 0, theta);
        rotZ(tmpVec, 0, 4 * PI / 3);
        
        m_faceVertices[i] = tmpVec[0];
        m_faceVertices[i + 1] = tmpVec[1];
        m_faceVertices[i + 2] = tmpVec[2];
        m_faceVertices[i + 3] = m_faceCenters[i + 3];    
    }
    
    GLubyte side0[] = {0, 117, 250, 255}; //TODO this could be handled better
    GLubyte side1[] = {250, 0, 242, 255};
    GLubyte side2[] = {250, 133, 0, 255};
    GLubyte side3[] = {0, 250, 8, 255};
    
    GLubyte sideColors[] = {
        side0[0],side0[1],side0[2],side0[3],
        
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3],
        
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        
        side0[0],side0[1],side0[2],side0[3],
        
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3],
        
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        
        side0[0],side0[1],side0[2],side0[3],
        
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3],
        
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3], 
        side0[0],side0[1],side0[2],side0[3],  
        
        
        side1[0],side1[1],side1[2],side1[3],
        
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3],
        
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        
        side1[0],side1[1],side1[2],side1[3],
        
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3],
        
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        
        side1[0],side1[1],side1[2],side1[3],
        
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3],
        
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        side1[0],side1[1],side1[2],side1[3], 
        
        
        side2[0],side2[1],side2[2],side2[3],
        
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3],
        
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3],
        
        side2[0],side2[1],side2[2],side2[3],
        
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3],
        
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3],
        
        side2[0],side2[1],side2[2],side2[3],
        
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3],
        
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3], 
        side2[0],side2[1],side2[2],side2[3],
        
        
        side3[0],side3[1],side3[2],side3[3],
        
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3],
        
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3],
        
        side3[0],side3[1],side3[2],side3[3],
        
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3],
        
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3],
        
        side3[0],side3[1],side3[2],side3[3],
        
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3],
        
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3], 
        side3[0],side3[1],side3[2],side3[3]
    };
    m_faceColors = new GLubyte[dataLength];
    for (uint i = 0; i < dataLength; i++)
        m_faceColors[i] = sideColors[i];
}

Puzzle::~Puzzle() {
    delete[] m_faceVertices;
    delete[] m_faceColors;
    delete[] m_faceCenters;
}

uint Puzzle::getNumFaceVertices() {
    return m_numFaceVertices;
}

uint Puzzle::getNumFaces() {
    return m_numFaces;
}

float* Puzzle::getFaceVertices() {
    return m_faceVertices;
}

float* Puzzle::getFaceCenters() {
    return m_faceCenters;
}

GLubyte* Puzzle::getFaceColors() {
    return m_faceColors;
}

// B = 0
// M = 1
// O = 2
// G = 3
void Puzzle::_blueTransform() { // TODO these transforms seem more generalizable
    GLubyte old[4 * 3 * 3];
    uint A = 4 * 9 * 3; // 4 floats per vertex, 9 faces per side, 3 vertices per face
    uint B = 4 * 3; // 4 floats per vertex, 3 vertices per face
    uint a, b, aa, bb;
    
    // m1 <- M1
    a = 1;
    b = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 0 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // m2 <- M2
    a = 1;
    b = 2;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 1 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // m3 <- M3
    a = 1;
    b = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 2 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // M1 <- G1
    a = 3;
    b = 1;
    aa = 1;
    bb = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // M2 <- G2
    a = 3;
    b = 2;
    aa = 1;
    bb = 2;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // M3 <- G3
    a = 3;
    b = 3;
    aa = 1;
    bb = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // G1 <- O1
    a = 2;
    b = 1;
    aa = 3;
    bb = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // G2 <- O2
    a = 2;
    b = 2;
    aa = 3;
    bb = 2;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // G3 <- O3
    a = 2;
    b = 3;
    aa = 3;
    bb = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // O1 <- m1
    aa = 2;
    bb = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 0 + i]; // a = side index, b = face index
    // O2 <- m2
    aa = 2;
    bb = 2;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 1 + i]; // a = side index, b = face index
    // O3 <- m3
    aa = 2;
    bb = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 2 + i]; // a = side index, b = face index
}

void Puzzle::_magentaTransform() {
    GLubyte old[4 * 3 * 3];
    uint A = 4 * 9 * 3; // 4 floats per vertex, 9 faces per side, 3 vertices per face
    uint B = 4 * 3; // 4 floats per vertex, 3 vertices per face
    uint a, b, aa, bb;
    
    // b1 <- B1
    a = 0;
    b = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 0 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // b2 <- B2
    a = 0;
    b = 2;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 1 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // b3 <- B3
    a = 0;
    b = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 2 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // B1 <- O6
    a = 2;
    b = 6;
    aa = 0;
    bb = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // B2 <- O7
    a = 2;
    b = 7;
    aa = 0;
    bb = 2;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // B3 <- O3
    a = 2;
    b = 3;
    aa = 0;
    bb = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // O6 <- G1
    a = 3;
    b = 1;
    aa = 2;
    bb = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // O7 <- G5
    a = 3;
    b = 5;
    aa = 2;
    bb = 7;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // O3 <- G6
    a = 3;
    b = 6;
    aa = 2;
    bb = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // G1 <- b1
    aa = 3;
    bb = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 0 + i]; // a = side index, b = face index
    // G5 <- b2
    aa = 3;
    bb = 5;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 1 + i]; // a = side index, b = face index
    // G6 <- b3
    aa = 3;
    bb = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 2 + i]; // a = side index, b = face index
}

void Puzzle::_orangeTransform() {
    GLubyte old[4 * 3 * 3];
    uint A = 4 * 9 * 3; // 4 floats per vertex, 9 faces per side, 3 vertices per face
    uint B = 4 * 3; // 4 floats per vertex, 3 vertices per face
    uint a, b, aa, bb;
    
    // b1 <- B1
    a = 0;
    b = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 0 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // b5 <- B5
    a = 0;
    b = 5;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 1 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // b6 <- B6
    a = 0;
    b = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 2 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // B1 <- G3
    a = 3;
    b = 3;
    aa = 0;
    bb = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // B5 <- G7
    a = 3;
    b = 7;
    aa = 0;
    bb = 5;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // B6 <- G6
    a = 3;
    b = 6;
    aa = 0;
    bb = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // G3 <- M6
    a = 1;
    b = 6;
    aa = 3;
    bb = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // G7 <- M5
    a = 1;
    b = 5;
    aa = 3;
    bb = 7;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // G6 <- M1
    a = 1;
    b = 1;
    aa = 3;
    bb = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // M6 <- b1
    aa = 1;
    bb = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 0 + i]; // a = side index, b = face index
    // M5 <- b5
    aa = 1;
    bb = 5;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 1 + i]; // a = side index, b = face index
    // M1 <- b6
    aa = 1;
    bb = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 2 + i]; // a = side index, b = face index
}

void Puzzle::_greenTransform() {
    GLubyte old[4 * 3 * 3];
    uint A = 4 * 9 * 3; // 4 floats per vertex, 9 faces per side, 3 vertices per face
    uint B = 4 * 3; // 4 floats per vertex, 3 vertices per face
    uint a, b, aa, bb;
    
    // b6 <- B6
    a = 0;
    b = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 0 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // b7 <- B7
    a = 0;
    b = 7;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 1 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // b3 <- B3
    a = 0;
    b = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 2 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // B6 <- M3
    a = 1;
    b = 3;
    aa = 0;
    bb = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // B7 <- M7
    a = 1;
    b = 7;
    aa = 0;
    bb = 7;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // B3 <- M6
    a = 1;
    b = 6;
    aa = 0;
    bb = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // M3 <- O6
    a = 2;
    b = 6;
    aa = 1;
    bb = 3;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // M7 <- O5
    a = 2;
    b = 5;
    aa = 1;
    bb = 7;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // M6 <- O1
    a = 2;
    b = 1;
    aa = 1;
    bb = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    
    // O6 <- b6
    aa = 2;
    bb = 6;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 0 + i]; // a = side index, b = face index
    // O5 <- b7
    aa = 2;
    bb = 5;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 1 + i]; // a = side index, b = face index
    // O1 <- b3
    aa = 2;
    bb = 1;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 2 + i]; // a = side index, b = face index
}

void Puzzle::_blueFreeTransform() {
    GLubyte old[4 * 3];
    uint A = 4 * 9 * 3; // 4 floats per vertex, 9 faces per side, 3 vertices per face
    uint B = 4 * 3; // 4 floats per vertex, 3 vertices per face
    uint a, b, aa, bb;
    
    // m0 <- M0
    a = 1;
    b = 0;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 0 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // M0 <- G0
    a = 3;
    b = 0;
    aa = 1;
    bb = 0;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // G0 <- O0
    a = 2;
    b = 0;
    aa = 3;
    bb = 0;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // O0 <- m0
    aa = 2;
    bb = 0;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 0 + i]; // a = side index, b = face index
}

void Puzzle::_magentaFreeTransform() {
    GLubyte old[4 * 3];
    uint A = 4 * 9 * 3; // 4 floats per vertex, 9 faces per side, 3 vertices per face
    uint B = 4 * 3; // 4 floats per vertex, 3 vertices per face
    uint a, b, aa, bb;
    
    // o8 <- O8
    a = 2;
    b = 8;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 0 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // O8 <- G4
    a = 3;
    b = 4;
    aa = 2;
    bb = 8;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // G4 <- B0
    a = 0;
    b = 0;
    aa = 3;
    bb = 4;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // B0 <- o8
    aa = 0;
    bb = 0;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 0 + i]; // a = side index, b = face index
}

void Puzzle::_orangeFreeTransform() {
    GLubyte old[4 * 3];
    uint A = 4 * 9 * 3; // 4 floats per vertex, 9 faces per side, 3 vertices per face
    uint B = 4 * 3; // 4 floats per vertex, 3 vertices per face
    uint a, b, aa, bb;
    
    // g8 <- G8
    a = 3;
    b = 8;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 0 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // G8 <- M4
    a = 1;
    b = 4;
    aa = 3;
    bb = 8;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // M4 <- B4
    a = 0;
    b = 4;
    aa = 1;
    bb = 4;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // B4 <- g8
    aa = 0;
    bb = 4;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 0 + i]; // a = side index, b = face index
}

void Puzzle::_greenFreeTransform() {
    GLubyte old[4 * 3];
    uint A = 4 * 9 * 3; // 4 floats per vertex, 9 faces per side, 3 vertices per face
    uint B = 4 * 3; // 4 floats per vertex, 3 vertices per face
    uint a, b, aa, bb;
    
    // m8 <- M8
    a = 1;
    b = 8;
    for (uint i = 0; i < 3 * 4; i ++)
        old[4 * 3 * 0 + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // M8 <- O4
    a = 2;
    b = 4;
    aa = 1;
    bb = 8;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // O4 <- B8
    a = 0;
    b = 8;
    aa = 2;
    bb = 4;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = m_faceColors[A * a + B * b + i]; // a = side index, b = face index
    // B8 <- m8
    aa = 0;
    bb = 8;
    for (uint i = 0; i < 3 * 4; i ++)
        m_faceColors[A * aa + B * bb + i] = old[4 * 3 * 0 + i]; // a = side index, b = face index
}

void Puzzle::update() {
    //if (DEBUG) printf("m_timer = %i\n", m_timer);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && m_timer == 0){ // shuffle puzzle
        m_timer = m_timerRefresh;
        _shuffle();
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && m_timer == 0){ // solve puzzle
        m_timer = m_timerRefresh;
        _solve();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && m_timer == 0){ // blue transform
        if (DEBUG) printf("a key pressed\n");
        m_timer = m_timerRefresh;
        _blueTransform();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && m_timer == 0){ // magenta transform
        if (DEBUG) printf("S key pressed\n");
        m_timer = m_timerRefresh;
        _magentaTransform();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && m_timer == 0){ // orange transform
        if (DEBUG) printf("d key pressed\n");
        m_timer = m_timerRefresh;
        _orangeTransform();
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && m_timer == 0){ // green transform
        if (DEBUG) printf("f key pressed\n");
        m_timer = m_timerRefresh;
        _greenTransform();
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && m_timer == 0){ // blue free transform
        if (DEBUG) printf("q key pressed\n");
        m_timer = m_timerRefresh;
        _blueFreeTransform();
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && m_timer == 0){ // magenta free transform
        if (DEBUG) printf("w key pressed\n");
        m_timer = m_timerRefresh;
        _magentaFreeTransform();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && m_timer == 0){ // orange free transform
        if (DEBUG) printf("e key pressed\n");
        m_timer = m_timerRefresh;
        _orangeFreeTransform();
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && m_timer == 0){ // green free transform
        if (DEBUG) printf("r key pressed\n");
        m_timer = m_timerRefresh;
        _greenFreeTransform();
	}
	if (m_timer > 0) m_timer--;
}

void Puzzle::_shuffle() {
    // generate a random N uint to determine the length of the sequence, within a range
    const uint N = rand() % 400 + 100; // tunable max number of transforms 
    if (DEBUG) printf("suffle sequence length is %i\n", N);
    // generate N random ints from rante R to give a sequence S of transforms
    uint S;
    for (uint i = 0; i < N; i++) {
        S = rand() % 16;
        
        // execute each transformation in sequence S
        switch (S) {
            default:
            case 0: _blueTransform();
            case 1: _blueTransform();
                break;
            case 2: _magentaTransform();
            case 3: _magentaTransform();
                break;
            case 4: _orangeTransform();
            case 5: _orangeTransform();
                break;
            case 6: _greenTransform();
            case 7: _greenTransform();
                break;
            case 8: _blueFreeTransform();
            case 9: _blueFreeTransform();
                break;
            case 10: _magentaFreeTransform();
            case 11: _magentaFreeTransform();
                break;
            case 12: _orangeFreeTransform();
            case 13: _orangeFreeTransform();
                break;
            case 14: _greenFreeTransform();
            case 15: _greenFreeTransform();
                break;
        }   
    }
}

void Puzzle::_solve() {
    // TODO
}
