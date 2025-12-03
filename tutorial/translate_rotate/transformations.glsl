@ctype mat4 HMM_Mat4 

@vs vs
in vec3 position;

layout (binding=0) uniform vs_params {
	mat4 transform;
};

void main() {
	gl_Position = transform * vec4(position, 1.0);
}
@end

@fs fs
out vec4 FragColor;

void main() {
	FragColor = vec4(1.0f, 0.0f, 0.0f, 0.5f);
}
@end

@program simple vs fs



