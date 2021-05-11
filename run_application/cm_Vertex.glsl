#version 330 core

layout (location = 0) in vec3 vertex_position;

out vec3 texCoord;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main() {
	texCoord = vertex_position;
	vec4 position = projection * camera * model * vec4(vertex_position, 1.f);

	gl_Position = position.xyww;
}
