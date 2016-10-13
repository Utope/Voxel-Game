#version 400

layout(location = 0) in  vec3 position;
layout(location = 1) in  vec3 color;

uniform mat4 matrixVP;

out vec3 pass_color;
void main(){
	gl_Position = matrixVP * vec4(position,1.0f);
	pass_color = color;
}