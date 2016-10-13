#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec2 pass_textureCoords;

uniform mat4 vpMatrix;
uniform mat4 modelMatrix;
void main(){
	
	gl_Position = vpMatrix * modelMatrix  * vec4(position,1.0f);
	

	pass_textureCoords = textureCoords;
}