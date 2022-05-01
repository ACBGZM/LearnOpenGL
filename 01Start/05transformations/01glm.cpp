#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>

#include<iostream>

int main()
{
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	
	glm::mat4 trans;	// �ҵ�glm�汾��0.9.8.5�������0.9.9�����ϣ���ʼ����λ������Ҫ��Ϊ��glm::mat4 trans = glm::mat4(1.0f
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));	// ƽ��(1,1,0)
	
	vec = trans * vec;
	
	std::cout << vec.x << vec.y << vec.z << std::endl;

	return 0;
}