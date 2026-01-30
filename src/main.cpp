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
    particles.emplace_back(0, 0, 0, 0, 1.99e30, 6.96e8, "Sun");
    particles.emplace_back(150000000000, 0, 0, 29780, 5.97e24, 6.37e6, "Earth");
    particles.emplace_back(150384400000, 0, 0, 30802, 7.35e22, 1.74e6, "Moon");

    SetTargetFPS(0);

    double dt = 0.4;
    double camX = 0.0;
    double camY = 0.0;
    double simTime = 0.0;
    double scale = 1.5e6;
    int steps = 10000;
    static bool paused = false;
    Vector2 dragStart = {0, 0};
    bool dragging = false;
    static bool left_locked = true;
    static bool right_locked = true;
    double userSpeed = 1.0;
    int trackedIndex = -1;
    static double shotMass = 5.97e24;
    static double shotRadius = 6.37e6;
    static char shotName[64] = "Shot";

    while (!WindowShouldClose()) {
        double effectiveSpeed = scale * userSpeed * (IsKeyDown(KEY_LEFT_SHIFT) ? 5.0 : 1.0);
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

        if (!ImGui::GetIO().WantTextInput)
        {
            if (IsKeyDown(KEY_W)) camY -= effectiveSpeed;
            if (IsKeyDown(KEY_S)) camY += effectiveSpeed;
            if (IsKeyDown(KEY_A)) camX -= effectiveSpeed;
            if (IsKeyDown(KEY_D)) camX += effectiveSpeed;
        }

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

            particles.emplace_back(worldX, worldY, dvx, dvy, shotMass, shotRadius, shotName);
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
            }

            simTime += dt;
        }
        }

        if (trackedIndex >= 0 && trackedIndex < particles.size())
        {
            camX = particles[trackedIndex].x;
            camY = particles[trackedIndex].y;
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

        if (dragging) {
            DrawLineV(dragStart, GetMousePosition(), RED);
        }

        rlImGuiBegin();

        {
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
            if (left_locked)
            {
                ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->Size.y), ImGuiCond_Always);

                flags |= ImGuiWindowFlags_NoMove;
                flags |= ImGuiWindowFlags_NoResize;
            }

            ImGui::Begin("CXXGrav Panel", nullptr, flags);

            if (ImGui::Button(left_locked ? "Unlock" : "Lock"))
                left_locked = !left_locked;
            
            if (ImGui::Button(paused ? "Resume" : "Pause")) {
                paused = !paused;
            }

            ImGui::DragScalar("Camera Speed", ImGuiDataType_Double, &userSpeed, 0.1, nullptr, nullptr, "%.6f");
            
            ImGui::Separator();

            ImGui::DragScalar("Mass (KG)", ImGuiDataType_Double, &shotMass, 1e20, nullptr, nullptr, "%.6e");
            ImGui::DragScalar("Radius (M)", ImGuiDataType_Double, &shotRadius, 1e5, nullptr, nullptr, "%.6e");
            ImGui::InputText("Name", shotName, IM_ARRAYSIZE(shotName));
            ImGui::End();

            {
                ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
                if (right_locked)
                {
                    ImGui::SetNextWindowPos(ImVec2(GetScreenWidth() - 250, 0), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->Size.y), ImGuiCond_Always);

                    flags |= ImGuiWindowFlags_NoMove;
                    flags |= ImGuiWindowFlags_NoResize;                    
                }

                ImGui::Begin("Bodies", nullptr, flags);
                if (ImGui::Button(right_locked ? "Unlock" : "Lock"))
                    right_locked = !right_locked;
                
                ImGui::Separator();
                ImGui::Text("Bodies in simulation:");

                for (int i = 0; i < particles.size(); i++) {
                    std::string label = particles[i].name + "##" + std::to_string(i);
                    if (ImGui::Selectable(label.c_str(), trackedIndex == i)) {
                        trackedIndex = i;}
                }

                if (trackedIndex != -1)
                {
                    if (ImGui::Button("Stop Tracking")) {
                        trackedIndex = -1;
                    }
                }
                

                ImGui::End();
                
            }
        }

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
}
