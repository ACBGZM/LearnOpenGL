#version 330 core
out vec4 FragColor;

in vec3 VertexColor;	// ����vertex shader
in vec2 TexCoord;

uniform sampler2D texture1;	// ��������󴫸�fragment shader

void main(){
	FragColor = texture(texture1, TexCoord);	// ���ݶ��������е��������꣬����������󣬻����ɫ�����
}