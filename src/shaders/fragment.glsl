/**************************************************************************
* This file is part of the Descend program
* Copyright (C) 2012 Michał Męciński
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#version 330

uniform vec4 Color;

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

    outColor = diffuse * Color + vec4( specular, specular, specular, 1.0 );
}
