#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
//gtr State Of The Art Amiga
void main( void ) {

   vec2 p =   gl_FragCoord.xy / resolution.xy *2.-1.  ;
   p.x *= resolution.x/resolution.y;
	 
   	
    vec2 c1 = vec2(0.9, 0.0);
    c1 += vec2(0.0  , cos(time) * .85);
    
    vec2 c2 = vec2(0.0, 0.25);
    c2 += vec2(-sin(time+sin(time)) * 1.40, cos(time) * .85+sin(time*0.4));
    
	 
   
    if (mod(float(int(distance(p,c1) * 15.)), 2.) == 1.){
        gl_FragColor = vec4(0.55, 0.14, 0.15, 1.0);
    }
    
    if (mod(float(int(distance(p,c2) * 15.)), 2.) == 0.){
        gl_FragColor = vec4(0,0,0,0);
    }

}