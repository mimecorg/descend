#version 330

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

in vec3 inPosition;

void main()
{
    vec4 pos = ViewMatrix * vec4( inPosition, 1.0 );

    gl_Position = ProjectionMatrix * pos;
}
