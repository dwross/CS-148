// phong.frag

varying vec3 varNormal;
varying vec3 varPos;

void main()
{
	vec3 N = varNormal;
	vec3 V = normalize(-varPos);
	vec3 L = normalize(gl_LightSource[0].position.xyz - varPos);
	vec3 R = normalize((-L) - 2.0*(dot((-L), N))*N);

	vec3 M_amb = vec3(0.5, 0.0, 0.0);
	vec3 M_dif = vec3(0.5, 0.0, 0.0);
	vec3 M_spc = vec3(0.5, 0.5, 0.5);

	vec3 I_amb = vec3(0.0, 0.0, 0.1);
	vec3 I_dif = vec3(1.0, 1.0, 1.0);
	vec3 I_spc = vec3(1.0, 1.0, 1.0);

	float shine = 10.0;

	vec3 amb = M_amb * I_amb;
	vec3 dif = clamp(dot(L, N), 0.0, 1.0) * (M_dif * I_dif);
	vec3 spc = pow(clamp(dot(R, V), 0.0, 1.0), shine) * (M_spc * I_spc);

    gl_FragColor = vec4(amb + dif + spc, 1.);
}
	