#ifndef TARGET_H
#define TARGET_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

class Target {
public:
    glm::vec2 pos, vel;
    float size;
    Target(glm::vec2 pos, glm::vec2 vel, float size);
    Target(glm::vec2 pos, glm::vec2 vel) : Target(pos, vel, 0.07f) {}
    Target(glm::vec2 pos) : Target(pos, glm::vec2(0.0f) - pos) {}
    Target() : Target(glm::vec2(0.0f)) {}
    ~Target();
    void draw();
    void move(float dt);
private:
    GLuint VAO, VBO;
    void updateVBO() {
        float vertices[] = {
            pos.x - size, pos.y - size,
            pos.x - size, pos.y + size,
            pos.x + size, pos.y - size,
            pos.x + size, pos.y + size,
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    }
};
#endif
