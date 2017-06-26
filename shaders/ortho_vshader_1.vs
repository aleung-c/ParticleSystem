#version 410

/*
**	The shader for displaying text, as the visualization is very different
**	from a 3d model.
*/

in vec4						vp;

uniform mat4				projection_matrix;
out vec2					TexCoords;

void main()
{
    gl_Position = projection_matrix * vec4(vp.x, vp.y, 0.0, 1.0);
    TexCoords.x = vp.z;
    TexCoords.y = vp.w;
}
