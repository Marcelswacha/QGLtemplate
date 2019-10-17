#version 460 core
in vec2 textureCoord;

uniform sampler2D texture1;
void main()
{
gl_FragColor = texture(texture1, textureCoord);
}
