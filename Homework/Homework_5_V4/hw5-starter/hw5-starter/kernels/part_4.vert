// part_4.vert

varying vec3 varNormal;
varying vec3 varPos;

void main()
{
    gl_Position = ftransform();

    varNormal =(gl_NormalMatrix * gl_Normal).xyz;
	varPos = (gl_ModelViewMatrix * gl_Vertex).xyz;
}