#version 330 core
out vec4 FragColor;

in vec3 VertexColor;	// ����vertex shader
in vec2 TexCoord;

uniform sampler2D texture1;	// ��������󴫸�fragment shader
uniform sampler2D texture2;

uniform float index;

void main(){
	// ��texture2ˮƽ��ת
	vec2 TexCoordXRev =  vec2(1.0 - TexCoord.x, TexCoord.y);

	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoordXRev), index);	// ���ݶ��������е��������꣬����������󣬻����ɫ�����
}