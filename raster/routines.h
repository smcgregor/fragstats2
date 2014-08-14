/*
 *  This file declares all the routines.
 */
void	allocate_memory();	
void	check_edges();
void    check_background();
void 	class_statistics();
void	free_memory();
void	get_char();
void	get_sizes();
void    head_class();
void	head_land();
void    head_patch();
void	landscape_edge_lengths();
void 	landscape_statistics();
void 	near_neigh_distance();
void 	patch_statistics();
void	prompt();
void 	read_ascii();
void 	read_binary();
void	read_classnames();
void    read_erdas();
void	read_header();
void	read_idrisi();
void	read_idrisi_desc();
void    read_image();
void 	read_svf();
void	read_weights();
void    regression();
void	set_char();
void	setfloat();
void	setint();
void	setshort();
void	setup();
void    space();
void	usage();
void	write_ascii();
void	write_binary();
void	write_erdas();
void	write_idrisi();
void	write_image();
void 	write_patch();
void	write_svf();

int	boundary_edge();
int	getint();
int  	get_patch();
int	num_core_area_patches();
int	patch_size();

short	getshort();

float	getfloat();
float	raster_contagion();

