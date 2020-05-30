/**
 * @ Author: Shimin Cao
 * @ Create Time: 2020-05-02 01:29:44
 * @ Modified by: Shimin Cao
 * @ Modified time: 2020-05-30 11:10:59
 * @ Description:
 */
#pragma once

#define tN (10) //thread number
#define BLOCKSIZE (200)
//openMP parallel dynamic schedule unit,
//distribute BLOCKSIZE particles as one batch to each thread,
//and give another when some thread is idle.
//This can improve multithread efficiency.

//Here we use the “Position Extended Forest - Ruth Like” (PEFRL) algorithm to improve calculation precision.
//Below are some constants.
const double ksi = +0.1786178958448091E+00;
const double lambda = -0.2123418310626054E+00;
const double chi = -0.6626458266981849E-01;

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
    double ppt[8];
    //currently used property:
    //ppt[0]:mass
    //ppt[1]:radius, unused
    //ppt[2]:charge, unused
    //ppt[3]:unused
    //ppt[4]:unused
    //ppt[5]:unused
    //ppt[6]:unused
    //ppt[7]:existing flag, 1 if exist, -1 if deleted
} particle;

#define MASS (0)
#define EXIST_FLAG (7)
#define LIST_MAX (10000000)
typedef struct
{
    int i;
    int j;
} pair;

typedef struct
{
    int count;
    pair _list[LIST_MAX];
} pair_list;

pair_list _coll_list;
pair_list *coll_list = &_coll_list;