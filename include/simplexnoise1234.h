/* SimplexNoise1234, Simplex noise with true analytic
 * derivative in 1D to 4D.
 *
 * Author: Stefan Gustavson, 2003-2005
 * Contact: stegu@itn.liu.se
 *
 * This code was GPL licensed until February 2011.
 * As the original author of this code, I hereby
 * release it into the public domain.
 * Please feel free to use it for whatever you want.
 * Credit is appreciated where appropriate, and I also
 * appreciate being told where this code finds any use,
 * but you may do as you like.
 */

 /* Minor changes by Mike Pedersen
  * Instead of having a constant permutation table, the snoise_init
  * function builds it. This allows the snoise functions to change
  * based on a seed value. This file, along with simplexnoise1234.c,
  * is still under public domain, though this might not necessarily
  * be true for the rest of the project.
  */


/** \file
		\brief Header for "simplexnoise1234.c" for producing Perlin simplex noise.
		\author Stefan Gustavson (stegu@itn.liu.se)
*/

/*
 * This is a clean, fast, modern and free Perlin Simplex noise function.
 * It is a stand-alone compilation unit with no external dependencies,
 * highly reusable without source code modifications.
 *
 *
 * Note:
 * Replacing the "float" type with "double" can actually make this run faster
 * on some platforms. Having both versions could be useful.
 */

/** 1D, 2D, 3D and 4D float Perlin simplex noise
 */
float snoise1( float x );
float snoise2( float x, float y );
float snoise3( float x, float y, float z );
float snoise4( float x, float y, float z, float w );
void snoise_init();
