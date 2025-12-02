@ctype mat4 HMM_Mat4

@vs vs
in vec3 position;

void main() {
	gl_Position = transform * vec4(position, 1.0);
}
@end

@fs fs
out FragColor;

void main() {
	FragColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);
}
@end





