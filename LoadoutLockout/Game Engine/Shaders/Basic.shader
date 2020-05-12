#shader vertex
#version 430
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
uniform vec2 tex_off;
uniform vec2 tex_mul;

out vec2 v_texCoord;

uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * vec4(position,0.0,1.0);
	v_texCoord.x = texCoord.x*tex_mul.x + tex_off.x;
	v_texCoord.y = texCoord.y*tex_mul.y + tex_off.y;
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
	color = texColor * u_Color;
};