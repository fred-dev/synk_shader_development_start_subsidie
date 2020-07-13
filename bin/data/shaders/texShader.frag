#version 150

uniform sampler2DRect tex0;
uniform sampler2DRect prevTexture;

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



//---- Noise Functions

mat2 rot2d(float angle){return mat2(cos(angle),-sin(angle),sin(angle),cos(angle));}
float r(float a, float b){return fract(sin(dot(vec2(a,b),vec2(12.9898,78.233)))*43758.5453);}
float h(float a){return fract(sin(dot(a,dot(12.9898,78.233)))*43758.5453);}

float noise(vec3 x){
    vec3 p  = floor(x);
    vec3 f  = fract(x);
    f       = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0 + 113.0*p.z;
    return mix(mix(mix( h(n+0.0), h(n+1.0),f.x),
                   mix( h(n+57.0), h(n+58.0),f.x),f.y),
               mix(mix( h(n+113.0), h(n+114.0),f.x),
                   mix( h(n+170.0), h(n+171.0),f.x),f.y),f.z);
}

vec3 dnoise2f(vec2 p){
    float i = floor(p.x), j = floor(p.y);
    float u = p.x-i, v = p.y-j;
    float du = 30.*u*u*(u*(u-2.)+1.);
    float dv = 30.*v*v*(v*(v-2.)+1.);
    u=u*u*u*(u*(u*6.-15.)+10.);
    v=v*v*v*(v*(v*6.-15.)+10.);
    float a = r(i,     j    );
    float b = r(i+1.0, j    );
    float c = r(i,     j+1.0);
    float d = r(i+1.0, j+1.0);
    float k0 = a;
    float k1 = b-a;
    float k2 = c-a;
    float k3 = a-b-c+d;
    return vec3(k0 + k1*u + k2*v + k3*u*v,
                du*(k1 + k3*v),
                dv*(k2 + k3*u));
}

float fbm(vec2 uv){               
    vec2 p = uv;
	float f, dx, dz, w = 0.5;
    f = dx = dz = 0.0;
    for(int i = 0; i < 28; ++i){        
        vec3 n = dnoise2f(uv);
        dx += n.y;
        dz += n.z;
        f += w * n.x / (1.0 + dx*dx + dz*dz);
        w *= 0.86;
        uv *= vec2(1.16);
        uv *= rot2d(1.25*noise(vec3(p*0.1, 0.12*time))+
                    0.75*noise(vec3(p*0.1, 0.20*time)));
    }
    return f;
}

float fbmLow(vec2 uv){
    float f, dx, dz, w = 0.5;
    f = dx = dz = 0.0;
    for(int i = 0; i < 4; ++i){        
        vec3 n = dnoise2f(uv);
        dx += n.y;
        dz += n.z;
        f += w * n.x / (1.0 + dx*dx + dz*dz);
        w *= 0.75;
        uv *= vec2(1.5);
    }
    return f;
}

vec3 ColorContrast(vec3 x,float contrast){
	vec4 k0=vec4(-1.24,1.84,0.4,0.0);
	x=mix(x,x*x*x*k0.x+x*x*k0.y+x*k0.z+k0.w,contrast*(1.0+(0.6-pow(abs(x-0.5),vec3(0.5)))));
	return x;
}


