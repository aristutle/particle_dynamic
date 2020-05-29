/**
 * @ Author: Shimin Cao
 * @ Create Time: 2020-05-02 01:29:44
 * @ Modified by: Your name
 * @ Modified time: 2020-05-22 16:31:41
 * @ Description:
 */
#pragma once

#define tN (24) //thread number

#define PI 3.141592653
#define SQUR(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
#define EXPOR(r, k) (exp((k) / (r)) / (r))
#define ONEOR(r) (1 / (r))
#define ONEOR3(r) (1 / (CUBE(r)))
#define NQEXP 10000
#define EPSILON 0.2

typedef struct
{
    double x;
    double y;
    double z;
} vector;

typedef struct
{
    vector R;
    vector V;
    vector A;
    double ppt[5];
} particle;