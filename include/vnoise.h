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

#ifndef VNOISE_H
#define VNOISE_H

#include "cmdline.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define VECTOR_LENGTH 4
#define VECTOR_SIZE (sizeof(double) * VECTOR_LENGTH)

//Vector used for positions in up to 4 dimensions.
//Will always be passed as pointer, since it's an array
typedef double vector[VECTOR_LENGTH];

struct options
{
    char* output_file;

    //Width and height of the output image. Must be positive.
    int width;
    int height;

    //Bit depth for the grayscale image. May be either 8 or 16.
    int depth;

    //--- Transform function options
    int transform;      //The selected transform function
    int dimensions;     //The amount of dimensions after the transform function has been applied.

    //--- Noise function options
    int noise;          //The selected noise function
    int ridged;         //If true, the absolute value of the noise function is used

    //--- Sum function options
    int octaves;        //The amount of octaves to be summed
    double persistence; //Determines the amplitude of each octave as a product of the last.
    double lacunarity;  //Determines the frequency of each octave as a product of the last.
    double frequency;   //Base frequency of the noise function
    double scale;       //The scaling factor to apply in order to normalize the sum to [-1,1] range.
    double* offsets;    //Offsets of each octave in order to prevent obvious patterns when using the same noise functtion several times
};

//Transforms the vector v (v is input and output)
void transform(struct options* opt, vector v);

//Returns the fractal noise at position v
//Output should be in [-1,1] range
double sum(struct options* opt, vector v);

//Returns the  noise value for a given vector position
//Output should be in [-1,1] range
double noise(struct options* opt, vector v);

int transform_initialize(struct gengetopt_args_info* args, struct options* opt);
int sum_initialize(struct gengetopt_args_info* args, struct options* opt);
int noise_initialize(struct gengetopt_args_info* args, struct options* opt);

void transform_finalize(struct options* opt);
void sum_finalize(struct options* opt);
void noise_finalize(struct options* opt);

int generate_noise(struct options*);
#endif
