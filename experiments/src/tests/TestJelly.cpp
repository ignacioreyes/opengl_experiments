#include "TestJelly.h"
#include <GL/glew.h>
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <cmath>


#define VerticesPerSide 100
#define TileSize (1.0f/99.0f)
#define kSpring 0.7f
#define damping 0.00006f

struct Force{
    float forceX;
    float forceY;
};

inline int get_vertex_1d_index(int x, int y)
{
    return y*VerticesPerSide + x;
}

Force ComputeForce(int x, int y, int dx, int dy, float* position_grid){
    int other_particle_x = x + dx;
    if ((other_particle_x < 0) || (other_particle_x >= VerticesPerSide)){
        return Force{0.0f, 0.0f};
    }
        
    int other_particle_y = y + dy;
    if ((other_particle_y < 0) || (other_particle_y >= VerticesPerSide)){
        return Force{0.0f, 0.0f};
    }

    float natural_length;
    if (dx * dy == 0)
    {
        natural_length = TileSize;
    }
    else 
    {
        natural_length = 1.4142 * TileSize;
    }
    
    float ThisXPosition = position_grid[get_vertex_1d_index(x, y)*2 + 0];
    float ThisYPosition = position_grid[get_vertex_1d_index(x, y)*2 + 1];

    float OtherXPosition = position_grid[
        get_vertex_1d_index(other_particle_x, other_particle_y)*2 + 0];
    float OtherYPosition = position_grid[
        get_vertex_1d_index(other_particle_x, other_particle_y)*2 + 1];
        
    float deltaX = ThisXPosition - OtherXPosition;
    float deltaY = ThisYPosition - OtherYPosition;
    
    float mag_distance = std::sqrt(deltaX*deltaX+deltaY*deltaY);
    float mag_force = (mag_distance - natural_length) * kSpring;
    return Force{
        -deltaX * mag_force / mag_distance,
        -deltaY * mag_force / mag_distance
    };
}

