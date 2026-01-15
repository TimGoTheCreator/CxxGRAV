#include "conf.h"
#include "raylib.h"
#include <vector>
#include <string>

int main() {
    InitWindow(800, 600, "SemiEuler");
    std::vector<Particle> particles;
    particles.emplace_back(-5e10, 0, 0, 15000, 2e30, 7e8, "StarA");
    particles.emplace_back(5e10, 0, 0, -15000, 2e30, 7e8, "StarB");
    particles.emplace_back(0, 5e10, -7500, 0, 2e30, 7e7, "StarC");


    SetTargetFPS(0); // Unlock FPS

    double dt = 0.4;
    double camX = 0.0;
    double camY = 0.0;
    double simTime = 0.0;
    double scale = 1.5e6;
    int steps = 10000;
    double camSpeed = 5000000;
    bool paused = false;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W)) camY -= camSpeed;
        if (IsKeyDown(KEY_S)) camY += camSpeed;
        if (IsKeyDown(KEY_A)) camX -= camSpeed;
        if (IsKeyDown(KEY_D)) camX += camSpeed;
        if (IsKeyDown(KEY_UP)) scale *= 1.005;

        if (IsKeyDown(KEY_DOWN)) scale /= 1.005;
                if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }

        // If its unpaused, run the loop, otherwise, dont.
        if(!paused) {
        for (int s = 0; s < steps; s++) {
            for (size_t i = 0; i < particles.size(); i++) {
                for (size_t j = i + 1; j < particles.size(); j++) {
                    Gravity(particles[i], particles[j], dt);
                }
            }

            for (auto& p : particles) {
                p.x += p.vx * dt;
                p.y += p.vy * dt;
            }

            simTime += dt;
        }
        }

        // Draw particles
        BeginDrawing();
        ClearBackground(BLACK);
        for (auto& p : particles) {

            int radius = (int)(p.r / scale);
            if (radius < 1) radius = 1;

            Color color = WHITE;
            if (p.name == "StarA") color = BLUE; // if particle name "Earth" set color blue
            else if (p.name == "StarB") color = YELLOW; // if particle name "Sun" set color yellow
            else if (p.name == "StarC") color = PURPLE; // if particle name "Moon" set color gray

            DrawCircle(
                (int)((p.x - camX) / scale) + 400,
                (int)((p.y - camY) / scale) + 300,
                radius,
                color
            );
        }

        if (paused) {
            DrawText("PAUSED", 10, 10, 20, WHITE);
        }

        EndDrawing();
    }
}
