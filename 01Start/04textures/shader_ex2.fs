#version 330 core
out vec4 FragColor;

in vec3 VertexColor;	// 来自vertex shader
in vec2 TexCoord;

uniform sampler2D texture1;	// 将纹理对象传给fragment shader
uniform sampler2D texture2;

uniform float index;

void main(){
	// 让texture2水平反转
	vec2 TexCoordXRev =  vec2(1.0 - TexCoord.x, TexCoord.y);

	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoordXRev), index);	// 根据顶点属性中的纹理坐标，查找纹理对象，获得颜色，输出
}