#shader vertex
#version 330
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
uniform bool is_circle;

void main()
{
	if (is_circle) {
		vec4 texColor = texture(u_Texture, v_texCoord);
		texColor.r = 0.0f;
		texColor.b = 0.0f;
		texColor.g = 1.0f;
		color = texColor;
	}
	else {
		vec4 texColor = texture(u_Texture, v_texCoord);
		color = vec4(0.0f,1.0f,0.0f,1.0f);
	}

}