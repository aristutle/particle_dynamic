/**
 * @ Author: Shimin Cao
 * @ Create Time: 2020-05-02 01:26:20
 * @ Modified by: Your name
 * @ Modified time: 2020-05-22 16:17:05
 * @ Description:
 */

#pragma once
#include "type.c"
#include <math.h>
#include <omp.h>

vector force(particle target, particle source);
double calc_a(particle *all, int N, int D, double dt);
double calc_x(particle *all, int N, int D, double dt);
double calc_v(particle *all, int N, int D, double dt);
double calc_v_and_x(particle *all, int N, int D, double dt);
double set_v(particle *all, int N);
double set_flag(particle *all, int N);
double collide(particle *all, int N);

vector force(particle target, particle source)
{
    vector res = {0, 0, 0};
    double G = 1000, magnitude = 1;
    double epsi = EPSILON, epsi2 = SQUR(epsi);
    double A = G * target.ppt[0] * source.ppt[0] / epsi2;
    double dx = target.R.x - source.R.x;
    double dy = target.R.y - source.R.y;
    double dz = target.R.z - source.R.z;
    double r2 = SQUR(dx) + SQUR(dy) + SQUR(dz), r = sqrt(r2);
    if (r2 > epsi2)
    {
        magnitude = -epsi2 / r2 * A;
    }
    else
    {
        //magnitude = 8 * A / epsi2 * SQUR(r - 3 / 4 * epsi) - 3 * A / 2;
        magnitude = 0;
    }
    res.x = dx / r * magnitude;
    res.y = dy / r * magnitude;
    res.z = dz / r * magnitude;
    return res;
}
double calc_x(particle *all, int N, int D, double dt)
{
    int i = 0;
#pragma omp parallel for num_threads(tN) schedule(dynamic,100)
    for (i = 0; i < N; i++)
    {
        if (all[i].ppt[1] < 0)
            continue;
        all[i].R.x += all[i].V.x * dt;
        all[i].R.y += all[i].V.y * dt;
        all[i].R.z += all[i].V.z * dt;
    }
    return 0;
}

double calc_v(particle *all, int N, int D, double dt)
{
    int i = 0;
#pragma omp parallel for num_threads(tN) schedule(dynamic,100)
    for (i = 0; i < N; i++)
    {
        if (all[i].ppt[1] < 0)
            continue;
        all[i].V.x += all[i].A.x * dt;
        all[i].V.y += all[i].A.y * dt;
        all[i].V.z += all[i].A.z * dt;
    }
    return 0;
}
double calc_v_and_x(particle *all, int N, int D, double dt)
{
    int i = 0;
#pragma omp parallel for num_threads(tN) schedule(dynamic,100)
    for (i = 0; i < N; i++)
    {
        if (all[i].ppt[1] < 0)
            continue;
        all[i].V.x += all[i].A.x * dt;
        all[i].V.y += all[i].A.y * dt;
        all[i].V.z += all[i].A.z * dt;
        all[i].R.x += all[i].V.x * dt;
        all[i].R.y += all[i].V.y * dt;
        all[i].R.z += all[i].V.z * dt;
    }
    return 0;
}
double calc_a(particle *all, int N, int D, double dt)
{
    int i = 0, j = 0;
#pragma omp parallel for private(j) num_threads(tN) schedule(dynamic,100)
    for (i = 0; i < N; i++)
    {
        if (all[i].ppt[1] < 0)
            continue;
        vector f;
        all[i].A.x = 0;
        all[i].A.y = 0;
        all[i].A.z = 0;
        for (j = 0; j < N; j++)
        {
            if (i == j)
                continue;
            f = force(all[i], all[j]);
            all[i].A.x += f.x / all[i].ppt[0];
            all[i].A.y += f.y / all[i].ppt[0];
            all[i].A.z += f.z / all[i].ppt[0];
        }
    }
    return 0;
}

double set_v(particle *all, int N)
{
    int i = 0;
    for (i = 0; i < N; i++)
    {
        all[i].V.x = 0;
        all[i].V.y = 0;
        all[i].V.z = 0;
    }
    return 0;
}
double set_flag(particle *all, int N)
{
    int i = 0;
    for (i = 0; i < N; i++)
    {
        all[i].ppt[1] = 1;
    }
    return 0;
}
double collide(particle *all, int N)
{
    int i = 0, j = 0;
    particle a, b;
    double epsi = EPSILON, epsi2 = SQUR(epsi);
    double r2 = 0;
    double m1, m2, m;
    for (i = 1; i < N; i++)
    {
        if (all[i].ppt[1] < 0)
            continue;
        for (j = 0; j < i; j++)
        {
            if (all[j].ppt[1] < 0 || i == j)
                continue;
            a = all[i];
            b = all[j];
            r2 = SQUR(a.R.x - b.R.x) + SQUR(a.R.y - b.R.y) + SQUR(a.R.z - b.R.z);
            if (r2 < epsi2)
            {
                m1 = a.ppt[0];
                m2 = b.ppt[0];
                m = m1 + m2;
                all[i].V.x = (m1 * a.V.x + m2 * b.V.x) / m;
                all[i].V.y = (m1 * a.V.y + m2 * b.V.y) / m;
                all[i].V.z = (m1 * a.V.z + m2 * b.V.z) / m;
                all[i].R.x = (m1 * a.R.x + m2 * b.R.x) / m;
                all[i].R.y = (m1 * a.R.y + m2 * b.R.y) / m;
                all[i].R.z = (m1 * a.R.z + m2 * b.R.z) / m;
                all[i].ppt[0] = m1+m2;
                all[j].ppt[1] = -1;
            }
        }
    }

    return 0;
}