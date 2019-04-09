#version 330

in vec2 texCoord;
in float Time;
in float lightFactor;

uniform sampler2D tex;

void main()
{
	// Party Mode
	//vec2 newChoord = vec2(texCoord.x + cos((Time + texCoord.y)*5)* 0.1, texCoord.y + sin((Time + texCoord.x)*3)* 0.1);
	//gl_FragColor = texture(tex, newChoord);

	//Regular Boring Mode
	gl_FragColor = texture(tex, texCoord) * lightFactor * 5;
}
