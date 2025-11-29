/***
uniforms are global variable they need to be manually reset otherwise they carry on the current state.
***/

@vs vs
in vec4 position;

void main() {
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
@end

@fs fs
layout (binding = 0) uniform fs_params {
	vec4 ourColor; // the properties of ourColor carry from the main program
};

out vec4 FragColor; 

void main() {
	FragColor = ourColor;
}
@end


@program simple vs fs


