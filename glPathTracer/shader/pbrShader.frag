#version 430 core
out vec4 FragColor;


in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} fs_in;

struct pbrPointLight {
    vec3 Position;
    vec3 LightColor;
};

struct pbrMaterial {
    vec3 Albedo;

    float Metallic;
    float Roughness;
    float IOR;
    float Transparency;

    sampler2D AlbedoMap;
    sampler2D RoughnessMap;
    sampler2D AOMap;
    sampler2D EmissiveMap;
    sampler2D BumpMap;

    vec3 F0; //0.04 by default

    //if textures applied -> use them!
    bool hasAlbedoMap;
    bool hasRoughnessMap;
    bool hasBumpMap;
    bool hasAmbientOcclusion;
};

uniform vec3 camPos;
const float PI = 3.14159265359;

uniform pbrPointLight pointLight;
uniform pbrMaterial material;

vec3 InterpolateMetalTint(pbrMaterial mat);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

void main()
{    

    vec3 N = vec3(0.0);
    if(material.hasBumpMap){
        //apply normal mapping to normal vectors from texture
        N = texture(material.BumpMap, fs_in.TexCoords).rgb;
        N = normalize(N * 2.0 - 1.0);
    }
    else {
        N = normalize(fs_in.Normal);
    }

    vec3 V = normalize(camPos - fs_in.FragPos);

    vec3 F0 = InterpolateMetalTint(material);

    //### reflectance equation ### 
    //Light Radiance per Fragment (w_i)
    vec3 L0 = vec3(0.0);
    vec3 L = normalize(pointLight.Position - fs_in.FragPos);
    vec3 H = normalize(V + L);

    float Distance = length(pointLight.Position - fs_in.FragPos);
    float Attenuation = 1.0 / (Distance * Distance); //quadratic decay (more precise than polynomial model)
    vec3 Radiance = pointLight.LightColor * Attenuation;

    //BRDF
    float NDF = DistributionGGX(N, H, material.Roughness);
    float G = GeometrySmith(N, V, L, material.Roughness);
    vec3 F  = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - material.Metallic; //if metallic, reduce diffuse reflection

    vec3 nominator = NDF*G*F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001); //prevent DIV/0

    //outgoing Radiance from Material
    float NdotL = max(dot(N,L), 0.0);

    vec3 Lo = vec3(0.0);
    vec3 ambient = vec3(0.0);


    if(material.hasAlbedoMap){
        ambient = vec3(0.03) * texture(material.AlbedoMap, fs_in.TexCoords).rgb;
        Lo = (kD * texture(material.AlbedoMap, fs_in.TexCoords).rgb / PI + specular) * Radiance * NdotL;

    }
    else{
        Lo = (kD * material.Albedo / PI + specular) * Radiance * NdotL;
        ambient = vec3(0.03) * material.Albedo;
    }

    vec3 Result = ambient + Lo;

    //gamma + HDR
//    vec3 color = Result / (Result + vec3(1.0));
//    color = pow(color, vec3(1.0/2.2));
    
    //Done!
    FragColor = vec4(Result, 1.0);
    //FragColor = vec4(color, 1.0);

}

//in case the material is metal and has a tint (Gold, Copper, ...), this interpolates the tint.
vec3 InterpolateMetalTint(pbrMaterial mat){
    return mix(mat.F0, mat.Albedo, mat.Metallic);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  

float DistributionGGX(vec3 N, vec3 H, float roughness){

    // in contrast to theory it looks more realistic to square the roughness value
    // the purpose of this renderer is to create beautiful pictures
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0); //--> Presumably wrong!
    denom = PI * denom * denom;

    return (nom/denom);
}

float GeometrySchlickGGX(float NdotV, float roughness){


    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0-k) + k;
    
    return (nom/denom);

}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){

    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return (ggx1 * ggx2);
}