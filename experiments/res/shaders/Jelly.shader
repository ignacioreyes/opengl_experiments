#shader vertex
#version 460 core

layout(location = 0) in vec4 position;

void main(){
    gl_Position = position;
};

#shader geometry
#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform float u_TriangleNominalSize;
uniform mat4 u_MVP;

out vec4 v_TriangleColor;

void main(){
    float distance_1 = distance(
        gl_in[0].gl_Position,
        gl_in[1].gl_Position
    );
    float distance_2 = distance(
        gl_in[1].gl_Position,
        gl_in[2].gl_Position
    );
    float distance_3 = distance(
        gl_in[2].gl_Position,
        gl_in[0].gl_Position
    );

    float semiperimeter = (distance_1 + distance_2 + distance_3) / 2;
    float area = sqrt(
        semiperimeter
        *(semiperimeter-distance_1)
        *(semiperimeter-distance_2)
        *(semiperimeter-distance_3));

    float intensity = clamp(area/u_TriangleNominalSize - 0.2, 0.0, 1.0);
    v_TriangleColor = vec4(1.0, 0.2, 0.4, 1.0) * intensity;

    gl_Position = u_MVP * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = u_MVP * gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = u_MVP * gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_TriangleColor;

void main(){
    color = v_TriangleColor;
};