#shader vertex
#version 330 core
layout(location = 0) in vec2 a_position;

uniform mat4 u_MVP;

void main()
{
  gl_Position = u_MVP * vec4(a_position, 0.0f, 1.0f);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
  FragColor = u_Color;
}
