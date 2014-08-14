/*
 *  Include file stats.h
 */
#include "routines.h"

#define  TRUE  1
#define  FALSE 0
#define  MAX_LABEL_LENGTH  10            /* max len of descriptive class names  */

extern char	imagename[];             /* name of input image file            */
extern char	out_file[];              /* basename for output files           */
extern char	weight_file[];           /* name of weight file                 */
extern char	id_file[];               /* name of patch ID image              */
extern char	desc_file[];             /* name of class descriptor file       */
extern char	file1[];                 /* name of basename.patch file         */
extern char	file2[];                 /* name of basename.class file         */
extern char	file3[];                 /* name of basename.land file          */
extern char	file4[];                 /* name of basename.full file          */
extern char	contrast_indices;        /* flag; calculate contrast indices    */
extern char	proximity;               /* flag; calculate proximity indices   */
extern char	id_image;                /* flag; ID image input                */
extern char	descriptors;             /* flag; descriptor file input         */
extern char	land_border;             /* flag; landscape border exists       */
extern char	do_nndist;               /* flag; calc nearest neigh indices    */
extern char	patch_stats;             /* flag; output patch statistics       */
extern char	class_stats;             /* flag; output class statistics       */
extern char	use_diags;               /* flag; use diagonals in patch finding*/
extern char	do_patchrich;            /* flag; calculate patch richness      */
extern char	*check;                  /* used in patch finding routine       */
extern char	**name;                  /* char descriptor for each class      */
extern char     bcode;                   /* flag; int, ext background present   */
  
extern short xpos[];                     /* X,Y pos of 8 neighbors              */
extern short ypos[];                     /* surrounding a cell                  */
  
extern short	*image;                  /* pointer to input image data         */
extern short	*id;                     /* pointer to ID image                 */
extern short	*stackx;                 /* column location of patch cells      */
extern short	*stacky;                 /* row location of patch cells         */
extern short	*patchtype;              /* classes that exist in the landscape */
extern short	*patchid;                /* ID of each patch in the landscape   */
extern short	data_type;               /* file type of input image            */
extern short	background;              /* background value                    */
extern short	num_rows;                /* number of rows in input image       */
extern short	num_cols;                /* number of columns in input image    */
extern short	min_class;               /* min class (patch) value in image    */
extern short	max_class;               /* max class (patch) value in image    */
extern short	max_num_classes;         /* max class value possible            */
extern short	min_dist;                /* dist from edge in cells for corearea*/
extern short	min_wt;                  /* min class value in weight file      */
extern short	max_wt;                  /* max class value in weight file      */
extern short	num_wt;                  /* #classes in weight file             */
  
extern int	MAX_CLASSES;             /* max class value in image plus 1     */
extern int	MAX_PATCHES;             /* #patches in the landscape           */
extern int	MAX_PATCH_SIZE;          /* size of largest patch in landscape  */
extern int	NUM_CLASSES;             /* #classes in the landscape           */
extern int	total_num_classes;       /* #classes, including landscape border*/
extern int	num_patches;             /* #patches in a class                 */
extern int	total_patches;           /* #patches in the landscape           */
extern int	total_nn;                /* #patches in landscape with neighbors*/
extern int	boundary;                /* length of the landscape boundary    */
extern int	total_size;              /* total size of image in cells        */
extern int	total_core_patches;      /* total #corearea patches in landscape*/
extern int	total_num_patches;       /* total #patches in landscape         */
extern int	*edge;                   /* len of patch types bordering a patch*/
extern int	*edgetype;               /* len of patch types bordering a patch*/
extern int	*ext_npts;               /* #exterior cells for each patch      */
extern int	*core_area_patches;      /* #corearea patches for each patch    */
  
extern float	*perimeter;              /* perimeter of each patch             */
extern float	*core_area_index;        /* core area index of each patch       */
extern float	*shape;                  /* shape index of each patch           */
extern float	*fractal;                /* fractal dimension of each patch     */
extern float	*class_area;             /* area of the class being evaluated   */
extern float	*contrast;               /* weights of each combo of classes    */
extern float	*edge_contrast;          /* weighted edge len of each patch     */
extern float	*prox;                   /* proximity index for each patch      */
extern float	cellsize;                /* size of cells in image (in meters)  */
extern float	extra_contrast;          /* contrast of patch types in border   */
extern float	edge_dist;               /* distance from edge for corearea (m) */
extern float	search_radius;           /* search radius for proximity index(m)*/
extern float	total_maxarea;           /* area of largest patch in landscape  */
extern float	total_shape;             /* shape index for the landscape       */
extern float	total_aw_shape;          /* area-wt shape index for landscape   */
extern float	total_fract;             /* fractal dimension for the landscape */
extern float	total_aw_fract;          /* area-wt fractal dim for landscape   */
extern float	total_core_index;        /* core area index for landscape       */
extern float	total_contrast;          /* total edge contrast in landscape    */
extern float	total_aw_contrast;       /* area-wt edge contrast in landscape  */
extern float	total_contrast_edge;     /* total weighted edge len in landscape*/
extern float	total_prox;              /* mean proximity index for landscape  */
extern float	edcon;                   /* total contrast of border patches    */
extern float	bound_wght;              /* default weight to apply to all edges*/
extern float	all_edge;                /* edge length - includes all edge     */
extern float	true_edge;               /* edge length - true edges only       */
extern float	lshape_edge;             /* edge length for landscape shape     */
extern float	iji_edge;                /* edge length for inter/jux index     */
  
extern double	*area;                   /* area of each patch in a class       */
extern double	*core_area;              /* core area of each patch in a class  */
extern double	*near_neigh_dist;        /* nearest neigh dist for each patch   */
extern double	*xx;                     /* array of log(perimeter) for DLFD    */
extern double	*yy;                     /* array of log(area) for DLFD         */
extern double	classarea;               /* total class area                    */
extern double	total_area;              /* area of the landscape               */
extern double	total_area_sq;           /* landscape area squared              */
extern double	total_core_area;         /* total core area in the landscape    */
extern double	total_core_sq;           /* landscape core area squared         */
extern double	total_nndist;            /* sum of near neigh dist in landscape */
extern double	total_nndist_sq;         /* nearest neighbor distance squared   */