namespace test {
    TestJelly::TestJelly()
        {   
        m_VertexPositions = (float*) malloc(sizeof(float)*2*VerticesPerSide*VerticesPerSide);
        m_OldVertexPositions = (float*) malloc(
            sizeof(float)*2*VerticesPerSide*VerticesPerSide);
                
        ResetPositions();

        unsigned int floor_tile_indices[3*2*(VerticesPerSide-1)*(VerticesPerSide-1)];

        // Define the triangles
        for (int y = 0; y < VerticesPerSide-1; y++)
        {
             for (int x = 0; x < VerticesPerSide-1; x++)
            {
                int base_index = (VerticesPerSide-1)*y + x;
                floor_tile_indices[6*base_index + 0] = get_vertex_1d_index(x, y);
                floor_tile_indices[6*base_index + 1] = get_vertex_1d_index(x+1, y);
                floor_tile_indices[6*base_index + 2] = get_vertex_1d_index(x, y+1);

                floor_tile_indices[6*base_index + 3] = get_vertex_1d_index(x+1, y);
                floor_tile_indices[6*base_index + 4] = get_vertex_1d_index(x+1, y+1);
                floor_tile_indices[6*base_index + 5] = get_vertex_1d_index(x, y+1); 
            }
        }        

        m_Shader = std::make_unique<Shader>("res/shaders/Jelly.shader");
        VertexBufferLayout layout;
        layout.Push(2);  // 2D positions

        m_FloorVertexArray = std::make_unique<VertexArray>();
        m_FloorVertexBuffer = std::make_unique<VertexBuffer>(
            m_VertexPositions, 
            sizeof(float)*2*VerticesPerSide*VerticesPerSide,
            GL_DYNAMIC_DRAW
        );
        m_FloorVertexArray->AddBuffer(*m_FloorVertexBuffer, layout);
        m_FloorIndexBuffer = std::make_unique<IndexBuffer>(
            floor_tile_indices, sizeof(floor_tile_indices) / sizeof(float));

        m_Proj = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -100.0f, 100.0f);
        m_View = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(0.0f, 0.0f, 0.0f));

        float modelScale = 600.0f;
        m_Model = glm::scale(
            glm::mat4(1.0f),
            glm::vec3(modelScale, modelScale, modelScale));
        m_Model = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(600.0f, 400.0f, 0.0f)) * m_Model;

        m_Shader->Bind();
        m_Shader->SetUniform1f(
            "u_TriangleNominalSize", 
            std::pow(TileSize, 2.0f)/2.0f);

        glm::mat4 mvp = m_Proj * m_View * m_Model; 
        m_Shader->SetUniformMat4f("u_MVP", mvp);
    }

    TestJelly::~TestJelly(){
        free(m_VertexPositions);
        free(m_OldVertexPositions);
    }

    void TestJelly::UpdatePositions(float deltaTime){
        float force_array[2*VerticesPerSide*VerticesPerSide]; 
        for(int y = 0; y<VerticesPerSide; y++)
        {
            for (int x = 0; x < VerticesPerSide; x++)
            {
                Force aux_force;

                int force_index_x = get_vertex_1d_index(x, y)*2 + 0;
                int force_index_y = get_vertex_1d_index(x, y)*2 + 1;

                float force_x = 0.0f;
                float force_y = 0.0f;

                aux_force = ComputeForce(
                    x, y, -1, -1, m_VertexPositions);
                force_x += aux_force.forceX; 
                force_y += aux_force.forceY;

                aux_force = ComputeForce(
                    x, y, 0, -1, m_VertexPositions);
                force_x += aux_force.forceX; 
                force_y += aux_force.forceY;

                aux_force = ComputeForce(
                    x, y, 1, -1, m_VertexPositions);
                force_x += aux_force.forceX; 
                force_y += aux_force.forceY;

                aux_force = ComputeForce(
                    x, y, -1, 0, m_VertexPositions);
                force_x += aux_force.forceX; 
                force_y += aux_force.forceY;

                aux_force = ComputeForce(
                    x, y, 1, 0, m_VertexPositions);
                force_x += aux_force.forceX; 
                force_y += aux_force.forceY;

                aux_force = ComputeForce(
                    x, y, -1, 1, m_VertexPositions);
                force_x += aux_force.forceX; 
                force_y += aux_force.forceY;

                aux_force = ComputeForce(
                    x, y, 0, 1, m_VertexPositions);
                force_x += aux_force.forceX; 
                force_y += aux_force.forceY;

                aux_force = ComputeForce(
                    x, y, 1, 1, m_VertexPositions);
                force_x += aux_force.forceX; 
                force_y += aux_force.forceY;

                // Damping
                float x_speed = (
                    m_VertexPositions[force_index_x] 
                    - m_OldVertexPositions[force_index_x]) / deltaTime;

                float y_speed = (
                    m_VertexPositions[force_index_y] 
                    - m_OldVertexPositions[force_index_y]) / deltaTime;

                force_x += -x_speed * damping; 
                force_y += -y_speed * damping;

                force_array[force_index_x] = force_x; 
                force_array[force_index_y] = force_y;
            }
        }

        float new_position_grid[2*VerticesPerSide*VerticesPerSide];

        float dt_squared_over_dm = std::pow(deltaTime * VerticesPerSide, 2.0f);
        for (int y = 0; y < 2*VerticesPerSide*VerticesPerSide; y++)
        {
            new_position_grid[y] = (
                2.0f * m_VertexPositions[y] 
                - m_OldVertexPositions[y]
                + force_array[y] * dt_squared_over_dm);
        }

        memcpy(
            m_OldVertexPositions, 
            m_VertexPositions, 
            sizeof(float)*2*VerticesPerSide*VerticesPerSide);

        memcpy(
            m_VertexPositions, 
            &new_position_grid, 
            sizeof(float)*2*VerticesPerSide*VerticesPerSide);
    }

    void TestJelly::ResetPositions(){
        // Set the positions of the vertices
        for (int y = 0; y < VerticesPerSide; y++)
        {
            for (int x = 0; x < VerticesPerSide; x++)
            {
                m_VertexPositions[get_vertex_1d_index(x, y)*2 + 0] = (TileSize * x - 0.5f) * 1.0f;
                m_VertexPositions[get_vertex_1d_index(x, y)*2 + 0] += (
                    15.0f * TileSize * x * std::abs(y - VerticesPerSide/2.0f) / std::pow(VerticesPerSide, 2.0f));
                
                m_VertexPositions[get_vertex_1d_index(x, y)*2 + 1] = (TileSize * y - 0.5f) * 0.92f;
            }
        }

        memcpy(
            m_OldVertexPositions, 
            m_VertexPositions, 
            sizeof(float)*2*VerticesPerSide*VerticesPerSide);   

    }

    void TestJelly::OnUpdate(float deltaTime){
        m_Shader->Bind();
        int positionUpdatesPerFrame = 8;
        for (int i = 0; i < positionUpdatesPerFrame; i++)
        {
            UpdatePositions(1.0f/(60.0f*positionUpdatesPerFrame));
        }
    }

    void TestJelly::OnRender(){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_FloorVertexBuffer->Update(
            m_VertexPositions,
            sizeof(float)*2*VerticesPerSide*VerticesPerSide);
        
        Renderer renderer;
        renderer.Draw(*m_FloorVertexArray, *m_FloorIndexBuffer, *m_Shader);
    }

    void TestJelly::OnImGuiRender(){
        // ImGui::SliderFloat("View: angle", &m_Angle, 0.0f, 6.28f);
        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (ImGui::Button("Reset position")) {
            ResetPositions();
        }
    }

}