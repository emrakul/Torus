#version 120

uniform vec3 position;
uniform vec3 direction;
uniform vec3 upVector;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float con;


float sdTorus( vec3 p, vec2 t )
{
	
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}


float map(vec3 p) {

	return sdTorus(p, vec2(6,2) );
}


vec3 getNormal(vec3 p ) {
	float eps = 0.00001;
	float center = map(p);
	vec3 n = vec3(  map(vec3(p.x-eps,p.y,p.z)) - map(vec3(p.x+eps,p.y,p.z)),
                   map(vec3(p.x,p.y-eps,p.z)) - map(vec3(p.x,p.y+eps,p.z)),
                   map(vec3(p.x,p.y,p.z-eps))  - map(vec3(p.x,p.y,p.z+eps)));
	return normalize(n);
}


vec3 intersect(vec3 origin, vec3 direction)
{
	float rayLength = 0.1;	
	    
    float maxt = 100.0;	
	float i;
	for (i = 0; i < 100; ++i)
	{
		vec3 point = origin + direction*rayLength;
		
		float dist = map(point );
		if(dist < 0.0001)
			return vec3(point);

		rayLength += dist;		
	}		
		
	return vec3(origin + direction*maxt);
}



void main(void){

	
	if(gl_FragCoord.x > 1200 || gl_FragCoord.y > 700 ||  gl_FragCoord.x < 400 || gl_FragCoord.y < 200){
	gl_FragDepth = 0.0;
	gl_FragColor = vec4(0.0);

	}
	else{

	vec2 resolution = vec2(1600, 900);

	vec2 uv = gl_FragCoord.xy / resolution;


		

	vec2 p = (uv * 2.0 - 1.0); p.x *= resolution.x / resolution.y;	
	vec3 origin = position;
	vec3 target = position + direction;

	vec3 direction = normalize(target - origin);
	vec3 right = normalize(cross(direction, upVector));
	vec3 up = normalize(cross(right, direction));
	vec3 rayDirection = normalize(p.x * right + p.y * up + 1.5 * direction);		
	vec3 pp = intersect(origin, rayDirection);
	vec3 normal = 0.5*getNormal(pp) + 0.5;

	vec4 clip = projectionMatrix*viewMatrix*vec4(pp,1.0);
	float z_n = clip.z;



	z_n = length(pp-origin) - length(p.x+p.y)*con;		
	float A = projectionMatrix[2].z;
    float B = projectionMatrix[3].z;
	float z_e = 0.5*(-A*z_n + B) / z_n + 0.5;
	
    gl_FragDepth  = z_e;


	gl_FragColor = vec4(normal,1.0);
	//gl_FragDepth = z_e;
	}
	
}