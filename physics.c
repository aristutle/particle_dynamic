/**
 * @ Author: Shimin Cao
 * @ Create Time: 2020-05-02 01:26:20
 * @ Modified by: Shimin Cao
 * @ Modified time: 2020-05-30 13:29:22
 * @ Description:
 */

#pragma once
#include "type.c"
#include "lib.c"
#include <math.h>
#include <omp.h>

vector force(particle target, particle source);
double calc_a(particle *all, int N, int D);
double calc_x(particle *all, int N, int D, double dt);
double calc_v(particle *all, int N, int D, double dt);
double set_zero_v(particle *all, int N);
double set_initial_flags(particle *all, int N);
double find_collide(particle *all, int N, int D);
double collide(particle *all, int N);
double count_particles(particle *all, int N);

vector force(particle target, particle source)
{
    vector res = {0, 0, 0};
    double G = 1000, magnitude = 1;
    double epsi = EPSILON, epsi2 = SQUR(epsi);
    double A = G * target.ppt[MASS] * source.ppt[MASS] / epsi2;
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
        //magnitude = NAN;
    }
    res.x = dx / r * magnitude;
    res.y = dy / r * magnitude;
    res.z = dz / r * magnitude;
    return res;
}
double calc_v(particle *all, int N, int D, double dt)
{
    int i = 0;
    calc_a(all, N, D);
#pragma omp parallel for num_threads(tN) schedule(dynamic, BLOCKSIZE)
    for (i = 0; i < N; i++)
    {
        if (all[i].ppt[EXIST_FLAG] < 0)
            continue;
        all[i].V.x += all[i].A.x * dt;
        all[i].V.y += all[i].A.y * dt;
        all[i].V.z += all[i].A.z * dt;
    }
    return 0;
}
double calc_x(particle *all, int N, int D, double dt)
{
    int i = 0;
#pragma omp parallel for num_threads(tN) schedule(dynamic, BLOCKSIZE)
    for (i = 0; i < N; i++)
    {
        if (all[i].ppt[EXIST_FLAG] < 0)
            continue;
        all[i].R.x += all[i].V.x * dt;
        all[i].R.y += all[i].V.y * dt;
        all[i].R.z += all[i].V.z * dt;
    }

    find_collide(all, N, D);
    if (_coll_list.count > 100)
        printf("%d pairs of collide.\n", _coll_list.count);
    collide(all, N);
    return 0;
}
double calc_a(particle *all, int N, int D)
{
    //here we assume all the colliding has been removed
    int i = 0, j = 0;
#pragma omp parallel for private(j) num_threads(tN) schedule(dynamic, BLOCKSIZE)
    for (i = 0; i < N; i++)
    {
        if (all[i].ppt[EXIST_FLAG] < 0)
            continue;
        vector f = {0.0, 0.0, 0.0};
        all[i].A.x = 0;
        all[i].A.y = 0;
        all[i].A.z = 0;
        for (j = 0; j < N; j++)
        {
            if (i == j || all[j].ppt[EXIST_FLAG] < 0)
                continue;
            f = force(all[i], all[j]);
            all[i].A.x += f.x / all[i].ppt[MASS];
            all[i].A.y += f.y / all[i].ppt[MASS];
            all[i].A.z += f.z / all[i].ppt[MASS];
        }
    }
    return 0;
}
double find_collide(particle *all, int N, int D)
{
    int i = 0, j = 0;
#pragma omp parallel for private(j) num_threads(tN) schedule(dynamic, BLOCKSIZE)
    for (i = 0; i < N; i++)
    {
        if (all[i].ppt[EXIST_FLAG] < 0)
            continue;
        for (j = 0; j < i; j++)
        {
            if (i == j || all[j].ppt[EXIST_FLAG] < 0)
                continue;
            double dx = all[i].R.x - all[j].R.x;
            double dy = all[i].R.y - all[j].R.y;
            double dz = all[i].R.z - all[j].R.z;
            double r2 = SQUR(dx) + SQUR(dy) + SQUR(dz);
            if (r2 < EPSILON * EPSILON)
            {
                //printf("%d and %d collides!\n", i, j);
                list_add(coll_list, &i, &j);
            }
        }
    }
    return 0;
}
double set_zero_v(particle *all, int N)
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
double set_initial_flags(particle *all, int N)
{
    int i = 0;
    for (i = 0; i < N; i++)
    {
        all[i].ppt[EXIST_FLAG] = 1;
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
    while (list_pop(coll_list, &i, &j) >= 0)
    {
        if (all[i].ppt[EXIST_FLAG] < 0 || all[j].ppt[EXIST_FLAG] < 0 || i == j)
            continue;
        a = all[i];
        b = all[j];
        r2 = SQUR(a.R.x - b.R.x) + SQUR(a.R.y - b.R.y) + SQUR(a.R.z - b.R.z);
        m1 = a.ppt[MASS];
        m2 = b.ppt[MASS];
        m = m1 + m2;
        all[i].V.x = (m1 * a.V.x + m2 * b.V.x) / m;
        all[i].V.y = (m1 * a.V.y + m2 * b.V.y) / m;
        all[i].V.z = (m1 * a.V.z + m2 * b.V.z) / m;
        all[i].R.x = (m1 * a.R.x + m2 * b.R.x) / m;
        all[i].R.y = (m1 * a.R.y + m2 * b.R.y) / m;
        all[i].R.z = (m1 * a.R.z + m2 * b.R.z) / m;
        all[i].A.x = (m1 * a.A.x + m2 * b.A.x) / m;
        all[i].A.y = (m1 * a.A.y + m2 * b.A.y) / m;
        all[i].A.z = (m1 * a.A.z + m2 * b.A.z) / m;
        all[i].ppt[MASS] = m;
        all[j].ppt[EXIST_FLAG] = -1;
    }
    list_clear(coll_list);
    return 0;
}

double count_particles(particle *all, int N)
{
    int i = 0;
    int count = 0;
    for (i = 0; i < N; i++)
    {
        if (all[i].ppt[EXIST_FLAG] > 0)
        {
            count++;
        }
    }
    return count;
}