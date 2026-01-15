// Copyright 2026 Timofey Zakharchuk (GitHub: TimGoTheCreator)
// Licensed under the Apache License, Version 2.0
#pragma once

#include <cmath>
#include <string>
#include "raylib.h"
#include "rlgl.h"

// ---------------- CPU PARTICLE ----------------
struct Particle
{
    double x, y;
    double vx, vy;
    double m;
    double r;
    std::string name;

    Particle(double px, double py, double pvx, double pvy, double pm, double pr, const std::string& n)
        : x(px), y(py), vx(pvx), vy(pvy), m(pm), r(pr), name(n) {}
};

// ---------------- GPU PARTICLE ----------------
struct ParticleGPU {
    float x, y;
    float vx, vy;
    float m;
};

// ---------------- CPU GRAVITY ----------------
inline void Gravity(Particle& a, Particle& b, double dt)
{
    const double G(6.67430e-11);
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    double distSq = dx * dx + dy * dy;
    double dist = std::sqrt(distSq);

    if (dist > 1e-5)
    {
        double F = G * a.m * b.m / distSq;
        double ax = F * dx / dist / a.m;
        double ay = F * dy / dist / a.m;
        double bx = -F * dx / dist / b.m;
        double by = -F * dy / dist / b.m;

        a.vx += ax * dt;
        a.vy += ay * dt;
        b.vx += bx * dt;
        b.vy += by * dt;
    }
}

// ---------------- GPU GRAVITY DISPATCH ----------------
inline void GravityGPU(unsigned int ssbo, Shader compute, int count, float dt)
{
    float G = 6.67430e-11f;

    int loc_dt = GetShaderLocation(compute, "dt");
    int loc_G  = GetShaderLocation(compute, "G");

    SetShaderValue(compute, loc_dt, &dt, SHADER_UNIFORM_FLOAT);
    SetShaderValue(compute, loc_G,  &G,  SHADER_UNIFORM_FLOAT);

    rlBindShaderBuffer(ssbo, 0);
    rlEnableShader(compute.id);

    int groups = (count + 255) / 256;
    rlEnableShader(compute.id);
    rlComputeShaderDispatch(groups, 1, 1);
    rlDisableShader();

    rlDisableShader();
}
