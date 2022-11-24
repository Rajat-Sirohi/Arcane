#include "bolt.h"

Bolt::Bolt(glm::vec2 direction, glm::vec2 origin, float segment_length) : origin(origin), seg_len(segment_length) {
    // Normalize direction vector
    dir = glm::normalize(direction);

    // Initialize points
    vertices.push_back(origin.x);
    vertices.push_back(origin.y);

    // Setup VAO, VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

Bolt::~Bolt() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Bolt::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size()/2);
}

void Bolt::grow() {
    glm::vec2 oldPos(vertices.end()[-2], vertices.end()[-1]);
    glm::vec2 noise(2*RAND_F - 1, 2*RAND_F - 1);
    glm::vec2 dPos = glm::normalize(dir + noise) * seg_len;
    glm::vec2 newPos = oldPos + dPos;
    vertices.push_back(newPos.x);
    vertices.push_back(newPos.y);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
}

bool Bolt::outOfBounds() {
    float x = vertices.end()[-2];
    float y = vertices.end()[-1];
    return (x <= -1) || (x >= 1) || (y <= -1) || (y >= 1);
}

glm::vec2 Bolt::getPos() {
    float x = vertices.end()[-2];
    float y = vertices.end()[-1];
    return glm::vec2(x,y);
}
