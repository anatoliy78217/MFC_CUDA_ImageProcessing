#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

//uniform vec2 tile = vec2(2,2), offset = vec2(0.2,0.2);

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	//gl_Position = transform * vec4(position, 1.0f);

	// We swap the y-axis by substracing our coordinates from 1. 
	// This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	TexCoord = texCoord;
	//TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
	//TexCoord = TexCoord * tile + offset;
	
}