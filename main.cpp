#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>

#include "shader.h"
#include "sound_manager.h"
#include "collision.h"
#include "target.h"
#include "bolt.h"
#include "fire.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <FreeImage.h>

#define SCR_WIDTH  800
#define SCR_HEIGHT 800
#define N_VO 2

GLFWwindow* window;
SoundManager *SoundMan;
glm::vec2 mouse_pos(0.0f);
GLuint VAO[N_VO], VBO[N_VO];
Shader passthroughShader = Shader();

enum object { O_TARGET, O_BOLT, O_FIRE };
std::vector<Target*> targets;
std::vector<Bolt*> bolts;
std::vector<Fire*> fires;
object active_object;

std::unordered_map<object, glm::vec3> colors;
bool toggle_spawn_targets = false;

void saveImage(int frameCount, bool saveAnimation) {
    int img_width = SCR_WIDTH;
    int img_height = SCR_HEIGHT;
    BYTE* pixels = new BYTE[3 * img_width * img_height];
    glReadPixels(0, 0, img_width, img_height, GL_BGR, GL_UNSIGNED_BYTE, pixels);
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, img_width, img_height, 3 * img_width, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

    char filename[13];
    sprintf(filename, "img/%05d.bmp", frameCount);
    FreeImage_Save(FIF_BMP, image, filename, 0);
    FreeImage_Unload(image);
    delete [] pixels;

    if (saveAnimation) {
        glfwSetWindowShouldClose(window, true);
        std::system("ffmpeg -y -f image2 -i 'img/%05d.bmp' animation.gif");
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (active_object == O_TARGET)
            targets.push_back(new Target(mouse_pos));
        else if (active_object == O_BOLT)
            bolts.push_back(new Bolt(mouse_pos));
        else if (active_object == O_FIRE)
            fires.push_back(new Fire(mouse_pos));
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_R) {
                if (active_object == O_TARGET)
                    targets.push_back(new Target(mouse_pos));
                else if (active_object == O_BOLT)
                    bolts.push_back(new Bolt(mouse_pos));
                else if (active_object == O_FIRE)
                    fires.push_back(new Fire(mouse_pos));
            } else if (key == GLFW_KEY_1) {
                active_object = O_TARGET;
            } else if (key == GLFW_KEY_2) {
                active_object = O_BOLT;
            } else if (key == GLFW_KEY_3) {
                active_object = O_FIRE;
            } else if (key == GLFW_KEY_TAB) {
                toggle_spawn_targets = !toggle_spawn_targets;
            } else if (key == GLFW_KEY_M) {
                SoundMan->toggleMute();
            }
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // Map to (-1,-1) -- (1,1) coordinate space
    float x = 2 * (xpos / SCR_WIDTH) - 1;
    float y = 1 - 2 * (ypos / SCR_HEIGHT);
    mouse_pos = glm::vec2(x, y);
}

void init() {
    srand(time(NULL));

    // Setup window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Arcane", NULL, NULL);
    if (window == NULL) {
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
        return;
	}
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0); // disable VSync, which artificially limits FPS
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	    std::cout << "Failed to initialize GLAD" << std::endl;
        return;
	}

    // Setup sound manager
    SoundMan = new SoundManager();
    SoundMan->loadSound("explosion");

    // Setup colors
    active_object = O_BOLT;
    colors[O_TARGET] = glm::vec3(1.0f, 1.0f, 1.0f);
    colors[O_BOLT] = glm::vec3(1.0f, 0.0f, 1.0f);
    colors[O_FIRE] = glm::vec3(1.0f, 0.0f, 0.0f);

    // Setup shader programs
    passthroughShader = Shader("shaders/passthrough.vs", "shaders/passthrough.fs");

    // Setup VAOs, VBOs
    glPointSize(10);
    float centerVertices[] = { 0.0f, 0.0f };
    float mutedVertices[] = {
        -1.0f, 0.9f,
        -0.9f, 1.0f,
        -1.0f, 1.0f,
        -0.9f, 0.9f,
    };

    glGenVertexArrays(N_VO, VAO);
    glGenBuffers(N_VO, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(centerVertices), centerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mutedVertices), mutedVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

