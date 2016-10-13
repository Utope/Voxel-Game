#version 400

attribute vec3 position;

uniform mat4 vpMatrix;

out vec3 color;
void main(){
	
	gl_Position = vpMatrix * vec4(position,1.0f);
	color = position;
}