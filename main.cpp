#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <vector>

const int SCREEN_W = 600;
const int SCREEN_H = 600;

void renderCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float a = 2.0f * 3.1415926f * i / segments;
        float x = r * cosf(a);
        float y = r * sinf(a);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

struct OrbitObject {
    float centerX, centerY;       // position on screen
    float orbitDistance;          // distance from center
    float radius;                 // circle size
    float speed;                  // rotation speed
    int segments;                 // smoothness
    float axisX, axisY, axisZ;    // rotation axis
    float angle;                  // current rotation
};

void rangeHint(const char* name, float v, float lo, float hi) {
    if (v < lo || v > hi) {
        std::cout << "Note: " << name
                  << " usually works best between "
                  << lo << " and " << hi << "\n";
    }
}

int main() {
    int count;
    std::cout << "Number of circles: ";
    std::cin >> count;

    std::vector<OrbitObject> items(count);

    for (int i = 0; i < count; i++) {
        std::cout << "\nSetup for circle " << i + 1 << "\n";

        std::cout << "Center X position (-1 to 1): ";
        std::cin >> items[i].centerX;

        std::cout << "Center Y position (-1 to 1): ";
        std::cin >> items[i].centerY;

        std::cout << "Orbit distance (0.1 - 0.8): ";
        std::cin >> items[i].orbitDistance;
        rangeHint("Orbit distance", items[i].orbitDistance, 0.1f, 0.8f);

        std::cout << "Circle radius (0.02 - 0.25): ";
        std::cin >> items[i].radius;
        rangeHint("Circle radius", items[i].radius, 0.02f, 0.25f);

        std::cout << "Rotation speed (0.1 - 10): ";
        std::cin >> items[i].speed;
        rangeHint("Rotation speed", items[i].speed, 0.1f, 10.0f);

        std::cout << "Segment count (40 - 200): ";
        std::cin >> items[i].segments;
        if (items[i].segments < 40)
            std::cout << "Note: low segment count may look rough\n";

        std::cout << "Rotation axis (x y z): ";
        std::cin >> items[i].axisX
                  >> items[i].axisY
                  >> items[i].axisZ;

        if (items[i].axisX == 0 &&
            items[i].axisY == 0 &&
            items[i].axisZ == 0)
            std::cout << "Note: rotation axis is zero\n";

        items[i].angle = 0.0f;
    }

    if (!glfwInit()) return -1;

    GLFWwindow* win = glfwCreateWindow(
        SCREEN_W, SCREEN_H, "Free Orbit Circles", nullptr, nullptr
    );
    if (!win) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(win);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto& c : items) {
            glLoadIdentity();

            // move to this circle's own center
            glTranslatef(c.centerX, c.centerY, 0.0f);

            // rotate around that point
            glRotatef(c.angle, c.axisX, c.axisY, c.axisZ);

            // draw orbiting circle
            renderCircle(c.orbitDistance, 0.0f, c.radius, c.segments);

            c.angle += c.speed;
            if (c.angle >= 360.0f) c.angle -= 360.0f;
        }

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}


