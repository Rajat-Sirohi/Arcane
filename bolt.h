#ifndef BOLT_H
#define BOLT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#define RAND_F (float(std::rand()) / float(RAND_MAX))

class Bolt {
public:
    glm::vec2 dir, origin;
    float seg_len;
    Bolt(glm::vec2 dir, glm::vec2 origin, float segment_length);
    Bolt(glm::vec2 dir, glm::vec2 origin) : Bolt(dir, origin, 0.015f) {}
    Bolt(glm::vec2 dir) : Bolt(dir, glm::vec2(0.0f)) {}
    Bolt() : Bolt(glm::vec2(1.0f, 0.0f)) {}
    ~Bolt();
    void draw();
    void grow();
    bool outOfBounds();
    glm::vec2 getPos();
private:
    GLuint VAO, VBO;
    std::vector<float> vertices;
};
#endif
