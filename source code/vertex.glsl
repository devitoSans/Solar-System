#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texCoord;

out vec3 normal;
out vec2 texCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main() {
	normal = mat3(transpose(inverse(model))) * vertex_normal;
	texCoord = vertex_texCoord;
	fragPos = vec3(model * vec4(vertex_position, 1.f));

	gl_Position = projection * camera * model * vec4(vertex_position, 1.f);
}
