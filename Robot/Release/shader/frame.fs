#version 430

uniform int mode;
uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;
uniform sampler2D tex;
in vec2 texcoord;
out vec4 vFragColor;




vec2 sw(vec2 p) {return vec2( floor(p.x) , floor(p.y) );}
vec2 se(vec2 p) {return vec2( ceil(p.x)  , floor(p.y) );}
vec2 nw(vec2 p) {return vec2( floor(p.x) , ceil(p.y)  );}
vec2 ne(vec2 p) {return vec2( ceil(p.x)  , ceil(p.y)  );}
float random(float p) {
	return fract(sin(p)*10000.);
}
float noise(vec2 p) {
	return random(p.x + p.y*10000.);
}
float smoothNoise(vec2 p) {
	vec2 inter = smoothstep(0., 1., fract(p));
	float s = mix(noise(sw(p)), noise(se(p)), inter.x);
	float n = mix(noise(nw(p)), noise(ne(p)), inter.x);
	return mix(s, n, inter.y);
	return noise(nw(p));
}
float movingNoise(vec2 p) {
	float total = 0.0;
	total += smoothNoise(p     - time);
	total += smoothNoise(p*2.  + time) / 2.;
	total += smoothNoise(p*4.  - time) / 4.;
	total += smoothNoise(p*8.  + time) / 8.;
	total += smoothNoise(p*16. - time) / 16.;
	total /= 1. + 1./2. + 1./4. + 1./8. + 1./16.;
	return total;
}
float nestedNoise(vec2 p) {
	float x = movingNoise(p);
	float y = movingNoise(p + 100.);
	return movingNoise(p + vec2(x, y));
}