std::vector<Bolt*> explode(Target *target) {
    SoundMan->playSound("explosion", false);
    std::vector<Bolt*> shrapnel;
    for (int i = 0; i < 2; i++) {
        float angle = RAND_F * 2*glm::pi<float>();
        glm::vec2 dir(glm::cos(angle), glm::sin(angle));
        shrapnel.push_back(new Bolt(dir, target->pos, 0.03f));
    }
    return shrapnel;
}

void update(float dt) {
    std::vector<Target*>::iterator it_t;
    std::vector<Bolt*>::iterator it_b;
    std::vector<Fire*>::iterator it_f;

    // Update target movements
    for (Target *target : targets) {
        target->move(dt);
    }

    // Grow bolts, check for OOB and collisions
    std::vector<Bolt*> all_shrapnel;
    it_b = bolts.begin();
    while (it_b != bolts.end()) {
        if ((*it_b)->outOfBounds()) {
            bolts.erase(it_b);
        } else {
            (*it_b)->grow();

            bool collision = false;
            it_t = targets.begin();
            while (it_t != targets.end()) {
                if (check_collision_point_rect((*it_b)->getPos(), (*it_t)->pos, (*it_t)->size)) {
                    collision = true;
                    std::vector<Bolt*> shrapnel = explode(*it_t);
                    all_shrapnel.insert(all_shrapnel.end(), shrapnel.begin(), shrapnel.end());

                    bolts.erase(it_b);
                    targets.erase(it_t);
                    break;
                }
                it_t++;
            }

            if (!collision)
                it_b++;
        }
    }
    bolts.insert(bolts.end(), all_shrapnel.begin(), all_shrapnel.end());

    // Move and shrink fires, check for fizzle out and collisions
    std::vector<Fire*> embers;
    it_f = fires.begin();
    while (it_f != fires.end()) {
        if ((*it_f)->update(dt)) {
            fires.erase(it_f);
        } else {
            it_t = targets.begin();
            while (it_t != targets.end()) {
                if (check_collision_rect_rect((*it_f)->pos, (*it_f)->size, (*it_t)->pos, (*it_t)->size)) {
                    embers.push_back(new Fire((*it_t)->pos, (*it_t)->vel, (*it_t)->size / 3));
                    targets.erase(it_t);
                } else {
                    it_t++;
                }
            }
            it_f++;
        }
    }
    fires.insert(fires.end(), embers.begin(), embers.end());
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    passthroughShader.use();
    passthroughShader.setFloat("time", glfwGetTime());

    // Center block
    passthroughShader.setBool("glow", false);
    passthroughShader.setVec3("color", colors[active_object]);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_POINTS, 0, 1);

    // Muted cross
    if (SoundMan->muteOn) {
        passthroughShader.setBool("glow", false);
        passthroughShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_LINE_STRIP, 0, 4);
    }

    // Targets
    passthroughShader.setBool("glow", false);
    passthroughShader.setVec3("color", colors[O_TARGET]);
    for (Target *target : targets)
        target->draw();

    // Bolts
    passthroughShader.setBool("glow", true);
    passthroughShader.setVec3("color", colors[O_BOLT]);
    for (Bolt *bolt : bolts)
        bolt->draw();

    // Fires
    passthroughShader.setBool("glow", true);
    passthroughShader.setVec3("color", colors[O_FIRE]);
    for (Fire *fire : fires)
        fire->draw();
}

void spawn_targets() {
    if (RAND_F < 0.001) {
        int border = std::rand() % 4;
        float offset = 2 * RAND_F - 1;

        glm::vec2 pos;
        if (border == 0)
            pos = { offset, -1.0f };
        else if (border == 1)
            pos = { offset, 1.0f };
        else if (border == 2)
            pos = { -1.0f, offset };
        else if (border == 3)
            pos = { 1.0f, offset };

        targets.push_back(new Target(pos));
    }
}

void run() {
    unsigned int frameCount = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    float dt = 0, prevTime = 0;
    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        dt = time - prevTime;
        prevTime = time;

        if (toggle_spawn_targets)
            spawn_targets();

        processInput(window);
        update(dt);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        frameCount++;
        //saveImage(frameCount, false);
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //std::cout << "FPS: " << (double)frameCount / (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count()) * 1000000 << std::endl;
    }
}

int main() {
    init();
    run();

    glDeleteVertexArrays(N_VO, VAO);
    glDeleteBuffers(N_VO, VBO);

    glfwTerminate();
    return 0;
}
