/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#version 450 core

in vec2 fTexCoord;

layout( location = 0 ) out vec4 fColor;

// uniform sampler2D tex;
uniform layout (binding = 0) sampler2D tex;

void main()
{
	fColor = texture(tex, fTexCoord);
	// fColor = texture(tex[textureIndex], fTexCoord);
	// fColor = vec4(fTexCoord, 1.0f, 1.0f);
}