// fragment shader

#version 150

uniform sampler2D tex;

in vec3 varyingtexcoord;

out vec4 outputColor;

void main()
{
	outputColor = texture2D(tex, varyingtexcoord.xy / varyingtexcoord.z);

}