void main (void)
{
    vec2 texCoord = vec2(varyingtexcoord.x , varyingtexcoord.y);
   // vec4 col = texture(prevTexture,texCoord) + (texture(tex0, texCoord) * FloatFrag1) ;
	vec4 col = texture(prevTexture, texCoord);



	if(mode == 1){

	//Start Transition
	//FLoatFrag 1 => Threshold

	//vec2 uv = texCoord;
	//vec3 t   = texture(tex0, texCoord).rgb;
	float thresh=(col.r+col.g+col.b)/3.0;


	if(FloatFrag1 >= thresh )
	col=col;
	else
	col = vec4(0.,0.,0.,1.);
	}


	else if(mode == 2){

	//Decay Shader
   	//FLoatFrag 1 => Desaturation
	//FLoatFrag 2 => Amplitude
	//FloatingFrag 3 => Speed


	vec2 uv = texCoord;
	uv=vec2(uv.x+sin((uv.y*.03)+(time * FloatFrag3 * 10 )) * ( FloatFrag2 * 20.) ,uv.y+sin((uv.x*.02)+(time * FloatFrag3 * 10 )) * (FloatFrag2 * 20.));

  	vec3 t = texture(tex0, uv).rgb; 
	vec3 grayXfer = vec3(0.3, 0.59, 0.11);
	vec3 gray = vec3(dot(grayXfer, t));
	t= vec3(mix(t, gray, FloatFrag1 ));
	col = vec4(t,1.);
	vec4 pre=texture(prevTexture, texCoord*0.95);

	col=(col*0.9)+(pre*0.1); 

   	}


   	else if(mode==3){

   	//HUE CYCLE
   	//FLoatFrag 1 to 3 => Color RGB
	//FloatFrag4 => Speed for Color A
	//FloatFrag5 => Speed for Color B


	float eps = .007;
	float PI = 3.1415927;
	vec2 uv = texCoord;
	vec3 t   = texture(prevTexture, uv).rgb;
	vec3 t00 = texture(prevTexture, uv+vec2(-eps,-eps)).rgb;
	vec3 t10 = texture(prevTexture, uv+vec2( eps,-eps)).rgb;
	vec3 t01 = texture(prevTexture, uv+vec2(-eps, eps)).rgb;
	vec3 t11 = texture(prevTexture, uv+vec2( eps, eps)).rgb;
	vec3 tm = (t00+t01+t10+t11)/4.;
	vec3 v=t; vec3 c;
	t = t-tm;
	//t = 1.-t;
	t = t*t*t;
	//t = 1.-t;
	v=t;
	v = 100.*t;

	float g = (tm.x-.3)*5.;
	//g = (g-.5); g = g*g*g/2.-.5; 
	vec3 col0 = sin(time*FloatFrag4* 30.0)*vec3(0.,0.,0.);
	vec3 col1 = sin(time* FloatFrag4 * 30.0)*vec3(FloatFrag1,FloatFrag2,FloatFrag3);
	vec3 col2 = sin(time* FloatFrag5 * 30.0)*vec3(FloatFrag3,FloatFrag1,FloatFrag2);
	vec3 col3 = vec3(.0,.0,.0);
	if      (g > 2.) c = mix(col2,col3,g-2.);
	else if (g > 1.) c = mix(col1,col2,g-1.);
	else             c = mix(col0,col1,g);
		
	c = clamp(c,0.,1.);
	v = clamp(v,0.,1.);
	v = c*(1.-v); 
	v = clamp(v,0.,1.);
	if (v==col0) v=col3;
	col = vec4(v,1.);
	}


 	else if(mode==4){

 	//ScanLiner
	// FloatFrag 1 to 3 => RGB COLOR
	// FloatFrag 4 => Speed

	vec2 res=vec2(100.0,100.0);
	vec2 uv = texture(prevTexture, texCoord).rg; 
    vec3 mCol = vec3(0);
    float w = 0.2;
    float r = tan(time* FloatFrag4 * 10. )* 0.5 + 0.6;
    float c = length(uv);
    mCol += smoothstep(r+w, r+w-0.1, c);
    mCol -= smoothstep(r, r-0.02, c);
    mCol *= vec3(FloatFrag1, FloatFrag2, FloatFrag3)*1.4*((c-r+0.03)/(w-0.01));
    col = vec4(mCol,1.0);

 	}


 	else if(mode==5){

 	//Digital Artifact
  	vec2 dir = texCoord;
 	float dist = distance(texCoord, vec2(5.));
  	vec2 offset = dir * (sin(dist * 0.1 + time*15.))* 0.05 ;
  	vec2 mtexCoord = texCoord + offset;
  	col = texture(prevTexture, mtexCoord);
  	//vec4 pre=texture(tex0, texCoord*0.97);
	//col=(col*0.7)+(pre*0.5); 

 	}

	else if(mode==6){

   //Digital Artifact II
   //FLoatFrag 1 => Distorion

    col = texture( prevTexture, texCoord * fbm(texCoord*FloatFrag1*0.05));

 	}

	
	else if(mode==7){

	//Digital Artifact III
	//FLoatFrag 1 => Distortion Size
	//FLoatFrag 2 => Wave Size
	vec2 uv = 0.1*texCoord;
    float t = time*0.6;
    col = vec4(fbm(uv*FloatFrag1*0.5),fbm(uv*FloatFrag1*0.5),fbm(uv*FloatFrag1*0.5),1.0);
 	float dist = distance(texCoord, vec2(.5));
  	vec2 offset = uv * (sin(dist * 0.1 + time*15.))* 0.05;
  	vec2 mtexCoord = texCoord + offset;
  	col=col/texture(prevTexture, mtexCoord);
	vec4 pre=texture(tex0, mtexCoord);
	col=(col*0.2)+(pre*0.7); 
 	}


	else if(mode == 8){

	//Color Madness II
	//FLoatFrag 1 => Speed

	vec2 uv = texCoord;
  	vec3 t = texture(tex0, uv).rgb; 
	col = vec4(t,1.);
	vec4 pre=texture(prevTexture, texCoord);
	float thresh=(col.r+col.g+col.b)/3.0;
	if(thresh > sin(time*FloatFrag1))
	col=vec4(col.r*sin(time),col.g*sin(time*2),col.b*sin(time*5),1.0);
	col=(col*0.8)+(pre*0.2); 
   	}


    outputColor.rgba = col.rgba;

}