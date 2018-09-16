// part_4.frag

uniform vec3 M_amb;
uniform vec3 M_dif;
uniform vec3 M_spc;

uniform vec3 I_amb;
uniform vec3 I_dif;
uniform vec3 I_spc;

uniform float shine;

varying vec3 varNormal;
varying vec3 varPos;

void main()
{

// unit vectors needed for the per pixel lighting

	vec3 N = normalize(varNormal);
	vec3 V = normalize(-varPos);
	vec3 L = normalize(gl_LightSource[0].position.xyz - varPos);
	vec3 R = normalize((-L) - 2.0*(dot((-L), N))*N);

// per pixel light calculations

	vec3 amb = M_amb * I_amb;
	vec3 dif = clamp(dot(N, L), 0.0, 1.0) * (M_dif * I_dif);
	vec3 spc = pow(clamp(dot(V, R), 0.0, 1.0), shine) * (M_spc * I_spc);

    gl_FragColor = vec4(amb + dif + spc, 1.);
}
