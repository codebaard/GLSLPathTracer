#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D ScreenTexture;

void main(){

    //HDR ToneMapping via Reinhard operator
    vec3 ToneMapping = texture(ScreenTexture, TexCoords).rgb;
    ToneMapping = ToneMapping / (ToneMapping + vec3(1.0));

    //Gamma 2.2
    float GammaExp = 2.2;
    vec3 Gamma = pow(ToneMapping, vec3(1.0/GammaExp));

    vec4 Result = vec4(Gamma, 1.0);

    FragColor = Result;
}