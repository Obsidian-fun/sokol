@vs vs

in vec2 position;
in vec2 offsets;

void main() {
	vec2 pos = position * (gl_InstanceIndex/100.0);
	gl_Position = vec4(pos +  offsets, 0.0, 1.0);
}
@end

@fs fs
out vec4 FragColor;

void main() {
	FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
@end

@program simple vs fs


