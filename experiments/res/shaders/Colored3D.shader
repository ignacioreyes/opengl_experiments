#shader vertex
#version 460 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
out vec3 v_Normal;
out vec3 v_FragPos;

void main(){
    gl_Position = u_Proj * u_View * u_Model * a_Position;
    v_Normal = a_Normal;
    v_FragPos = vec3(u_Model * a_Position);
};


#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragPos;
uniform vec3 u_ModelColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;

void main(){
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;
    vec3 ambient = 0.1 * u_LightColor;
    color = vec4((diffuse + ambient) * u_ModelColor, 1.0);
};