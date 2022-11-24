#ifndef FIRE_H
#define FIRE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

class Fire {
public:
    glm::vec2 pos, vel;
    float size;
    Fire(glm::vec2 pos, glm::vec2 vel, float size);
    Fire(glm::vec2 pos, glm::vec2 vel) : Fire(pos, vel, 0.1f) {}
    Fire(glm::vec2 pos) : Fire(pos, glm::vec2(0.0f)) {}
    Fire() : Fire(glm::vec2(0.0f, 0.0f)) {}
    ~Fire();
    void draw();
    bool update(float dt);
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
