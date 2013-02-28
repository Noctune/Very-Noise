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

#include <png.h>
#include <stdlib.h>
#include "vnoise.h"

//Renders the noise determined by the settings in opt to the image specified by data
//Returns true if successful and false otherwise.
int render_noise(struct options *opt, png_bytep data)
{
    int x, y;
    double value;

    for (y = 0; y < opt->height; y++)
        for (x = 0; x < opt->width; x++)
        {
            vector position = {0};

            position[0] = (double)x / opt->width;
            position[1] = (double)y / opt->height;
            transform(opt, position);
            value = sum(opt, position);

            // Clamps value to [-1,1]. The noise should be in this range,
            // but due to floating point errors, there might be values above
            // or below.
            value = value < -1 ? -1 : (value > 1 ? 1 : value);

            if (opt->depth == 8)
            {
                unsigned int discrete;

                // Converts real range to 8 bit range
                discrete = (value + 1) / 2.0 * ((1 << 8) - 1);
                data[y * opt->width + x] = discrete;
            }
            else //opt->depth == 16
            {
                unsigned int discrete;

                // Converts real range to 16 bit range
                discrete = (value + 1) / 2.0 * ((1 << 16) - 1);

                //LibPNG seems to expect big endian at the byte level
                data[(y * opt->width + x) * 2] = discrete >> 8;
                data[(y * opt->width + x) * 2 + 1] = discrete;
            }


        }

    return 1;
}

//Generates a noise image and saves it to the file specified by the options parameter.
//Returns true if successful and false otherwise.
#define check(x, msg) if (!(x)) { status = 0; fprintf(stderr, "%s\n", (msg)); goto cleanup; }
int generate_noise(struct options* opt)
{
    int status = 1, i;
    FILE* file;
    png_structp png_data;
    png_infop info;
    png_bytep data = malloc(opt->width * opt->height * sizeof(png_byte) * (opt->depth / 8));
    png_bytepp rows = malloc(opt->height * sizeof(data));

    file = fopen(opt->output_file, "w");
    check(file, "Could not open output file");

    png_data = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    check(png_data, "png_create_write_struct failed");

    info = png_create_info_struct(png_data);
    check(info, "png_create_write_struct failed");

    check(!setjmp(png_jmpbuf(png_data)), "Error during png_init_o");
    png_init_io(png_data, file);

    check(!setjmp(png_jmpbuf(png_data)), "Error during writing of header");
    png_set_IHDR(png_data, info, opt->width, opt->height,
                 opt->depth, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_data, info);
    check(!setjmp(png_jmpbuf(png_data)), "Error during file output");

    for (i = 0; i < opt->height; i++)
        rows[i] = data + i * opt->width * (opt->depth / 8);

    check(render_noise(opt, data), "Error during noise generation");

    png_write_image(png_data, (png_bytepp)rows);

    check(!setjmp(png_jmpbuf(png_data)), "Error during end of file output");

    png_write_end(png_data, NULL);

    cleanup:
    if (file) fclose(file);
    if (data) free(data);
    if (rows) free(rows);

    return status;
}
