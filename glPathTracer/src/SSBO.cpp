#include <SSBO.h>


void SSBO::_initBuffer() {
	glGenBuffers(1, &_SSBO);

}

void vertexSSBO::FillBuffer(Model model) {

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(model._meshes), &model._meshes, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);

	glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	memcpy(gpuMem, &model._meshes, sizeof(model._meshes));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

testSSBO::testSSBO() {
	_bindingIndex = 0;
	td.position = 0;
	td.sqrt = 0;
}

void testSSBO::MapBuffer(Shader* shader) {

	GLuint blockIndex = 0;
	blockIndex = glGetProgramResourceIndex(shader->ID, GL_SHADER_STORAGE_BLOCK, "test");

	if (blockIndex == GL_INVALID_INDEX) {
		throw(std::exception("Shader Block index invalid!"));
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, blockIndex, bindingPoint);
	glShaderStorageBlockBinding(shader->ID, blockIndex, bindingPoint);
}

void testSSBO::FillBuffer() {

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(td), &td, GL_STATIC_DRAW);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(td), &td, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);

	gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	memcpy(gpuMem, &td, sizeof(td));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void testSSBO::LoadBuffer() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	if (gpuMem == NULL)
		jLog::Instance()->Error("Could not map shader log into client's memory space for reading.");
	else
		memcpy(&td, gpuMem, sizeof(td));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
