uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main() {
	int t;
	t+=int(gl_FragCoord.x/resolution.y*10.)%2;
	t+=int(gl_FragCoord.y/resolution.y*10.)%2;
    gl_FragColor = vec4( vec3( t%2+.5 ), 1.0 );
    //gl_FragColor = vec4(1);
}