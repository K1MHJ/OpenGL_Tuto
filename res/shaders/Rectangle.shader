#shader vertex
#version 330 core

layout(location = 0) in vec2 particle_vert;
layout(location = 1) in vec2 pos;

uniform mat4 u_MVP;

void main()
{
  mat4 matrix = mat4(1.0, 0.0, 0.0, 0.0,
                       0.0, 1.0, 0.0, 0.0,
                       0.0, 0.0, 1.0, 0.0,
                       pos.x, pos.y, 0.0, 1.0);
  gl_Position = u_MVP * matrix * vec4(particle_vert,0.0,1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
  color = u_Color;
}
