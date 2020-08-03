#include <SSBO.h>
#include <Rendermesh.h>


void SSBO::_initBuffer() {
	glGenBuffers(1, &_SSBO);
}

RendermeshSSBO::RendermeshSSBO() {
	_bindingIndex = RENDERMESH_SSBO_BINDING_POINT;
	Mesh = (Rendermesh*)malloc(sizeof(Rendermesh));
}

TransformSSBO::TransformSSBO() {
	_bindingIndex = TRANSFORM_SSBO_BINDING_POINT;
	mat = new Matrices();
}

void RendermeshSSBO::FillBuffer(Rendermesh* rendermesh) {

	this->Mesh = rendermesh;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Triangle)*rendermesh->Facecount, rendermesh->Faces, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);
	
	//gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	gpuMem = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Triangle) * rendermesh->Facecount, GL_MAP_WRITE_BIT);
	
	if (gpuMem == NULL) {
		throw(std::exception("GPU Memory couldnt be mapped!"));
	}

	memcpy(gpuMem, Mesh->Faces, sizeof(Triangle) * Mesh->Facecount);

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

void RendermeshSSBO::ReadBuffer(Rendermesh* rendermesh) {

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); //essentially: wait until ready.

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	if (gpuMem == NULL)
		jLog::Instance()->Error("Could not map shader log into clients memory space for reading.");
	else
		memcpy(Mesh, gpuMem, sizeof(Triangle)*rendermesh->Facecount);

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

//void RendermeshSSBO::MapBuffer(Rendermesh* rendermesh) {
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);
//
//	GLuint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
//
//	Triangle* Faces = (Triangle*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, rendermesh->Facecount * sizeof(Triangle), bufMask);
//
//	Faces = rendermesh->Faces;
//
//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//}

void TransformSSBO::FillBuffer(glm::mat4 projection, glm::mat4 view, glm::mat4 model) {

	_packData(projection, view, model);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrices), mat, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);

	gpuMem = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Matrices), GL_MAP_WRITE_BIT);

	if (gpuMem == NULL) {
		throw(std::exception("GPU Memory couldnt be mapped!"));
	}

	memcpy(gpuMem, mat, sizeof(Matrices));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TransformSSBO::LoadBuffer() {

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); //essentially: wait until ready.

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);
	gpuMem = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Matrices), GL_MAP_READ_BIT);

	if (gpuMem == NULL)
		jLog::Instance()->Error("Could not map shader log into client's memory space for reading.");
	else
		memcpy(&mat, (Matrices*)gpuMem, sizeof(mat));

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
