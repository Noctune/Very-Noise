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

#include <stdlib.h>
#include <time.h>
#include "vnoise.h"

int main(int argc, char **argv)
{
    struct options opt;
    struct gengetopt_args_info args_info;

    if (cmdline_parser(argc, argv, &args_info))
        return EXIT_FAILURE;

    //Seed with time if no seed is given
    if (args_info.seed_given)
        srand(args_info.seed_arg);
    else
        srand(time(NULL));

    opt.output_file = args_info.output_arg;
    opt.width = args_info.width_arg;
    opt.height = args_info.height_arg;
    opt.depth = args_info.depth_arg;

    if (opt.width < 0 || opt.height < 0)
    {
        fprintf(stderr, "Width and height must be positive");
        return EXIT_FAILURE;
    }
    
    if (opt.depth != 8 && opt.depth != 16)
    {
        fprintf(stderr, "Depth must be 8 or 16.");
        return EXIT_FAILURE;
    }
    
    if (!noise_initialize(&args_info, &opt))
        goto clean_noise;
    if (!sum_initialize(&args_info, &opt))
        goto clean_sum;
    if (!transform_initialize(&args_info, &opt))
        goto clean_transform;

    generate_noise(&opt);

    clean_transform:
    transform_finalize(&opt);
    
    clean_sum:
    sum_finalize(&opt);

    clean_noise:
    noise_finalize(&opt);
    return EXIT_SUCCESS;
}
