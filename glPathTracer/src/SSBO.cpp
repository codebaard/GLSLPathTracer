#include <SSBO.h>
#include <Rendermesh.h>


void SSBO::_initBuffer() {
	glGenBuffers(1, &_SSBO);

}

void RendermeshSSBO::FillBuffer(Rendermesh* rendermesh) {

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Triangle)*rendermesh->Facecount, rendermesh->Faces, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);
	
	gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	
	if (gpuMem == NULL) {
		throw(std::exception("GPU Memory couldnt be mapped!"));
	}

	memcpy(gpuMem, rendermesh->Faces, sizeof(Triangle) * rendermesh->Facecount);

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void RendermeshSSBO::BindBuffer() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);
}

void RendermeshSSBO::UnbindBuffer() {
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TransformSSBO::FillBuffer(glm::mat4 projection, glm::mat4 view, glm::mat4 model) {

	_packData(projection, view, model);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrices), mat, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);

	gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	if (gpuMem == NULL) {
		throw(std::exception("GPU Memory couldnt be mapped!"));
	}

	memcpy(gpuMem, mat, sizeof(Matrices));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TransformSSBO::RefreshBuffer(glm::mat4 projection, glm::mat4 view, glm::mat4 model) {

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);

	gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	if (gpuMem == NULL) {
		throw(std::exception("GPU Memory couldnt be mapped!"));
	}

	memcpy(gpuMem, mat, sizeof(Matrices));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TransformSSBO::_packData(glm::mat4 projection, glm::mat4 view, glm::mat4 model) {
	mat->projection = projection;
	mat->view = view;
	mat->model = model;
}

//void RendermeshSSBO::LoadBuffer() {
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
//	gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
//
//	if (gpuMem == NULL)
//		jLog::Instance()->Error("Could not map shader log into client's memory space for reading.");
//	else
//		memcpy(&td, gpuMem, sizeof(td));
//
//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//}
