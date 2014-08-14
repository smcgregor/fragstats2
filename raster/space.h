/*
 *  Include file space.h
 */
char	imagename[60];           /* name of input image file            */
char	out_file[60];            /* basename for output files           */
char	weight_file[60];         /* name of weight file                 */
char	id_file[60];             /* name of patch ID image              */
char	desc_file[60];           /* name of class descriptor file       */
char	file1[60];               /* name of basename.patch file         */
char	file2[60];               /* name of basename.class file         */
char	file3[60];               /* name of basename.land file          */
char	file4[60];               /* name of basename.full file          */
char	contrast_indices;        /* flag; calculate contrast indices    */
char	proximity;               /* flag; calculate proximity indices   */
char	id_image;                /* flag; ID image input                */
char	descriptors;             /* flag; descriptor file input         */
char	land_border;             /* flag; landscape border exists       */
char	do_nndist;               /* flag; calc nearest neigh indices    */
char	patch_stats;             /* flag; output patch statistics       */
char	class_stats;             /* flag; output class statistics       */
char	use_diags;               /* flag; use diagonals in patch finding*/
char	do_patchrich;            /* flag; calculate patch richness      */
char	*check;                  /* used in patch finding routine       */
char	**name;                  /* char descriptor for each class      */
char	bcode;                   /* flag; int, ext background present   */
  
short xpos[8] = {0,1,0,-1,1,1,-1,-1}; /* X,Y pos of 8 neighbors         */
short ypos[8] = {-1,0,1,0,-1,1,1,-1}; /* surrounding a cell             */
  
short	*image;                  /* pointer to input image data         */
short	*id;                     /* pointer to ID image                 */
short	*stackx;                 /* column location of patch cells      */
short	*stacky;                 /* row location of patch cells         */
short	*patchtype;              /* classes that exist in the landscape */
short	*patchid;                /* ID of each patch in the landscape   */
short	data_type;               /* file type of input image            */
short   background;              /* background value                    */
short	num_rows;                /* number of rows in input image       */
short	num_cols;                /* number of columns in input image    */
short	min_class;               /* min class (patch) value in image    */
short	max_class;               /* max class (patch) value in image    */
short	max_num_classes;         /* max class value possible            */
short	min_dist;                /* dist from edge in cells for corearea*/
short	min_wt;                  /* min class value in weight file      */
short	max_wt;                  /* max class value in weight file      */
short	num_wt;                  /* #classes in weight file             */
  
int	MAX_CLASSES;             /* max class value in image plus 1     */
int	MAX_PATCHES;             /* #patches in the landscape           */
int	MAX_PATCH_SIZE;          /* size of largest patch in landscape  */
int	NUM_CLASSES;             /* #classes in the landscape           */
int	total_num_classes;       /* #classes, including landscape border*/
int	num_patches;             /* #patches in a class                 */
int	total_patches;           /* #patches in the landscape           */
int	total_nn;                /* #patches in landscape with neighbors*/
int	boundary;                /* length of the landscape boundary    */
int	total_size;              /* total size of image in cells        */
int	total_core_patches;      /* total #corearea patches in landscape*/
int	total_num_patches;       /* total #patches in landscape         */
int	*edge;                   /* len of patch types bordering a patch*/
int	*edgetype;               /* len of patch types bordering a patch*/
int	*ext_npts;               /* #exterior cells for each patch      */
int	*core_area_patches;      /* #corearea patches for each patch    */
  
float	*perimeter;              /* perimeter of each patch             */
float	*core_area_index;        /* core area index of each patch       */
float	*shape;                  /* shape index of each patch           */
float	*fractal;                /* fractal dimension of each patch     */
float	*class_area;             /* area of the class being evaluated   */
float	*contrast;               /* weights of each combo of classes    */
float	*edge_contrast;          /* weighted edge len of each patch     */
float	*prox;                   /* proximity index for each patch      */
float	cellsize;                /* size of cells in image (in meters)  */
float	extra_contrast;          /* contrast of patch types in border   */
float	edge_dist;               /* distance from edge for corearea (m) */
float	search_radius;           /* search radius for proximity index(m)*/
float	total_maxarea;           /* area of largest patch in landscape  */
float	total_shape;             /* shape index for the landscape       */
float	total_aw_shape;          /* area-wt shape index for landscape   */
float	total_fract;             /* fractal dimension for the landscape */
float	total_aw_fract;          /* area-wt fractal dim for landscape   */
float	total_core_index;        /* core area index for landscape       */
float	total_contrast;          /* total edge contrast in landscape    */
float	total_aw_contrast;       /* area-wt edge contrast in landscape  */
float	total_contrast_edge;     /* total weighted edge len in landscape*/
float	total_prox;              /* mean proximity index for landscape  */
float	edcon;                   /* total contrast of border patches    */
float	bound_wght;              /* default weight to apply to all edges*/
float	all_edge;                /* edge length - includes all edge     */
float	true_edge;               /* edge length - true edges only       */
float	lshape_edge;             /* edge length for landscape shape     */
float	iji_edge;                /* edge length for inter/jux index     */
  
double	*area;                   /* area of each patch in a class       */
double	*core_area;              /* core area of each patch in a class  */
double	*near_neigh_dist;        /* nearest neigh dist for each patch   */
double	*xx;                     /* array of log(perimeter) for DLFD    */
double	*yy;                     /* array of log(area) for DLFD         */
double	classarea;               /* total class area                    */
double	total_area;              /* area of the landscape               */
double	total_area_sq;           /* landscape area squared              */
double	total_core_area;         /* total core area in the landscape    */
double	total_core_sq;           /* landscape core area squared         */
double	total_nndist;            /* sum of near neigh dist in landscape */
double	total_nndist_sq;         /* nearest neighbor distance squared   */
