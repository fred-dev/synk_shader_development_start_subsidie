#version 150

uniform mat4 matrix;
in vec4 position;
in vec2 texcoord;


uniform int mode;
uniform float FloatVert1;
uniform float FloatVert2;
uniform float FloatVert3;
uniform float FloatVert4;
uniform float FloatVert5;
uniform float FloatVert6;
uniform float time;

// this is something we're creating for this shader
out vec2 texCoordVarying;


void main()
{
    texCoordVarying = vec2(texcoord.x, texcoord.y);

    vec4 outPosition = matrix * position;	

	gl_Position = outPosition;
}