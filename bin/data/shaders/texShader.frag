#version 150

uniform sampler2DRect tex0;

uniform int mode;
uniform float FloatFrag1;
uniform float FloatFrag2;
uniform float FloatFrag3;
uniform float FloatFrag4;
uniform float FloatFrag5;
uniform float FloatFrag6;
uniform float time;

in vec2 varyingtexcoord;
out vec4 outputColor;



void main (void)
{
    
    vec2 texCoord = vec2(varyingtexcoord.x , varyingtexcoord.y);
    vec4 col = texture(tex0,texCoord);
    outputColor.rgba = col.rgba;
}