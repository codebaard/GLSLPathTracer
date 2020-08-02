#version 430 core

layout(std430, binding = 0) buffer testSSBO{
	uint position;
	float root;
} test;

void main(){

	test.position += 1;
	test.root = sqrt(test.position);
}