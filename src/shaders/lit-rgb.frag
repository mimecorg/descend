#version 330

uniform vec3 LightDirection;

uniform float AmbientIntensity;
uniform float DiffuseIntensity;
uniform float SpecularIntensity;
uniform float Shininess;

in vec3 exPosition;
in vec3 exNormal;
in vec3 exColor;

out vec4 outColor;

void main()
{
    vec3 faceNormal = normalize( gl_FrontFacing ? exNormal : -exNormal );

    vec3 eye = normalize( -exPosition );
    vec3 reflected = -reflect( LightDirection, faceNormal );

    float diffuse = AmbientIntensity + DiffuseIntensity * max( dot( faceNormal, LightDirection ), 0.0 );
    float specular = SpecularIntensity * pow( max( dot( reflected, eye ), 0.0 ), Shininess );

    vec4 color = vec4( exColor, 1.0 );

    outColor = diffuse * color + vec4( specular, specular, specular, 1.0 );
}
