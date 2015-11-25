#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

void main()
{	
    gl_Position = vec4(position.x, position.y + 0.3, position.z, 1.0f);
    ourColor = color;
}
