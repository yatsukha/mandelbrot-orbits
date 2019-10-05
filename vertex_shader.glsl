#version 330 core

layout (location = 0) in vec3 pos;

out vec2 tex_coordinate;

void main() {
	gl_Position = vec4(pos, 1.0);
	tex_coordinate = pos.xy;
}