#version 330 core
out vec4 FragColor;

in vec3 VertexColor;	// 来自vertex shader
in vec2 TexCoord;

uniform sampler2D texture1;	// 将纹理对象传给fragment shader

void main(){
	FragColor = texture(texture1, TexCoord);	// 根据顶点属性中的纹理坐标，查找纹理对象，获得颜色，输出
}