#version 400

in vec2 pass_textureCoords; 

uniform sampler2D textureSampler;
out vec4 out_colour;
void main(){

	out_colour = texture(textureSampler, pass_textureCoords);
}