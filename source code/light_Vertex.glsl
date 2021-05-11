#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec2 vertex_texCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main() {
	texCoord = vertex_texCoord;

	gl_Position = projection * camera * model * vec4(vertex_position, 1.f);
}
