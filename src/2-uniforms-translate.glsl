/***
uniforms are global variable they need to be manually reset otherwise they carry on the current state.
***/

@vs vs
in vec4 position;
layout (binding = 0) uniform fs_params {
	vec4 ourPostion; 
};

void main() {
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	gl_Position = ourPosition;
}
@end

@fs fs
out vec4 FragColor; 

void main() {
	FragColor = ourColor;
}
@end

@program simple vs fs


