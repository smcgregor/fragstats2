/*
 * This file defines a structure to contain patch boundary
 * information.  This definition is used in patch.c and nndist.c
 *
 * Tom Moore (tmoore@pnfi.forestry.ca)
 * Sept. 28/93
 *
 */

typedef struct {
  int min_x, min_y, max_x, max_y;
  short *px, *py;
} patchdef;
