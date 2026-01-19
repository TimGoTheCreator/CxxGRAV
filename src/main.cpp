// Copyright 2026 Timofey Zakharchuk (GitHub: TimGoTheCreator)

// Licensed under the Apache License, Version 2.0

#include "conf.h"

#include "raylib.h"

#include <vector>

#include <string>

#include "rlImGui.h"

#include "imgui.h"



int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);

    InitWindow(800, 600, "CxxGRAV");

    rlImGuiSetup(true);

    std::vector<Particle> particles;

    particles.emplace_back(0, 0, 0, 0, 0, 0, 1.99e30, 6.96e8, "Sun");

    particles.emplace_back(150000000000, 0, 0, 0, 29780, 0, 5.97e24, 6.37e6, "Earth");

    particles.emplace_back(150384400000, 0, 0, 0, 30802, 0, 7.35e22, 1.74e6, "Moon");



    SetTargetFPS(0);



    double dt = 0.4;

    double camX = 0.0;

    double camY = 0.0;

    double simTime = 0.0;

    double scale = 1.5e6;

    int steps = 10000;

    double camSpeed = 5000000;

    bool paused = false;

    Vector2 dragStart = {0, 0};

    bool dragging = false;

    double effectiveSpeed = camSpeed * scale * (IsKeyDown(KEY_LEFT_SHIFT) ? 0.01 : 0.001);



    while (!WindowShouldClose()) {

        static int lastW = 0;

        static int lastH = 0;



        int w = GetScreenWidth();

        int h = GetScreenHeight();



        if (w != lastW || h != lastH)

        {

            SetWindowSize(w, h);

            lastW = w;

            lastH = h;

        }



        int cx = w / 2;

        int cy = h / 2;

        

        BeginDrawing();

        ClearBackground(BLACK);



        if (IsKeyPressed(KEY_F11)) ToggleBorderlessWindowed();



        if (IsKeyDown(KEY_W)) camY -= effectiveSpeed;

        if (IsKeyDown(KEY_S)) camY += effectiveSpeed;

        if (IsKeyDown(KEY_A)) camX -= effectiveSpeed;

        if (IsKeyDown(KEY_D)) camX += effectiveSpeed;

        if (IsKeyDown(KEY_UP)) scale *= 1.005;



        if (!ImGui::GetIO().WantCaptureMouse) {

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                dragStart = GetMousePosition();

                dragging = true;

            }

        }



        if (dragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {

            dragging = false;



            Vector2 dragEnd = GetMousePosition();

            double worldX = (dragStart.x - cx) * scale + camX;

            double worldY = (dragStart.y - cy) * scale + camY;



            double dvx = (dragEnd.x - dragStart.x) * 1000;

            double dvy = (dragEnd.y - dragStart.y) * 1000;



            particles.emplace_back(worldX, worldY, 0, dvx, dvy, 0, 5.97e24, 6.37e6, "Shot");

        }



        if (IsKeyDown(KEY_DOWN)) scale /= 1.005;

            

        if (IsKeyPressed(KEY_SPACE)) {

            paused = !paused;

        }



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

                p.z += p.vz * dt;

            }



            simTime += dt;

        }

        }

        

        for (auto& p : particles) {



            int radius = (int)(p.r / scale);

            if (radius < 1) radius = 1;



            Color color = WHITE;

            if (p.name == "Earth") color = BLUE;

            else if (p.name == "Sun") color = YELLOW;

            else if (p.name == "Moon") color = GRAY;



            DrawCircle(

                (int)((p.x - camX) / scale) + cx,

                (int)((p.y - camY) / scale) + cy,

                radius,

                color

            );

        }



        if (paused) {

            DrawText("PAUSED", 10, 10, 20, WHITE);

        }



        if (dragging) {

            DrawLineV(dragStart, GetMousePosition(), RED);

        }



        rlImGuiBegin();;

        rlImGuiEnd();

        EndDrawing();

    }



    rlImGuiShutdown();

    CloseWindow();

}

