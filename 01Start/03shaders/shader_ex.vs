#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform float pos_x_bias;
uniform float pos_y_bias;

void main(){
	vec3 pos_bias = vec3(pos_x_bias, pos_y_bias, 0.0f);
	gl_Position = vec4(aPos + pos_bias, 1.0f);		// ex2����������������ƫ��
	vertexColor = aColor;
	// vertexColor = aPos;		// ex3����ɫΪ����λ�á����½�R�Ǹ���ȡ0��G��B����0������Ǻ�ɫ
}