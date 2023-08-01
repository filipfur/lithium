#version 330 core

#include lightning.frag

uniform sampler2D u_texture;

uniform vec4 u_color;

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

void main()
{
    vec4 color = texture(u_texture, texCoord) * u_color;
    fragColor = vec4(pow(color.rgb, vec3(1.0/2.2)), color.a);
}