#include <Model.h>

void Model::Draw(RenderShader& shader) {

	for (Mesh mesh : _meshes) {
		mesh.Draw(shader);
	}
}

void Model::_loadModel(std::string path) {
	Assimp::Importer importer;

	jLog::Instance()->Log(INFO, "Loading Scene...");

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		jLog::Instance()->Error("Assimp: Error loading scene!");
		jLog::Instance()->Error(importer.GetErrorString());
		throw std::exception("Error no file.");
		return;
	}
	_directory = path.substr(0, path.find_last_of('\\'));

	jLog::Instance()->Log(INFO, "Processing nodes in File...");
	_processNode(scene->mRootNode, scene); //recursive call!
	jLog::Instance()->Log(INFO, "Node processing successful.");

	jLog::Instance()->Log(INFO, "Model successfully loaded.");
}

void Model::_processNode(aiNode* node, const aiScene* scene) {
	
	//process all meshes in the node
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(_processMesh(mesh, scene));
	}

	//and their children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		_processNode(node->mChildren[i], scene);
	}
}

Mesh Model::_processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	mtlMaterial* material = new mtlMaterial();

	//vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		//positions
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		//normals
		if (mesh->mNormals) {
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}
		else
			vertex.Normal = glm::vec3(0.0f);

		//Texture coordinates
		if (mesh->mTextureCoords[0]) { //are there any texture coordinates?
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f);

		//done
		vertices.push_back(vertex);
	}

	//process mesh indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		//aiFace face = mesh[i].mFaces[i];
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0 ; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//process materials
	if (mesh->mMaterialIndex >= 0) {
		aiColor3D color;
		aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];
		
		aiString name;
		aimaterial->Get(AI_MATKEY_NAME, name);
		material->MaterialName = name.C_Str();

		ai_int illumModel;
		aimaterial->Get(AI_MATKEY_SHADING_MODEL, illumModel);
		material->illum = illumModel-1; //Library is 1-based. (3rd Model in List == illum Model 2)

		aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->Ka = glm::vec4(color.r, color.g, color.b, 1.0);
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->Kd = glm::vec4(color.r, color.g, color.b, 1.0);
		aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->Ks = glm::vec4(color.r, color.g, color.b, 1.0);
		aimaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->Ke = glm::vec4(color.r, color.g, color.b, 1.0);

		aimaterial->Get(AI_MATKEY_SHININESS, material->Ns);
		aimaterial->Get(AI_MATKEY_OPACITY, material->d);
		aimaterial->Get(AI_MATKEY_REFRACTI, material->Ni);

		//check both mtl and pbr workflow!
		material->Ambient = _loadMaterialTextures(aimaterial, aiTextureType_AMBIENT, "AmbientMap"); 
		if(!material->Ambient.IsLoaded)		
			material->Ambient = _loadMaterialTextures(aimaterial, aiTextureType_AMBIENT_OCCLUSION, "AmbientMap");
		
		material->Diffuse = _loadMaterialTextures(aimaterial, aiTextureType_DIFFUSE, "DiffuseMap");
		if (!material->Diffuse.IsLoaded)	
			material->Diffuse = _loadMaterialTextures(aimaterial, aiTextureType_BASE_COLOR, "DiffuseMap");

		material->Specular = _loadMaterialTextures(aimaterial, aiTextureType_SPECULAR, "SpecularMap");
		if(!material->Specular.IsLoaded)	
			material->Specular = _loadMaterialTextures(aimaterial, aiTextureType_DIFFUSE_ROUGHNESS, "SpecularMap");
		
		material->Emissive = _loadMaterialTextures(aimaterial, aiTextureType_EMISSIVE, "EmissiveMap");		
		if(!material->Emissive.IsLoaded)
			material->Emissive = _loadMaterialTextures(aimaterial, aiTextureType_EMISSION_COLOR, "EmissiveMap");
		
		material->Normal = _loadMaterialTextures(aimaterial, aiTextureType_NORMALS, "NormalMap");
		if(!material->Normal.IsLoaded)
			material->Normal = _loadMaterialTextures(aimaterial, aiTextureType_NORMAL_CAMERA, "NormalMap");
	}

	return Mesh(vertices, indices, material);
}

Texture Model::_loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {

	Texture* newTex;
	aiString str;

	mat->GetTexture(type, 0, &str);

	//check if texture already loaded to prevent loading the same texture several times
	for (unsigned int j = 0; j < _loadedTextures.size(); j++) {
		if (std::strcmp(_loadedTextures[j].FileName.data(), str.C_Str()) == 0) {
			newTex = new Texture(type, typeName, &_loadedTextures[j]); //new Texture is internal reference to existing
			return *newTex;
		}
	}

	try { //in case texture does not exist
		std::string fqPath = _directory + "\\" + str.C_Str();
		newTex = new Texture(fqPath.c_str(), type, typeName);
		if (newTex->IsLoaded) {
			_loadedTextures.push_back(*newTex);
		}
	}

	catch (std::string msg) {
		jLog::Instance()->Log(WARNING, msg.c_str());
		newTex = new Texture(); //in case loading fails, texture object is intentionally initialized but empty.
	}

	return *newTex; 
}