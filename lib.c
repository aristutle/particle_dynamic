/**
 * @ Author: Shimin Cao
 * @ Create Time: 2020-04-26 01:47:17
 * @ Modified by: Shimin Cao
 * @ Modified time: 2020-05-02 12:38:35
 * @ Description:
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "type.c"

double *readArray_1D(const char *filename, int dim);
double *readArray(const char *filename, int n, int D, int p);
double *writeArray(const char *filename, double *arr, int n, int D, int p);
particle *array_to_particle(double *arr, int n, int D, int p);
double *particle_to_array(particle *par, int n, int D, int p);
particle *readParticle(const char *filename, int n, int D, int p);
particle *writeParticle(const char *filename, particle *parptr, int n, int D, int p);

particle *readParticle(const char *filename, int n, int D, int p)
{
    particle *res = NULL;
    double *tmp = NULL;
    tmp = readArray(filename, n, D, p);
    res = array_to_particle(tmp, n, D, p);
    free(tmp);
    return res;
}
particle *writeParticle(const char *filename, particle *parptr, int n, int D, int p)
{
    double *tmp = NULL;
    tmp = particle_to_array(parptr, n, D, p);
    writeArray(filename, tmp, n, D, p);
    free(tmp);
    return parptr;
}

double *readArray_1D(const char *filename, int dim)
{
    const char *mode = "r";
    int i;
    FILE *fp;
    double *buffer = (double *)malloc(sizeof(double) * dim);

    fp = fopen(filename, mode);
    if (fp == NULL)
    {
        printf("File cannot open.\n");
        return NULL;
    }

    for (i = 0; i < dim; i++)
    {
        fscanf(fp, "%lf", &(buffer[i]));
    }

    fclose(fp);
    return buffer;
}

double *readArray(const char *filename, int n, int D, int p)
{
    double *buffer = NULL;
    int m = 2 * D + p;
    if (n <= 0 || m <= 0)
    {
        printf("Wrong size!\n");
        return NULL;
    }
    buffer = readArray_1D(filename, n * m);
    if (!buffer)
    {
        printf("Read error!\n");
        return NULL;
    }
    return buffer;
}

double *writeArray(const char *filename, double *arr, int n, int D, int p)
{
    const char *mode = "w";
    int i, j, k, m = 2 * D + p;
    double *buffer = arr;
    FILE *fp;

    fp = fopen(filename, mode);
    if (fp == NULL)
    {
        perror("File cannot open.");
        return NULL;
    }

    if (n <= 0 || m <= 0)
    {
        perror("Wrong size!\n");
        return NULL;
    }
    k = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            if (j)
                fprintf(fp, " ");
            fprintf(fp, "%lf", buffer[k++]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return arr;
}

particle *array_to_particle(double *arr, int n, int D, int p)
{
    particle *res = NULL;
    int i, j, k = 0;
    res = (particle *)malloc(sizeof(particle) * n);
    if (!res)
    {
        printf("Error in array_to_particle!");
        free(arr);
        return NULL;
    }
    for (i = 0; i < n; i++)
    {
        res[i].R.x = arr[k++];
        res[i].R.y = arr[k++];
        if (D >= 3)
            res[i].R.z = arr[k++];
        res[i].V.x = arr[k++];
        res[i].V.y = arr[k++];
        if (D >= 3)
            res[i].V.z = arr[k++];
        for (j = 0; j < p; j++)
        {
            res[i].ppt[j] = arr[k++];
        }
    }
    return res;
}

double *particle_to_array(particle *par, int n, int D, int p)
{
    double *res = NULL;
    int i, j, k, s = n * (2 * D + p);
    res = (double *)malloc(sizeof(double) * s);
    if (!res)
    {
        printf("Error in particle_to_array!");
        return NULL;
    }
    k = 0;
    for (i = 0; i < n; i++)
    {
        res[k++] = par[i].R.x;
        res[k++] = par[i].R.y;
        if (D >= 3)
            res[k++] = par[i].R.z;
        res[k++] = par[i].V.x;
        res[k++] = par[i].V.y;
        if (D >= 3)
            res[k++] = par[i].V.z;
        for (j = 0; j < p; j++)
        {
            res[k++] = par[i].ppt[j];
        }
    }
    return res;
}