/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#version 450 core

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vTexCoord;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;

out vec2 fTexCoord;

void main()
{
	gl_Position = Projection * View * World * vPosition;
	fTexCoord = vTexCoord;
}