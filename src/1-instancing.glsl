@ctype vec HMM_Vec4
@vs vs

in vec2 position;
layout (binding=0) uniform vs_params {
	vec4 offset[100];
};

void main() {
	gl_Position = vec4(position +  offset, 0.0, 1.0);
}
@end

@fs fs
in vec4 FragColor;

void main() {
	FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
@end

@program simple vs fs


