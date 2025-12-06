@ctype vec4 HMM_Vec4
@vs vs

in vec2 position;
layout (binding = 0) uniform vs_params {
	vec4 offsets[100];
};

void main() {
	vec2 offset = offsets[gl_InstanceIndex].xy;
	gl_Position = vec4(position +  offset, 0.0, 1.0);
}
@end

@fs fs
out vec4 FragColor;

void main() {
	FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
@end

@program simple vs fs


