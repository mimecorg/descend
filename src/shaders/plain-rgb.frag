#version 330

in vec3 exColor;

out vec4 outColor;

void main()
{
    outColor = vec4( exColor, 1.0 ) ;
}
