#include <shader.h>

Shader::Shader() {
	_shaderList = NULL;
	ID = 0;
}

void Shader::AddShaderToPipeline(std::string cwd, const char* filename, ShaderType type) {

	try{
		ShaderProgram* newElement = new ShaderProgram();
		ShaderProgram* itr = NULL;
		ShaderProgram* prev = NULL;

		std::string path = std::string(cwd) + std::string(filename);

		newElement->filepath = path.c_str();
		newElement->type = type;
		newElement->ID = _buildShader(type, newElement->filepath.c_str());
		newElement->nextElement = NULL;

		//fill the single linked list
		if (_shaderList == NULL) {
			_shaderList = newElement;
		}
		else {
			itr = _shaderList;
			while (itr) {
				prev = itr;
				itr = itr->nextElement;
			}
			prev->nextElement = newElement;
		}

		jLog::Instance()->Log(INFO, "Shader added to Pipeline.");
	}
	catch (std::exception e) {
		throw;
	}

}

unsigned int Shader::_buildShader(ShaderType type, const char* filepath) {
		
	jLog::Instance()->Log(INFO, "Building: " + std::string(filepath));

	const char* code = "";
	unsigned int ShaderID;
	int success;
	char infoLog[INFO_LOG_SIZE];

	
	std::string ShaderCode;
	std::ifstream ShaderFile;
	ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		ShaderFile.open(filepath);
		std::stringstream ShaderStream;
		ShaderStream << ShaderFile.rdbuf();
		ShaderFile.close();
		ShaderCode = ShaderStream.str();
		jLog::Instance()->Log(INFO, "Shader file successfully read.");
	}
	catch (std::exception e) {
		throw;
	}

	switch (type) {
	case VERTEX:
		ShaderID = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT:
		ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case COMPUTE:
		ShaderID = glCreateShader(GL_COMPUTE_SHADER);
		if (_arraySize != 0)
			ShaderCode = _replace(ShaderCode);
		break;
	}

	code = ShaderCode.c_str();

	glShaderSource(ShaderID, 1, &code, NULL);
	glCompileShader(ShaderID);

	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ShaderID, INFO_LOG_SIZE, NULL, infoLog);
		std::string str("Shader Compilation failed. InfoLog:" + std::string(infoLog));
		jLog::Instance()->Error(str);
		return 0;
	}
	else {
		jLog::Instance()->Log(INFO, "Shader successfully compiled.");
		return ShaderID;
	}
}

void Shader::InitShader() {

	ShaderProgram* shader = _shaderList;
	char infoLog[INFO_LOG_SIZE];
	int success;

	jLog::Instance()->Log(INFO, "Building Shader programm...");

	ID = glCreateProgram();

	while (shader) {
		glAttachShader(ID, shader->ID);

		shader = shader->nextElement;
	}

	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(ID, INFO_LOG_SIZE, NULL, infoLog);
		std::string str = std::string("Shader Programm Compilation failed. InfoLog:\n") + std::string(infoLog);
		throw(std::exception(str.c_str()));
	}

	_cleanShaderList();

	jLog::Instance()->Log(INFO, "Shader program successfully built.");
}

void Shader::_cleanShaderList() {
	ShaderProgram* shader = _shaderList;
	ShaderProgram* prev = NULL;

	jLog::Instance()->Log(INFO, "Cleaning Shader list.");

	while (shader) {
		glDeleteShader(shader->ID);
		prev = shader;
		shader = shader->nextElement;
		delete prev;
	}

	jLog::Instance()->Log(INFO, "Shader List successfully cleaned up.");
}

void Shader::use() const {
	glUseProgram(ID);
}

void RenderShader::setLight(DirectionalLight* light) {
	setVec3fv("dirLight.direction", glm::value_ptr(light->Direction));
	setVec3fv("dirLight.ambient", glm::value_ptr(light->Ambient));
	setVec3fv("dirLight.diffuse", glm::value_ptr(light->Diffuse));
	setVec3fv("dirLight.specular", glm::value_ptr(light->Specular));
}

void RenderShader::setLight(PointLight* light) {
	setVec3fv("pointLight.Position", glm::value_ptr(light->Position));

	if (light->isPBR) {
		setVec3fv("pointLight.LightColor", glm::value_ptr(light->pbrColor));
	}
	else
	{
		setFloat("pointLight.constant", light->ConstantDecay);
		setFloat("pointLight.linear", light->LinearDecay);
		setFloat("pointLight.quadratic", light->QuadraticDecay);

		setVec3fv("pointLight.ambient", glm::value_ptr(light->Ambient));
		setVec3fv("pointLight.diffuse", glm::value_ptr(light->Diffuse));
		setVec3fv("pointLight.specular", glm::value_ptr(light->Specular));
	}

}

void RenderShader::setLight(PointLight* light, unsigned int i) {

	std::string plsName = std::string("pointLights[") + std::to_string(i) + std::string("].");

	setVec3fv(plsName + "position", glm::value_ptr(light->Position));

	setFloat(plsName + "constant", light->ConstantDecay);
	setFloat(plsName + "linear", light->LinearDecay);
	setFloat(plsName + "quadratic", light->QuadraticDecay);

	setVec3fv(plsName + "ambient", glm::value_ptr(light->Ambient));
	setVec3fv(plsName + "diffuse", glm::value_ptr(light->Diffuse));
	setVec3fv(plsName + "specular", glm::value_ptr(light->Specular));
}

