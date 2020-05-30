/**
 * @ Author: Shimin Cao
 * @ Create Time: 2020-04-26 00:11:06
 * @ Modified by: Shimin Cao
 * @ Modified time: 2020-05-30 13:30:45
 * @ Description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#include "lib.c"
#include "physics.c"

int main(int argc, char **argv)
{
    /*
    calling convention: *.exe [in] [out] [N] [D] [p] [end_of_line]
    arguments:                                                                                                                                                                                                                                                                                              
    [in]        filename of input
    [out]       filename of output
    [N]         number of particles, also number of input file rows
    [D]         dimension of particle x,v.
                note that D can be 2 or 3, but struct of vector in the program is always 3 dimension.
    [p]         number of particle properties. All scalar.
                total particle property number should be 2*D+m, which is also number of input file column.
    before running, establish "snapshot" directory in current position to contain snapshot results!
    */
    int i = 0;
    int tid = omp_get_thread_num();
    double *array = NULL;
    particle *allpar = NULL;
    char *default_arg[] = {argv[0], ".\\initial-5000.txt", ".\\final.txt", "5000", "2", "8"};
    char *input_file = NULL;
    char *output_file = NULL;
    int N = 0, D = 0, p = 0, m = 0;
    char **args = argv;
    clock_t start, finish, mid;
    double duration;
    char tmp_name[1024];
    int N_snap = 100;
    int total_round = 10000;
    int particle_number = 0;
    double deltaT = 0.0001;

    if (argc < 2)
    {
        argc = 6;
        args = default_arg;
    }
    input_file = args[1];
    output_file = args[2];
    sscanf(args[3], "%d", &N);
    sscanf(args[4], "%d", &D);
    sscanf(args[5], "%d", &p);
    printf("%s,%s,%d,%d,%d\n", input_file, output_file, N, D, p);

    allpar = readParticle(input_file, N, D, p);

    set_zero_v(allpar, N);
    set_initial_flags(allpar, N);
    sprintf(tmp_name, ".\\snapshot\\round%07d.txt", 0);
    allpar = writeParticle(tmp_name, allpar, N, D, p);

    start = clock();
    mid = clock();
    for (int kk = 0; kk <= total_round; kk++)
    {

        calc_x(allpar, N, D, ksi * deltaT);
        calc_v(allpar, N, D, (0.5 - lambda) * deltaT);
        calc_x(allpar, N, D, chi * deltaT);
        calc_v(allpar, N, D, lambda * deltaT);
        calc_x(allpar, N, D, (1 - 2 * (chi + ksi)) * deltaT);
        calc_v(allpar, N, D, lambda * deltaT);
        calc_x(allpar, N, D, chi * deltaT);
        calc_v(allpar, N, D, (0.5 - lambda) * deltaT);
        calc_x(allpar, N, D, ksi * deltaT);

        if (kk % N_snap == 0 && kk > 0)
        {
            sprintf(tmp_name, ".\\snapshot\\round%07d.txt", kk);
            allpar = writeParticle(tmp_name, allpar, N, D, p);
            finish = clock();
            duration = (double)(finish - mid) / CLOCKS_PER_SEC;
            particle_number = count_particles(allpar, N);
            printf("%d rounds done. %lf ms per round, %d particles left.\n", kk, duration * 1000 / N_snap, particle_number);
            mid = clock();
        }
    }
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    allpar = writeParticle(output_file, allpar, N, D, p);

    free(allpar);
    printf("thread number:%d\n", tN);
    printf("total elapsed time:%lfs\n", duration);
    return EXIT_SUCCESS;
}