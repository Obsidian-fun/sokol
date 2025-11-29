/***
uniforms are global variable they need to be manually reset otherwise they carry on the current state.
***/

@vs vs
in vec3 position;
in vec3 aColor;

out vec3 ourColor;

void main() {
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	ourColor = aColor; // setting unifofm to aColor which is the input from VBO.
}
@end

@fs fs

in vec3 ourColor;
out vec4 FragColor;

void main() {
	FragColor = vec4(ourColor, 1.0f);
}
@end


@program simple vs fs


