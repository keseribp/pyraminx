#version 330 core

layout(location = 0) in vec4 vertexIn;
layout(location = 1) in vec4 centerIn;
layout(location = 2) in vec4 colorIn;

out vec4 colorToFragShader;

uniform mat4 MVP;

void main() {
    vec4 _vertexIn;
    _vertexIn.x = vertexIn.x + centerIn.x;
    _vertexIn.z = vertexIn.z + centerIn.z;
    _vertexIn.w = 1.0;
    
    gl_Position = MVP * _vertexIn;
    colorToFragShader = colorIn;
}
