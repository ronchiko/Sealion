#version 450

in vec2 position;
in vec4 _color;
in mat3 transform;

uniform mat3 _translation;

out vec4 color;

void main() {

	vec3 offset = _translation * transform * vec3(position.xy, 1.0);

	gl_Position = vec4(offset.xy, 0.0, 1.0);

	color = _color;
}