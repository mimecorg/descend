#version 330

uniform vec4 FrontColor;
uniform vec4 BackColor;

uniform vec3 LightDirection;

uniform float AmbientIntensity;
uniform float DiffuseIntensity;
uniform float SpecularIntensity;
uniform float Shininess;

in vec3 exPosition;
in vec3 exNormal;

out vec4 outColor;

void main()
{
    vec3 faceNormal = normalize( gl_FrontFacing ? exNormal : -exNormal );

    vec3 eye = normalize( -exPosition );
    vec3 reflected = -reflect( LightDirection, faceNormal );

    float diffuse = AmbientIntensity + DiffuseIntensity * max( dot( faceNormal, LightDirection ), 0.0 );
    float specular = SpecularIntensity * pow( max( dot( reflected, eye ), 0.0 ), Shininess );

    vec4 color = gl_FrontFacing ? FrontColor : BackColor;

    outColor = diffuse * color + vec4( specular, specular, specular, 1.0 );
}
