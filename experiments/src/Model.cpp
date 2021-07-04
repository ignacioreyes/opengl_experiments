#include "Model.h"

#include <fstream>
#include <sstream>
#include <iostream>


Model::Model(const std::string& filepath){
    std::ifstream stream(filepath);
    std::string line;

    enum class ParserState{
        PREHEADER = 0,
        VERTEX_HEADER = 1,
        FACE_HEADER = 2,
        VERTICES = 3,
        FACES = 4
    };
    ParserState state = ParserState::PREHEADER;

    int n_vertices;
    int n_faces;

    int loaded_vertices = 0;
    int loaded_faces = 0;

    VertexBufferLayout layout;
    float* vertices;
    unsigned int* indices;

    while(getline(stream, line)){
        if (line.find("element vertex") != std::string::npos){
            state = ParserState::VERTEX_HEADER;
            std::string str_number = line.substr(15);
            n_vertices = std::stoi(str_number);

            layout.Push(3);
            layout.Push(3);
            layout.Push(2);

            vertices = (float*) malloc(sizeof(float) * 8 * n_vertices);
            continue;
        }
        if (line.find("element face") != std::string::npos){
            state = ParserState::FACE_HEADER;
            std::string str_number = line.substr(13);
            n_faces = std::stoi(str_number);
            indices = (unsigned int*) malloc(sizeof(unsigned int) * 3 * n_faces);
            continue;
        }
        if (line.find("end_header") != std::string::npos){
            state = ParserState::VERTICES;
            continue;
        }
            
        if (state == ParserState::VERTEX_HEADER){
            // layout is hard-coded
            continue;
        }
        if (state == ParserState::FACE_HEADER){
            // hard-coded
            continue;
        }
        if (state == ParserState::VERTICES){
            std::stringstream ss(line);
            std::string str_number;
            int i = 0;
            while (ss >> str_number){
                vertices[loaded_vertices*8 + i] = std::stof(str_number);
                i++;
            }

            loaded_vertices++;
            if (loaded_vertices == n_vertices){
                state = ParserState::FACES;
                continue;
            }
        }
        if (state == ParserState::FACES){
            std::stringstream ss(line);
            std::string str_number;

            ss >> str_number;  // ignore the first number

            int i = 0;
            while (ss >> str_number){
                indices[loaded_faces*3 + i] = std::stoi(str_number);
                i++;
            }

            loaded_faces++;
            if (loaded_faces == n_faces){
                break;
            }
        }
    }

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, n_vertices*8*sizeof(float));
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3*n_faces);

    free(vertices);
    free(indices);
};

Model::~Model() {};

void Model::Draw(Renderer& renderer, Shader& shader){
    renderer.Draw(*m_VertexArray, *m_IndexBuffer, shader);  
};