#version 330 core

in vec2 tex_coordinate;

out vec4 color;

uniform sampler2D tex;

void main() {
	color = texture(tex, (tex_coordinate + 1.0) / 2.0);
}