#pragma once
#include <cmath>

struct Particle
{
    double x, y;
    double vx, vy;
    double m;

    Particle(double px, double py, double pvx, double pvy, double pm)
        : x(px), y(py), vx(pvx), vy(pvy), m(pm) {}

};

inline void Gravity(Particle& a, Particle& b, double dt)
{
    const double G (6.67430e-11);
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    double distSq = dx*dx + dy*dy;
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
