// Copyright 2026 Timofey Zakharchuk (GitHub: TimGoTheCreator)
// Licensed under the Apache License, Version 2.0
#pragma once
#include <cmath>
#include <string>

struct Particle
{
    double x, y;
    double vx, vy;
    double m;
    double r;
    std::string name;

    Particle(double px, double py, double pvx, double pvy, double pm, double pr, const std::string&n)
        : x(px), y(py), vx(pvx), vy(pvy), m(pm), r(pr), name(n) {}

};

inline void Gravity(Particle& a, Particle& b, double dt)
{
    constexpr double G (6.67430e-11);
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    double distSq = dx*dx + dy*dy;
    double dist = std::sqrt(distSq);

    if (dist > 1e-5)
    {
        double F = G * a.m * b.m / distSq;
        double ax = F * dx / dist / a.m;
        double ay = F * dy / dist / a.m;
        double bx = -ax * a.m / b.m;
        double by = -ay * a.m / b.m;

        a.vx += ax * dt;
        a.vy += ay * dt;
        b.vx += bx * dt;
        b.vy += by * dt;
    }
}
