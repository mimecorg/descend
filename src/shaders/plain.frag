#version 330

uniform vec4 FrontColor;
uniform vec4 BackColor;

out vec4 outColor;

void main()
{
    outColor = gl_FrontFacing ? FrontColor : BackColor;
}