void RenderShader::setLight(SpotLight* light) {
	setVec3fv("spotLight.position", glm::value_ptr(light->Position));
	setVec3fv("spotLight.direction", glm::value_ptr(light->Direction));

	setFloat("spotLight.constant", light->ConstantDecay);
	setFloat("spotLight.linear", light->LinearDecay);
	setFloat("spotLight.quadratic", light->QuadraticDecay);

	setFloat("spotLight.InnerCutOff", glm::cos(glm::radians(light->InnerCutOff)));
	setFloat("spotLight.OuterCutOff", glm::cos(glm::radians(light->OuterCutOff)));

	setVec3fv("spotLight.ambient", glm::value_ptr(light->Ambient));
	setVec3fv("spotLight.diffuse", glm::value_ptr(light->Diffuse));
	setVec3fv("spotLight.specular", glm::value_ptr(light->Specular));
}

void RenderShader::setMaterial(mtlMaterial* mat) {
	setVec3fv("material.Ka", glm::value_ptr(mat->Ka));
	setVec3fv("material.Kd", glm::value_ptr(mat->Kd));
	setVec3fv("material.Ks", glm::value_ptr(mat->Ks));
	setVec3fv("material.Ke", glm::value_ptr(mat->Ke));

	setInt("material." + mat->Diffuse.UniformName, mat->Diffuse.UniformID);
	setInt("material." + mat->Specular.UniformName, mat->Specular.UniformID);
	setInt("material." + mat->Ambient.UniformName, mat->Ambient.UniformID);
	setInt("material." + mat->Emissive.UniformName, mat->Emissive.UniformID);
	setInt("material." + mat->Normal.UniformName, mat->Normal.UniformID);

	setFloat("material.Ns", mat->Ns);
	setFloat("material.Ni", mat->Ni);
	setFloat("material.d", mat->d);
}

void RenderShader::setMaterial(pbrMaterial* mat) {
	setVec3fv("material.Albedo", glm::value_ptr(mat->Albedo));
	setVec3fv("material.F0", glm::value_ptr(mat->F0));

	//load maps
	if (mat->AlbedoMap.IsLoaded) {
		mat->AlbedoMap.ActivateTexture();
		setInt("material." + mat->AlbedoMap.UniformName, mat->AlbedoMap.UniformID);
		setBool("material.hasAlbedoMap", true);
	}
	else 
		setBool("material.hasAlbedoMap", false);

	if (mat->RoughnessMap.IsLoaded) {
		mat->RoughnessMap.ActivateTexture();
		setInt("material." + mat->RoughnessMap.UniformName, mat->RoughnessMap.UniformID);
		setBool("material.hasRoughnessMap", true);
	}
	else
		setBool("material.hasRoughnessMap", false);

	if (mat->Normal.IsLoaded) {
		mat->Normal.ActivateTexture();
		setInt("material." + mat->Normal.UniformName, mat->Normal.UniformID);
		setBool("material.hasBumpMap", true);
	}
	else
		setBool("material.hasBumpMap", false);

	if (mat->AOMap.IsLoaded) {
		mat->AOMap.ActivateTexture();
		setInt("material." + mat->AOMap.UniformName, mat->AOMap.UniformID);
		setBool("material.hasAmbientOcclusion", true);
	}
	else
		setBool("material.hasAmbientOcclusion", false);

	setFloat("material.Metallic", mat->Metallic);
	setFloat("material.Roughness", mat->Roughness);
	setFloat("material.IOR", mat->IOR);
	setFloat("material.Transparency", mat->Tr);
}

//void ComputeShader::DispatchCompute(unsigned int problemSize, unsigned int workGroups, unsigned int workItems, unsigned int dim) {
void ComputeShader::DispatchCompute(unsigned int x, unsigned int y, unsigned int z) {
		
	//ProblemSize = problemSize;
	//WorkGroups = workGroups;
	//WorkItems = workItems;

	//unsigned int x= 0, y=0, z=0;

	//switch (dim) {
	//case 1:
	//	x = problemSize;
	//	y = 0;
	//	z = 0;
	//	break;
	//case 2:


	//	break;
	//case 3:

	//	break;
	//default:
	//	x = problemSize;
	//	y = 0;
	//	z = 0;
	//	break;
	//}

	glDispatchCompute(x, y, z);
}

void ComputeShader::SetInternalArraySize(unsigned int i) {
	_arraySize = i;
}

std::string Shader::_replace(std::string sourceCode) {

	std::string repl = std::to_string(_arraySize);

	for (int i = 0; i < sourceCode.length(); i++) {
		if (sourceCode[i] == '$') {
			sourceCode.replace(i, 1, repl);
		}

	}

	return sourceCode;
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat4f(const std::string& name, float v1, float v2, float v3, float v4) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}

void Shader::setMat4fv(const std::string& name, GLfloat* trans) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, trans);
}

void Shader::setVec3f(const std::string& name, float v1, float v2, float v3) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

void Shader::setVec3fv(const std::string& name, GLfloat* vec) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, vec);
}

void Shader::setVec4f(const std::string& name, float v0, float v1, float v2, float v3) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
}

void Shader::setVec4fv(const std::string& name, GLfloat* vec) const {
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, vec);
}

