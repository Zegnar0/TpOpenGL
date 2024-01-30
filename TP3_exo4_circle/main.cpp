#include "p6/p6.h"
#include "glimac/default_shader.hpp"
#include "glm/glm.hpp"
#include <cmath>


struct Vertex2DColor
{
    glm::vec2 position;
    glm::vec3 color;
};

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    const p6::Shader shader = p6::load_shader(
    "shaders/triangle.vs.glsl",
    "shaders/triangle.fs.glsl"
    );

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    int NumbTr = 100;
   
    std::vector<Vertex2DColor> vertices = {};

     for (int i = 0; i < NumbTr; i++)
    {
        vertices.push_back(Vertex2DColor{{0.5f*cos(i*2*M_PI/NumbTr), 0.5f*sin(i*2*M_PI/NumbTr)}, {1.f, 0.f, 0.f}});
        vertices.push_back(Vertex2DColor{{0.5f*cos((i+1)*2*M_PI/NumbTr), 0.5f*sin((i+1)*2*M_PI/NumbTr)}, {0.f, 1.f, 0.f}});
        vertices.push_back(Vertex2DColor{{0.f, 0.f}, {0.f, 0.f, 1.f}});
    }

    glBufferData(GL_ARRAY_BUFFER,NumbTr*3*sizeof(Vertex2DColor),vertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    static constexpr GLuint vertex_attr_position = 3;
    static constexpr GLuint vertex_attr_color = 8;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_color);


    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, position)));
    glVertexAttribPointer(vertex_attr_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor),(const GLvoid*)(offsetof(Vertex2DColor, color)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        shader.use();
        glDrawArrays(GL_TRIANGLES,0,NumbTr*3);
        // glimac::bind_default_shader();
        glBindVertexArray(0);
        


    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}