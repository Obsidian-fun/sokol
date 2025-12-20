@ctype mat4 HMM_Mat4

@vs vs
in vec3 position;

layout(binding=0) uniform vs_params {
	mat4 model;
	mat4 view;
	mat4 projection;
};

void main() {
	gl_Position = projection * view * model *vec4(position, 1.0);
}
@end

@fs fs
out vec4 fragmentColor;

void main() {
	fragmentColor = vec4(1.0, 0.5f, 2.5f, 1.0f);
}
@end
@program simple vs fs 

