#version 330

uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

in vec3 inPosition;
in vec3 inNormal;

out vec3 exPosition;
out vec3 exNormal;

void main()
{
    vec4 pos = ViewMatrix * vec4( inPosition, 1.0 );

    exPosition = vec3( pos );
    exNormal = NormalMatrix * inNormal;

    gl_Position = ProjectionMatrix * pos;
}
