#include "target.h"

Target::Target(glm::vec2 position, glm::vec2 velocity, float size) : pos(position), vel(velocity), size(size) {
    // Setup VAO, VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    updateVBO();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

Target::~Target() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Target::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Target::move(float dt) {
    pos += vel * dt;

    if (pos.x - size <= -1) {
        pos.x = -1 + size;
        vel.x *= -1;
    } else if (pos.x + size >= 1) {
        pos.x = 1 - size;
        vel.x *= -1;
    }

    if (pos.y - size <= -1) {
        pos.y = -1 + size;
        vel.y *= -1;
    } else if (pos.y + size >= 1) {
        pos.y = 1 - size;
        vel.y *= -1;
    }

    updateVBO();
}
