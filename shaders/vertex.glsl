#version 450

in vec2 position;
in vec2 scale;
in vec4 _color;

uniform mat2 _translation;

out vec4 color;

void main() {

	vec2 offset = _translation * (position.xy + scale.xy);

	gl_Position = vec4(offset.xy, 0.0, 1.0);

	color = _color;
}