// Copyright 2026 Timofey Zakharchuk (GitHub: TimGoTheCreator)
// Licensed under the Apache License, Version 2.0

#include "conf.h"
#include "raylib.h"
#include <vector>
#include <string>

int main() {
    InitWindow(800, 600, "CxxGRAV");

    std::vector<Particle> particles;
    particles.emplace_back(0, 0, 0, 0, 1.99e30, 6.96e8, "Sun");
    particles.emplace_back(150000000000, 0, 0, 29780, 5.97e24, 6.37e6, "Earth");
    particles.emplace_back(150384400000, 0, 0, 30802, 7.35e22, 1.74e6, "Moon");

    // ---- GPU PARTICLE BUFFER + SHADER ----
    std::vector<ParticleGPU> gpuParticles;
    gpuParticles.reserve(particles.size());
    for (auto& p : particles) {
        gpuParticles.push_back({
            (float)p.x, (float)p.y,
            (float)p.vx, (float)p.vy,
            (float)p.m
        });
    }

    unsigned int ssbo = rlLoadShaderBuffer(
        gpuParticles.size() * sizeof(ParticleGPU),
        gpuParticles.data(),
        RL_DYNAMIC_DRAW
    );

    Shader compute = LoadShader(0, "gravity.comp");

    bool useGPU = false; // 1 = CPU, 2 = GPU

    SetTargetFPS(0);

    double dt = 0.4;
    double camX = 0.0;
    double camY = 0.0;
    double simTime = 0.0;
    double scale = 1.5e6;
    int steps = 10000;
    double camSpeed = 5000000;
    bool paused = false;

    while (!WindowShouldClose()) {
        // mode toggle
        if (IsKeyPressed(KEY_ONE)) useGPU = false;
        if (IsKeyPressed(KEY_TWO)) useGPU = true;

        if (IsKeyDown(KEY_W)) camY -= camSpeed;
        if (IsKeyDown(KEY_S)) camY += camSpeed;
        if (IsKeyDown(KEY_A)) camX -= camSpeed;
        if (IsKeyDown(KEY_D)) camX += camSpeed;
        if (IsKeyDown(KEY_UP)) scale *= 1.005;
        if (IsKeyDown(KEY_DOWN)) scale /= 1.005;
        if (IsKeyPressed(KEY_SPACE)) paused = !paused;

        if (!paused) {
            for (int s = 0; s < steps; s++) {

                if (!useGPU) {
                    // ---- CPU PHYSICS ----
                    for (size_t i = 0; i < particles.size(); i++) {
                        for (size_t j = i + 1; j < particles.size(); j++) {
                            Gravity(particles[i], particles[j], dt);
                        }
                    }
                } else {
                    // ---- GPU PHYSICS ----
                    GravityGPU(ssbo, compute, (int)particles.size(), (float)dt);
                }

                for (auto& p : particles) {
                    p.x += p.vx * dt;
                    p.y += p.vy * dt;
                }

                simTime += dt;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (auto& p : particles) {
            int radius = (int)(p.r / scale);
            if (radius < 1) radius = 1;

            Color color = WHITE;
            if (p.name == "Earth") color = BLUE;
            else if (p.name == "Sun") color = YELLOW;
            else if (p.name == "Moon") color = GRAY;

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

        DrawText(useGPU ? "MODE: GPU (2)" : "MODE: CPU (1)", 10, 40, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
