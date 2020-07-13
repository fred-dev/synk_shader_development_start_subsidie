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

vec3 EffectShape5(vec3 x,float effect){
	vec4 B=vec4(0.0,0.0,1.5,-0.28);
	vec4 G=vec4(0.01,0.0,1.03,-0.02);
	vec4 R=vec4(0.0,0.0,0.77,0.1);
	effect*=4.0/5.0;
	x.r=mix(x,x*x*x*R.x+x*x*R.y+x*R.z+R.w,effect).r;
	x.g=mix(x,x*x*x*G.x+x*x*G.y+x*G.z+G.w,effect).g;
	x.b=mix(x,max(-0.3*x,x*x*x*B.x+x*x*B.y+x*B.z+B.w),effect).b;
	return x;
}


vec3 ColorShape(vec3 x,float color){
	x+=(color-0.5)*vec3(0.0,0.08,-0.24);
	return x;
}
vec3 ColorContrast(vec3 x,float contrast){
	vec4 k0=vec4(-1.24,1.84,0.4,0.0);
	x=mix(x,x*x*x*k0.x+x*x*k0.y+x*k0.z+k0.w,contrast*(1.0+(0.6-pow(abs(x-0.5),vec3(0.5)))));
	return x;
}

void main()
{

	//FLoatFrag 1 => Effect Power
	//FLoatFrag 2 => Contrast
	//FLoatFrag 3 => Color Shape

	vec4 col=texture(tex0, texCoordVarying);
	if(mode!=0){
	for(int i=0;i<3;i++){
	col.rgb=ColorShape(col.rgb,FloatFrag3);
	col.rgb=ColorContrast(col.rgb,FloatFrag2);
	}
	col.rgb=EffectShape5(col.rgb,FloatFrag1);
	}
    outputColor = col;


}

