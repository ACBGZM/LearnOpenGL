#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform float pos_x_bias;
uniform float pos_y_bias;

void main(){
	vec3 pos_bias = vec3(pos_x_bias, pos_y_bias, 0.0f);
	gl_Position = vec4(aPos + pos_bias, 1.0f);		// ex2，给顶点的坐标加上偏移
	vertexColor = aColor;
	// vertexColor = aPos;		// ex3，颜色为顶点位置。左下角R是负数取0，G、B都是0，因此是黑色
}