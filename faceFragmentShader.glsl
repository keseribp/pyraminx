#version 330 core

in vec4 colorToFragShader;

out vec4 color;

void main() {
	color = colorToFragShader;
}
