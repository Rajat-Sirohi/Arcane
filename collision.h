#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

bool check_collision_point_rect(glm::vec2 point, glm::vec2 rect_pos, float rect_size) {
    float px = point.x;
    float py = point.y;
    float lx = rect_pos.x - rect_size;
    float ly = rect_pos.y - rect_size;
    float rx = rect_pos.x + rect_size;
    float ry = rect_pos.y + rect_size;
    return (px >= lx) && (px <= rx) && (py >= ly) && (py <= ry);
}

bool check_collision_rect_rect(glm::vec2 r1_pos, float r1_size, glm::vec2 r2_pos, float r2_size) {
    /*
              ---------r2
         -----|---r1    |
         |   l2---|------
        l1---------
     */
    float l1x = r1_pos.x - r1_size;
    float l1y = r1_pos.y - r1_size;
    float r1x = r1_pos.x + r1_size;
    float r1y = r1_pos.y + r1_size;
    float l2x = r2_pos.x - r2_size;
    float l2y = r2_pos.y - r2_size;
    float r2x = r2_pos.x + r2_size;
    float r2y = r2_pos.y + r2_size;

    // rectangle has zero area
    if (l1x == r1x || l1y == r1y || l2x == r2x || l2y == r2y)
        return false;

    // one rectangle on left side of other
    if (l1x > r2x || l2x > r1x)
        return false;

    // one rectangle above the other
    if (l1y > r2y || l2y > r1y)
        return false;

    return true;
}

#endif
