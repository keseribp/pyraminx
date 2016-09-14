#ifndef COMMON_H
#define COMMON_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cuda_runtime.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "vector_types.h"

const /*static*/ float INV_SQRT_3 = 1.0f / sqrt(3.0f);
const /*static*/ float INV_SQRT_3_OVER_2 = 2.0f * INV_SQRT_3;
const /*static*/ float PI = 3.1415926;
const /*static*/ bool DEBUG = false;

#endif