void main(){
	if(mode==0){//None
		vFragColor = texture2D( tex, texcoord );
	}
	else if(mode==1){//Offset
		vec2 texC=texcoord;
		vFragColor = texture2D( tex, texC + vec2(time/10, 0.5) );
	}
	else if(mode==2){//Mosaics
		vec2 texC=texcoord;
		texC.x = floor(texcoord.x * float(50)) / float(50)+0.01;
		texC.y = floor(texcoord.y * float(50)) / float(50)+0.01;

		vFragColor = texture2D( tex, texC );
	}
	else if(mode==3){//PartMosaics
		vec2 texC=texcoord;
		if(length((texC-mouse)*resolution)<0.15*resolution.y){
			texC.x = floor(texcoord.x * float(50)) / float(50)+0.01;
			texC.y = floor(texcoord.y * float(50)) / float(50)+0.01;
		}
		vFragColor = texture2D( tex, texC );
	}
	else if(mode==4){//Color Distortion
		vec4 color = texture2D(tex, texcoord);
		vFragColor.r = (sin(color.r*10)+1.)*.5;
		vFragColor.g = (sin(color.g*20)+1.)*.5;
		vFragColor.b = (sin(color.b*40)+1.)*.5;
		vFragColor.a = 1.;
	}
	else if(mode==5){//Spatial Distortion
		vec2 p = texcoord;
		p.x = p.x + sin(p.y*80.+time*6.)*0.03;
		vFragColor = texture2D(tex, p);
	}
	else if(mode==6){//Displacement
		vec2 p = texcoord;
		vec4 color = texture2D(tex, p);
		p.x += color.r * 0.1 * sin(time);
		float brightness = fract(p.x*10);
		vFragColor.rgb = vec3(brightness);
		vFragColor.a = 1.;
	}
	else if(mode==7){//Patterns
		vec2 p = texcoord;
		float size = 0.1;
		p.x = mod(p.x, size);
		p.x = abs(p.x - size/2.);
		p.x = abs(mod(p.x + time/6., 2.)-1.);
		vFragColor = texture2D(tex, p);
	}
	else if(mode==8){//fog
		vec2 p = texcoord * 6.;
		float brightness = nestedNoise(p);
		vFragColor.rgb = vec3(brightness);
		vFragColor.rgb = vec3(brightness,nestedNoise(p*2),nestedNoise(p*3));
		vFragColor= vFragColor + texture2D( tex, texcoord );
	}
	else if(mode==9){//DoG
		float sigma_e = 1.0;
		float sigma_r = 1.4;
		float phi = 3.4;
		float tau = 0.99;
		int nbins = 8;
		vec2 img_size = vec2(10,10);
		//DoG
		float twoSigmaESquared = 2.0 * sigma_e * sigma_e;
		float twoSigmaRSquared = 2.0 * sigma_r * sigma_r;
		int halfWidth = int(ceil( 2.0 * sigma_r ));
		vec4 color = vec4(0);
		int n = 0;
		vec2 sum = vec2(0.0);
		vec2 norm = vec2(0.0);
		for ( int i = -halfWidth; i <= halfWidth; ++i ) {
			for ( int j = -halfWidth; j <= halfWidth; ++j ) {
				float d = length(vec2(i,j));
				vec2 kernel = vec2( exp( -d * d / twoSigmaESquared ), exp( -d * d / twoSigmaRSquared ));
				vec4 c = texture2D(tex, texcoord + vec2(i,j) / img_size);
				vec2 L = vec2(0.299 * c.r + 0.587 * c.g + 0.114 * c.b);
				color+= c;
				n++;
				norm += 2.0 * kernel;
				sum += kernel * L;
			}
		}
		sum /= norm;
		float H = 100.0 * (sum.x - tau * sum.y);
		float edge = ( H > 0.0 )? 1.0 : 2.0 * smoothstep(-2.0, 2.0, phi * H );
		vec3 edge_color = vec3(0.0,0.0,0.0);
		color/=n; //Blur
		vFragColor = vec4(mix(edge_color, vec3(1), edge),1.0);
	}
	else if(mode==10){//Swirling
		float radius = 50.0+time*10;
		float angle = time/20;
		vec2 uv = texcoord;
		vec2 center = resolution/2;
		vec2 texSize = resolution;
		vec2 tc = uv * texSize;
		tc -= center;
		float dist = length(tc);
		if (dist < radius){
			float percent = (radius - dist) / radius;
			float theta = percent * percent * angle * 8.0;
			float s = sin(theta);
			float c = cos(theta);
			tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
		}
		tc += center;
		vec3 color = texture2D(tex, tc / texSize).rgb;
		vFragColor = vec4(color, 1.0);
	}
	else if(mode==11){//Cross stitching
		float stitching_size = 10.0;
		int invert = 1;
		vec2 uv = texcoord;
		vec4 c = vec4(0.0);
		float size = stitching_size;
		vec2 cPos = uv * resolution;
		vec2 tlPos = floor(cPos / vec2(size, size));
		tlPos *= size;
		int remX = int(mod(cPos.x, size));
		int remY = int(mod(cPos.y, size));
		if (remX == 0 && remY == 0)
			tlPos = cPos;
		vec2 blPos = tlPos;
		blPos.y += (size - 1.0);
		if ((remX == remY) || (((int(cPos.x) - int(blPos.x)) == (int(blPos.y) - int(cPos.y))))){
			c = texture2D(tex, tlPos * vec2(1.0/resolution.x, 1.0/resolution.y)) * 1.4;
		}
		else{
			c = vec4(0.0, 0.0, 0.0, 1.0);
		}
		vFragColor = c;
	}
	else if(mode==12){//Edge Detectors
		const mat3 G2[9] = mat3[](
			1.0/(2.0*sqrt(2.0)) * mat3( 1.0, sqrt(2.0), 1.0, 0.0, 0.0, 0.0, -1.0, -sqrt(2.0), -1.0 ),
			1.0/(2.0*sqrt(2.0)) * mat3( 1.0, 0.0, -1.0, sqrt(2.0), 0.0, -sqrt(2.0), 1.0, 0.0, -1.0 ),
			1.0/(2.0*sqrt(2.0)) * mat3( 0.0, -1.0, sqrt(2.0), 1.0, 0.0, -1.0, -sqrt(2.0), 1.0, 0.0 ),
			1.0/(2.0*sqrt(2.0)) * mat3( sqrt(2.0), -1.0, 0.0, -1.0, 0.0, 1.0, 0.0, 1.0, -sqrt(2.0) ),
			1.0/2.0 * mat3( 0.0, 1.0, 0.0, -1.0, 0.0, -1.0, 0.0, 1.0, 0.0 ),
			1.0/2.0 * mat3( -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0 ),
			1.0/6.0 * mat3( 1.0, -2.0, 1.0, -2.0, 4.0, -2.0, 1.0, -2.0, 1.0 ),
			1.0/6.0 * mat3( -2.0, 1.0, -2.0, 1.0, 4.0, 1.0, -2.0, 1.0, -2.0 ),
			1.0/3.0 * mat3( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
		);
		vec2 uv = texcoord;
		vec3 tc = vec3(1.0, 0.0, 0.0);
		vec3 samplee;
		mat3 I;
		float cnv[9];
		int i, j;
		for (i=0; i<3; i++){
			for (j=0; j<3; j++) {
				samplee = texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2(i-1,j-1), 0).rgb;
				I[i][j] = length(samplee); 
			}
		}
		for (i=0; i<9; i++) {
			float dp3 = dot(G2[i][0], I[0]) + dot(G2[i][1], I[1]) + dot(G2[i][2], I[2]);
			cnv[i] = dp3 * dp3; 
		}
		float M = (cnv[4] + cnv[5]) + (cnv[6] + cnv[7]); // Line detector
		float S = (cnv[0] + cnv[1]) + (cnv[2] + cnv[3]) + (cnv[4] + cnv[5]) + (cnv[6] + cnv[7]) + cnv[8]; 
		tc = vec3(sqrt(M/S));
		vFragColor = vec4(tc, 1.0);
	}
	else if(mode==13){//Radial Blur Filter
		const float sampleDist = 1.0;
		const float sampleStrength = 2.2;
		float samples[10] =
		float[](-0.08,-0.05,-0.03,-0.02,-0.01,0.01,0.02,0.03,0.05,0.08);
		vec2 dir = 0.5 - texcoord;
		float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
		dir = dir/dist;
		vec4 color = texture2D(tex,texcoord);
		vec4 sum = color;
		for (int i = 0; i < 10; i++){
			sum += texture2D( tex, texcoord + dir * samples[i] * sampleDist );
		}
		sum *= 1.0/11.0;
		float t = dist * sampleStrength;
		t = clamp( t ,0.0,1.0);
		vFragColor = mix( color, sum, t );
	}
	else if(mode==14){//Kaleidoscope
		float s=2;
		vec2 texC=texcoord;
		texC.x=texC.x*resolution.x/resolution.y;
		texC.x=texC.x-floor(texC.x*s)/s;
		texC.y=texC.y-floor(texC.y*(s/0.577))/(s/0.577);
		if(texC.y>1./(s/0.577)/2.)
			texC.y=1./(s/0.577)-texC.y;
		if(texC.x>1./s/2.)
			texC.x=1./s-texC.x;
		texC=texC+vec2(0.5)+vec2(sin(time/10.),cos(time/10.));
		vFragColor = texture2D( tex, texC );
	}
}