#version 150

// this is how we receive the texture
uniform sampler2DRect tex0;

in vec2 texCoordVarying;
out vec4 outputColor;

uniform int mode;
uniform float FloatFrag1;
uniform float FloatFrag2;
uniform float FloatFrag3;
uniform float FloatFrag4;
uniform float FloatFrag5;
uniform float FloatFrag6;
uniform float time;

void main()
{
    outputColor = texture(tex0, texCoordVarying);
}