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

#define TRANSFORM_LINE 0
#define TRANSFORM_PLANE 1
#define TRANSFORM_PLANE3 2
#define TRANSFORM_PLANE4 3
#define TRANSFORM_CYLINDER 4
#define TRANSFORM_SPHERE 5
#define TRANSFORM_TILEABLE 6

int dimensions(int transform)
{
    switch (transform)
    {
    default: return 0;
    case TRANSFORM_LINE:     return 1;
    case TRANSFORM_PLANE:    return 2;
    case TRANSFORM_PLANE3:   return 3;
    case TRANSFORM_PLANE4:   return 4;
    case TRANSFORM_CYLINDER: return 3;
    case TRANSFORM_SPHERE:   return 3;
    case TRANSFORM_TILEABLE: return 4;
    }
}

void transform(struct options* opt, vector v)
{
    /* Scale value to get a circle with a circumference of 1 */
    const double circle = 0.5 / M_PI;

    switch (opt->transform)
    {
        double angle, inclination, azimuth;
    default:
    case TRANSFORM_LINE:
    case TRANSFORM_PLANE:
    case TRANSFORM_PLANE3:
    case TRANSFORM_PLANE4:
        break;

    case TRANSFORM_CYLINDER:
        angle = v[0] * M_PI * 2;
        v[2] = v[1] * M_PI * 2;
        v[1] = sin(angle) * circle;
        v[0] = cos(angle) * circle;
        break;

    case TRANSFORM_SPHERE:
        azimuth     = v[0] * M_PI * 2;
        inclination = v[1] * M_PI;
        v[2] = cos(inclination) * circle;
        v[1] = sin(inclination) * sin(azimuth) * circle;
        v[0] = sin(inclination) * cos(azimuth) * circle;
        break;

    case TRANSFORM_TILEABLE:
        v[3] = sin(v[1] * M_PI * 2) * circle;
        v[2] = sin(v[0] * M_PI * 2) * circle;
        v[1] = cos(v[1] * M_PI * 2) * circle;
        v[0] = cos(v[0] * M_PI * 2) * circle;
        break;
    }
}

int transform_initialize(struct gengetopt_args_info* args, struct options* opt)
{
    opt->transform =
        args->line_given     ? TRANSFORM_LINE :
        args->plane_given    ? TRANSFORM_PLANE :
        args->plane3_given   ? TRANSFORM_PLANE3 :
        args->plane4_given   ? TRANSFORM_PLANE4 :
        args->cylinder_given ? TRANSFORM_CYLINDER :
        args->sphere_given   ? TRANSFORM_SPHERE :
        args->tileable_given ? TRANSFORM_TILEABLE :
        TRANSFORM_PLANE;

    opt->dimensions = dimensions(opt->transform);

    return 1;
}

void transform_finalize(struct options* opt)
{ }
