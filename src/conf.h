// Copyright 2026 Timofey Zakharchuk (GitHub: TimGoTheCreator)
// Licensed under the Apache License, Version 2.0
#pragma once
#include <cmath>
#include <string>

struct Particle
{
    double x, y, z;
    double vx, vy, vz;
    double m;
    double r;
    std::string name;

    Particle(double px, double py, double pz, double pvx, double pvy, double pvz, double pm, double pr, const std::string&n)
        : x(px), y(py), z(pz), vx(pvx), vy(pvy), vz(pvz), m(pm), r(pr), name(n) {}

};

inline void Gravity(Particle& a, Particle& b, double dt)
{
    constexpr double G (6.67430e-11);
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    double dz = b.z - a.z;
    double distSq = dx*dx + dy*dy + dz*dz;
    double dist = std::sqrt(distSq);

    if (dist > 1e-5)
    {
        double F = G * a.m * b.m / distSq;
        double ax = F * dx / dist / a.m;
        double ay = F * dy / dist / a.m;
        double az = F * dz / dist / a.m;
        double bx = -ax * a.m / b.m;
        double by = -ay * a.m / b.m;
        double bz = -az * a.m / b.m;

        a.vx += ax * dt;
        a.vy += ay * dt;
        a.vz += az * dt;
        b.vx += bx * dt;
        b.vy += by * dt;
        b.vz += bz * dt;
    }
} 
