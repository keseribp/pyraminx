#ifndef PUZZLE_H
#define PUZZLE_H

#include "common.h"

class Puzzle {
    private:
        uint m_numFaces;
        uint m_numFaceVertices;
        float* m_faceVertices;
        float* m_faceCenters;
        GLubyte* m_faceColors;
        uint m_timer;
        uint m_timerRefresh;
        GLFWwindow* window;
    private:
        void _blueTransform();
        void _magentaTransform();
        void _orangeTransform();
        void _greenTransform();
        void _blueFreeTransform();
        void _magentaFreeTransform();
        void _orangeFreeTransform();
        void _greenFreeTransform();
        void _shuffle();
        void _solve();
    public:
        Puzzle(GLFWwindow* window);
        ~Puzzle();
        void update();
        uint getNumFaceVertices();
        uint getNumFaces();
        float* getFaceVertices();
        float* getFaceCenters();
        GLubyte* getFaceColors();
};

#endif
