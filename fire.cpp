#include "fire.h"

Fire::Fire(glm::vec2 position, glm::vec2 velocity, float size) : pos(position), vel(velocity), size(size) {
    // Setup VAO, VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    updateVBO();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

Fire::~Fire() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Fire::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool Fire::update(float dt) {
    pos += vel * dt;    // move
    size -= 0.03 * dt;  // shrink
    updateVBO();
    return size <= 0;
}
