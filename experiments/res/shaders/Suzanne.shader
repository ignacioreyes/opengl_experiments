#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 idk;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat3 u_Normal;

out vec3 v_Normal;
out vec3 v_FragPos;

void main(){
    mat4 MVP = u_Proj * u_View * u_Model;
    gl_Position = MVP * vec4(a_Position, 1.0);
    v_Normal = u_Normal * a_Normal;
    v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
};


#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 u_ModelColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

void main(){
    // Ambient light
    float AmbientStrength = 0.1;
    vec3 AmbientLight = AmbientStrength * u_LightColor;

    // Diffuse light
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_FragPos);
    float DiffuseAttenuation = max(dot(norm, lightDir), 0.0);
    vec3 DiffuseLight = DiffuseAttenuation * u_LightColor;

    // Specular light
    float SpecularStrength = 0.7;
    vec3 ViewDirection = normalize(u_ViewPosition - v_FragPos);
    vec3 ReflectDirection = reflect(-lightDir, norm);
    float spec = pow(
        max(dot(ViewDirection, ReflectDirection), 0.0),
        32);
    vec3 SpecularLight = SpecularStrength * spec * u_LightColor;

    color = vec4((DiffuseLight + AmbientLight + SpecularLight) * u_ModelColor, 1.0);
};