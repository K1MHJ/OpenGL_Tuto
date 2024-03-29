#shader vertex
#version 330 core
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texCoord;

out vec2 v_TexCoord;
uniform mat4 u_MVP;

void main()
{
  gl_Position = u_MVP * vec4(a_position, 0, 1);
  v_TexCoord = a_texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
  //vec4 texColor = texture(u_Texture, v_TexCoord);
  vec3  c = vec3(1,0,0);
  float f = cos(gl_FragCoord.x);
  FragColor = u_Color * f;
}
