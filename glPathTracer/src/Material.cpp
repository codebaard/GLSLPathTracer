#include <Material.h>

pbrMaterial* mtlMaterial::TransformToPBR() {
	pbrMaterial* output = new pbrMaterial();

	output->MaterialName = this->MaterialName;

	output->Albedo = this->Kd; //not entirely correct but close enough
	output->IOR = this->Ni;
	output->Tr = this->d;
	output->Roughness = (-1.0f * (1.0f / 900.0f) * this->Ns) + 1; //this function is not correct but close. Non-Linear-Error max 25%!

	output->AlbedoMap = this->Diffuse; //not entirely correct but close enough
	output->AlbedoMap.UniformID = this->Diffuse.UniformID;
	output->AlbedoMap.UniformName = "AlbedoMap";
	output->AlbedoMap.SetGLParams(this->Diffuse.GiveGlTextureID(), this->Diffuse.GiveGLTextureUnit());

	output->RoughnessMap = this->Specular; //rougness = specular*-1! --> Convert!
	output->RoughnessMap.UniformID = this->Specular.UniformID;
	output->RoughnessMap.UniformName = "RoughnessMap";
	output->RoughnessMap.SetGLParams(this->Specular.GiveGlTextureID(), this->Specular.GiveGLTextureUnit());

	output->AOMap = this->Ambient;
	output->AOMap.UniformID = this->Ambient.UniformID;
	output->AOMap.UniformName = "AOMap";
	output->AOMap.SetGLParams(this->Ambient.GiveGlTextureID(), this->Ambient.GiveGLTextureUnit());

	output->Normal = this->Normal;
	output->Normal.UniformID = this->Normal.UniformID;
	output->Normal.UniformName = this->Normal.UniformName;
	output->Normal.SetGLParams(this->Normal.GiveGlTextureID(), this->Normal.GiveGLTextureUnit());

	//Blender mtl export makes differentiation over illum-model
	switch (this->illum) {
	case 1:
		output->Metallic = 0;

		if (output->Roughness <= 0.5) { //ASSIMP Bug -> Workaround! https://github.com/assimp/assimp/issues/3351#issue-669607350
			output->Metallic = 1;
		}
		break;
	case 2:
		output->Metallic = 0;
		break;
		
	case 3:
		output->Metallic = 1;
		break;
	}

	return output;
}