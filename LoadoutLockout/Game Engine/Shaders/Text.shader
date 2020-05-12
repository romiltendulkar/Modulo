#shader vertex
#version 430
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 u_mvp;
void main()
{
	gl_Position = u_mvp * vec4(position, 0.0, 1.0);
	v_texCoord = texCoord;
};

#shader fragment
#version 430
layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 v_texCoord;

void main()
{
	vec4 texColor = texture(u_Texture, v_texCoord);
	color = texColor.x * u_Color;
};
