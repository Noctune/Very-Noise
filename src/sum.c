/* Copyright (C) 2013  Mike Pedersen
 * This file is part of VNoise.
 *
 * VNoise is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VNoise is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with VNoise.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "vnoise.h"

#define SUM_FRACTAL 0

void add(double* a, double* b)
{
    int i;
    for (i = 0; i < VECTOR_LENGTH; i++)
        a[i] += b[i];
}

void scale(double* a, double b)
{
    int i;
    for (i = 0; i < VECTOR_LENGTH; i++)
        a[i] *= b;
}

double sum(struct options* opt, vector v)
{
    int i;
    vector position;
    double value = 0;
    for (i = 0; i < opt->octaves; i++)
    {
        memcpy(position, v, VECTOR_SIZE);
        scale(position, opt->frequency * pow(opt->lacunarity, i));
        add(position, opt->offsets + i * VECTOR_SIZE);
        value += noise(opt, position) * pow(opt->persistence, i);
    }

    /* Normalize into [-1,1] range */
    return value * opt->scale;
}

int sum_initialize(struct gengetopt_args_info* args, struct options* opt)
{
    int i;
    const double offset = 7883; //Arbitarily chosen prime value for offsets

    opt->octaves = args->octaves_arg;
    opt->persistence = args->persistence_arg;
    opt->lacunarity = args->lacunarity_arg;
    opt->frequency = args->frequency_arg;
    opt->scale = (1.0 - 1.0 / opt->persistence) / (pow(opt->persistence, opt->octaves - 1.0) - 1.0 / opt->persistence);

    opt->offsets = malloc(sizeof(double) * opt->octaves * VECTOR_SIZE);
    for (i = 0; i < opt->octaves * VECTOR_SIZE; i++)
        opt->offsets[i] = (double)rand() / (double)RAND_MAX * offset;

    return 1;
}

void sum_finalize(struct options* opt)
{
    free(opt->offsets);
}
