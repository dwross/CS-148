// phong.vert

varying vec3 varNormal;
varying vec3 varPos;

void main()
{
    // we want to render the shape using standard OpenGL position transforms.
    gl_Position = ftransform();

    varNormal = (gl_NormalMatrix * gl_Normal).xyz;
	varPos = (gl_ModelViewMatrix * gl_Vertex).xyz;
}
