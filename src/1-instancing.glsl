@ctype vec HMM_Vec4
@vs vs

in vec3 position;
layout (binding=0) uniform vs_params {
	vec4 offset[100];
}

void main() {
	gl_Position = vec4(position.x, position.y, position.z, 1.0f);

}
@end

@fs fs

void main() {

}
@end

@program simple vs fs


