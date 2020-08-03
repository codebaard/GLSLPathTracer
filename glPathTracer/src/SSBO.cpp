#include <SSBO.h>
#include <Rendermesh.h>


void SSBO::_initBuffer() {
	glGenBuffers(1, &_SSBO);
}

RendermeshSSBO::RendermeshSSBO() {
	//_bindingIndex = RENDERMESH_SSBO_BINDING_POINT;
	_mesh = (Rendermesh*)malloc(sizeof(Rendermesh));
	_bindingName = "RendermeshSSBO";
}

TransformSSBO::TransformSSBO() {
	//_bindingIndex = TRANSFORM_SSBO_BINDING_POINT;
	_mat = new Matrices();
	_bindingName = "TransformSSBO";
}

void RendermeshSSBO::FillBuffer(Rendermesh* rendermesh) {

	this->_mesh = rendermesh;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Triangle)*rendermesh->Facecount, rendermesh->Faces, GL_DYNAMIC_COPY);
	
	//gpuMem = (Triangle*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Triangle), GL_MAP_READ_BIT);
	gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

	if (gpuMem == NULL)
		throw(std::exception("GPU Memory couldnt be mapped!"));

	memcpy(gpuMem, _mesh->Faces, sizeof(Triangle) * _mesh->Facecount);

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

//GLuint ssbo = 0;
//glGenBuffers(1, &ssbo);
//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(shader_data), &shader_data, GL_DYNAMIC_COPY);

//GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
//memcpy(p, &shader_data, sizeof(shader_data))
//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

void RendermeshSSBO::ReadBuffer() {

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); //essentially: wait until ready.

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	gpuMem = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	if (gpuMem == NULL)
		throw(std::exception("GPU Memory couldnt be mapped!"));

	memcpy(_mesh->Faces, gpuMem, sizeof(Triangle));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TransformSSBO::MapBufferToAdressSpace(GLuint programID) {

	_bindingIndex = glGetProgramResourceIndex(programID, GL_SHADER_STORAGE_BLOCK, _bindingName);

	//GLuint ssbo_binding_point_index = 2;
	//glShaderStorageBlockBinding(programID, _bindingIndex, TRANSFORM_SSBO_BINDING_POINT);


	//glShaderStorageBlockBinding(programID, _bindingIndex, 80);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);
}

void RendermeshSSBO::MapBufferToAdressSpace(GLuint programID) {

	_bindingIndex = glGetProgramResourceIndex(programID, GL_SHADER_STORAGE_BLOCK, _bindingName);

	//GLuint ssbo_binding_point_index = 2;
	//glShaderStorageBlockBinding(programID, _bindingIndex, RENDERMESH_SSBO_BINDING_POINT);


	//glShaderStorageBlockBinding(programID, _bindingIndex, 80);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _SSBO);
}

std::string RendermeshSSBO::DebugGiveVector() {
	glm::vec3 temp1 = _mesh->Faces->Edge1;
	glm::vec3 temp2 = _mesh->Faces->Edge2;

	std::string out = "\n";
	out += glm::to_string(temp1);
	out += "\n";
	out += glm::to_string(temp2);

	return out;
}



void TransformSSBO::FillBuffer(glm::mat4 projection, glm::mat4 view, glm::mat4 model) {

	_packData(projection, view, model);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrices), _mat, GL_DYNAMIC_DRAW);

	gpuMem = (Matrices*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Matrices), GL_MAP_WRITE_BIT);

	if (gpuMem == NULL)
		throw(std::exception("GPU Memory couldnt be mapped!"));

	memcpy(gpuMem, _mat, sizeof(Matrices));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TransformSSBO::ReadBuffer() {

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); //essentially: wait until ready.

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
	gpuMem = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Matrices), GL_MAP_READ_BIT);

	if (gpuMem == NULL)
		jLog::Instance()->Error("Could not map shader log into client's memory space for reading.");
	else
		memcpy(_mat, gpuMem, sizeof(_mat));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
 }


void TransformSSBO::_packData(glm::mat4 projection, glm::mat4 view, glm::mat4 model) {
	_mat->projection = projection;
	_mat->view = view;
	_mat->model = model;
}

std::string TransformSSBO::DebugGiveVector() {
	glm::vec3 temp = _mat->view[0];
	return glm::to_string(temp);

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
