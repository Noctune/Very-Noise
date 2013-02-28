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

#include <math.h>
#include <string.h>
#include "vnoise.h"
#include "simplexnoise1234.h"

#define NOISE_SIMPLEX 0

double noise_function(struct options* opt, vector v)
{
    switch(opt->noise)
    {
    default: return 0;
    case NOISE_SIMPLEX:
        switch (opt->dimensions)
        {
            default: return 0;
            case 1: return snoise1(v[0]);
            case 2: return snoise2(v[0], v[1]);
            case 3: return snoise3(v[0], v[1], v[2]);
            case 4: return snoise4(v[0], v[1], v[2], v[3]);
        }
    }
}

double noise(struct options* opt, vector v)
{
    double value;
    value = noise_function(opt, v);

    if (opt->ridged)
        value = fabs(value) * 2 - 1;
    return value;
}

int noise_initialize(struct gengetopt_args_info* args, struct options* opt)
{
    opt->noise =
        args->simplex_given ? NOISE_SIMPLEX :
        NOISE_SIMPLEX;

    opt->ridged = args->ridged_given;

    switch (opt->noise)
    {
        default:
            return 0;
        case NOISE_SIMPLEX:
            snoise_init();
            break;
    }
    return 1;
}

void noise_finalize(struct options* opt)
{ }
