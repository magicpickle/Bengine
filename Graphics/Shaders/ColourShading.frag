#version 130

in vec4 fragmentColor;
in vec2 fragmentPosition;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D mySampler;

void main()
{
	//color = fragmentColor + vec4(1.0 * (cos(time)+1.0) * 0.5, 1.0 * (cos(time+2)+1.0) * 0.5, 1.0 * (cos(time+1)+1.0) * 0.5, 0.0);

	vec4 textureColor = texture(mySampler, fragmentUV);

	color = fragmentColor * textureColor;



}