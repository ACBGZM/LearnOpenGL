#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;	// 将纹理对象传给fragment shader
uniform sampler2D texture2;

void main(){
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2f);	// 根据顶点属性中的纹理坐标，查找纹理对象，获得颜色，输出
}