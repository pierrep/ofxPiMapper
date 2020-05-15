// vertex shader

#version 330

uniform mat4 modelViewProjectionMatrix;

layout (location = 0) in vec3 position;
layout (location = 3) in vec3 texcoord;

out vec3 varyingtexcoord;

void main(){
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
	varyingtexcoord = texcoord;
}
