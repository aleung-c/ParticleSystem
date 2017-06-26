#version 410

in vec2					TexCoords;
out vec4				frag_colour;

uniform sampler2D		text;
uniform vec3			textColor;
uniform bool			IsText;

void main()
{
	if (IsText)
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, (texture(text, TexCoords)).r);
		frag_colour = vec4(textColor, 1.0) * sampled;
	}
	else
	{
		// on UI elems, the Y on the texture is reversed. We correct it here.
		vec2 reverse_coords = TexCoords;
		reverse_coords.y *= -1.0;

		vec4 texel = texture(text, reverse_coords);
		frag_colour = texel;
	}
}
