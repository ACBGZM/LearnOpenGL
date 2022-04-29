#version 330 core
out vec4 FragColor;

in vec3 VertexColor;	// ����vertex shader
in vec2 TexCoord;

uniform sampler2D texture1;	// ���������󴫸�fragment shader
uniform sampler2D texture2;

void main(){
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2f);	// ���ݶ��������е��������꣬�����������󣬻����ɫ�����
}