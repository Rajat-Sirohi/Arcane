#version 330 core

uniform float time;
uniform bool glow;
uniform vec3 color;

out vec4 FragColor;

void main() {
    if (glow) {
       float intensity = 2 * sin(15*time) - 1;
       intensity = intensity * 0.1 + 0.9;
       FragColor = vec4(color * intensity, 1.0f);
    } else {
       FragColor = vec4(color, 1.0f);
    }
}
