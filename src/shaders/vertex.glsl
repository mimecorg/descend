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
