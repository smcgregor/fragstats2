/*
 *  ERDAS file image header
 */

	struct image_header {
                char         head[6];           /* version     */
                short int    pack_type;         /* 4,8,16 bits */
                short int    nbands;            /* #bands      */
                char         notused2[6];
                int          rcols;             /* #cols       */
                int          rrows;             /* #rows       */
                int          xfirst;            /* start X - UL*/
                int          yfirst;            /* start Y - UL*/
                char         notused3[56];
                short int    map_units;         /* code for units*/
                short int    num_classes;       /* #classes    */
                char         notused4[28];
                char         x_pixel_size[4];   /* X cell size */
                char         y_pixel_size[4];   /* Y cell size */
        };
 
        struct  image_header    imagehd;
