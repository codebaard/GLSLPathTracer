#include <Texture.h>

Texture::Texture(const char* filepath, unsigned int aiTextureType, std::string typeName) {
	jLog::Instance()->Log(INFO, "Creating new Texture.");

	//set values for defined behavior
	_width = 0;
	_height = 0;
	_nrChannels = 0;
	_data = NULL;
	_textureID = 0;
	_glTextureObject = 0;
	_clrChannels = 0;

	TextureType = typeName;

	//FileName = filepath;
	std::string tmp = filepath;
	std::size_t found = tmp.find_last_of("\\");
	FileName = tmp.substr(found+1);

	jLog::Instance()->Log(INFO, "Trying to load Texture...");
	_data = stbi_load(filepath, &_width, &_height, &_nrChannels, 0); // 0 -> All channels in file

	if (_data) {
		jLog::Instance()->Log(INFO, "Texture loaded successfully!");
		_determineClrChannels();
		IsLoaded = true;
	}
	else {
		throw std::string("Texture file not found or mesh does not have texture attached.");
	}

	_setUniformParams(aiTextureType, typeName);
	_configGLTexture();

}

Texture::Texture(unsigned int aiTextureType, std::string typeName, Texture* loadedTex) {
	jLog::Instance()->Log(INFO, "Creating new Texture from existing.");

	//set values for defined behavior
	_width = loadedTex->_width;
	_height = loadedTex->_height;
	_nrChannels = loadedTex->_nrChannels;
	_data = NULL;
	_textureID = loadedTex->_textureID;
	_glTextureObject = loadedTex->_glTextureObject;
	_setUniformParams(aiTextureType, typeName);

	_clrChannels = loadedTex->_clrChannels;
	FileName = loadedTex->FileName;

	TextureType = typeName;

	IsLoaded = true;
}

void Texture::FlipTexturesVertically() {
	stbi_set_flip_vertically_on_load(true);
}

unsigned int Texture::_determineClrChannels() {
	if (!_nrChannels) {
		jLog::Instance()->Log(WARNING, "No ColorChannel information found. Texture not loaded correctly!");
		return 0;
	}

	switch (_nrChannels)
	{
	case 3:
		_clrChannels = GL_RGB;
		break;
	case 4:
		_clrChannels = GL_RGBA;
		break;
	default:
		_clrChannels = 0;
		break;
	}
	return 1;
}



void Texture::_setUniformParams(unsigned int aiTextureType, std::string uniformName) {
	switch (aiTextureType) {
	case 1: 
		UniformID = DIFFUSE_MAP; 
		_glTextureObject = GL_TEXTURE0;
		break;
	case 2:
		UniformID = SPECULAR_MAP; 
		_glTextureObject = GL_TEXTURE1;
		break;
	case 3: 
		UniformID = AMBIENT_MAP; 
		_glTextureObject = GL_TEXTURE2;
		break;
	case 4: 
		UniformID = EMISSIVE_MAP; 
		_glTextureObject = GL_TEXTURE3;
		break;
	case 6:
		UniformID = NORMAL_MAP;
		_glTextureObject = GL_TEXTURE4;
		break;
	case 12: //albedo
		UniformID = DIFFUSE_MAP;
		_glTextureObject = GL_TEXTURE0;
		break;
	case 13:
		UniformID = NORMAL_MAP;
		_glTextureObject = GL_TEXTURE4;
		break;
	case 14:
		UniformID = EMISSIVE_MAP;
		_glTextureObject = GL_TEXTURE3;
		break;
	case 16: //roughness
		UniformID = SPECULAR_MAP;
		_glTextureObject = GL_TEXTURE1;
		break;
	case 17: //AO
		UniformID = AMBIENT_MAP;
		_glTextureObject = GL_TEXTURE2;
		break;
	}
	UniformName = uniformName;
}

void Texture::_configGLTexture() {

	glGenTextures(1, &_textureID);

	glActiveTexture(_glTextureObject);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	// Optional: Put this in seperate function (init-style) with enum for options
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, _clrChannels, _width, _height, 0, _clrChannels, GL_UNSIGNED_BYTE, _data);
	glGenerateMipmap(GL_TEXTURE_2D);

	//clean up after use
	stbi_image_free(_data);
	

	jLog::Instance()->Log(INFO, "gl-Texture built successfully.");
}

void Texture::ActivateTexture() {

	glActiveTexture(_glTextureObject);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::DeactivateTexture() {

	glActiveTexture(0);
}

void Texture::SetGLParams(unsigned int id, unsigned int glTextureUnit) {
	_textureID = id;
	_glTextureObject = glTextureUnit;
}

unsigned int Texture::GiveGLTextureUnit() {
	return _glTextureObject;
}

unsigned int Texture::GiveGlTextureID() {
	return _textureID;
}