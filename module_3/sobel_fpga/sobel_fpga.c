#include <libgimp/gimp.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

void sobel_max(uint8_t *image_out, uint8_t *image_in, uint32_t columns, uint32_t rows);

static void init(void);
static void quit(void);
static void query(void);
static void run(const gchar *name, gint nparams, const GimpParam *param, gint *nreturn_vals, GimpParam **return_vals);

static void time_measure(struct timespec *time);
static gdouble time_diff(struct timespec *start, struct timespec *end);

static void sobel_fpga (GimpDrawable *drawable);

GimpPlugInInfo PLUG_IN_INFO = {
	init,
	quit,
	query,
	run
};

MAIN()

static void init(void) {}
static void quit(void) {}

static void query(void) {
	static GimpParamDef args[] = {
		{
			GIMP_PDB_INT32,
			"run-mode",
			"Run mode"
		},{
			GIMP_PDB_IMAGE,
			"image",
			"Input image"
		},{
			GIMP_PDB_DRAWABLE,
			"drawable",
			"Input drawable"
		}
	};
	
	gimp_install_procedure (
		"sobel_fpga",
		"Sobel Edge Detection Operator on FPGA.",
		"Performs Sobel Operator on an Input Image.",
		"Achim Loesch",
		"Copyright Achim Loesch",
		"2017",
		"Sobel@FPGA...",
		"RGBA",
		GIMP_PLUGIN,
		G_N_ELEMENTS(args), 0,
		args, NULL
	);
	
	gimp_plugin_menu_register (
		"sobel_fpga",
		"<Image>/Filters/Misc"
	);
}

static void run(const gchar *name, gint nparams, const GimpParam *param, gint *nreturn_vals, GimpParam **return_vals) {
	static GimpParam  values[1];
	GimpPDBStatusType status = GIMP_PDB_SUCCESS;
	GimpRunMode       run_mode;
	GimpDrawable *drawable;
	
	*nreturn_vals	= 1;
	*return_vals	= values;

	values[0].type	= GIMP_PDB_STATUS;
	values[0].data.d_status = status;
	
	run_mode	= param[0].data.d_int32;
	
	drawable	= gimp_drawable_get(param[2].data.d_drawable);
	
	sobel_fpga(drawable);
	
	gimp_displays_flush();
	gimp_drawable_detach(drawable);
}

static void time_measure(struct timespec *time) {
	clock_gettime(CLOCK_REALTIME, time);
}

static gdouble time_diff(struct timespec *start, struct timespec *end) {
	struct timespec temp;
	gdouble diff = 0.0;
	
	if ((end->tv_nsec - start->tv_nsec)<0) {
		temp.tv_sec  =              end->tv_sec  - start->tv_sec  - 1;
		temp.tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
	} else {
		temp.tv_sec  = end->tv_sec  - start->tv_sec;
		temp.tv_nsec = end->tv_nsec - start->tv_nsec;
	}
	
	diff += (gdouble)temp.tv_sec;
	diff += (gdouble)temp.tv_nsec / 1000000000.0;
	
	return diff;
}

static void sobel_fpga(GimpDrawable *drawable) {
	struct timespec start;
	struct timespec end;
	gdouble diff	= 0.0;
	
	gint i, j, channels;
	gint x1, y1, x2, y2;
	GimpPixelRgn rgn_in, rgn_out;
	
	guchar *image_in	= NULL;
	guchar *image_out	= NULL;
	
	gimp_drawable_mask_bounds(drawable->drawable_id, &x1, &y1, &x2, &y2);
	
	gimp_pixel_rgn_init(&rgn_in,  drawable, x1, y1, x2-x1, y2-y1, FALSE, FALSE);
	gimp_pixel_rgn_init(&rgn_out, drawable, x1, y1, x2-x1, y2-y1,  TRUE,  TRUE);
	
	channels	= gimp_drawable_bpp(drawable->drawable_id);
	
	image_in	= g_new(guchar, x2 * y2 * channels);
	image_out	= g_new(guchar, x2 * y2 * channels);
	
	gimp_pixel_rgn_get_rect(&rgn_in, image_in, x1, y1, x2-x1, y2-y1);
	
	/*
	 * In order to iterate images use i and j to access pixels.
	 *
	 *  x1 <= j < x2
	 * /------------/ y1
	 * /            / <=
	 * /            / i
	 * /            / <
	 * /------------/ y2
	 *
	 */
	
	time_measure(&start);
	
	/*
	 * ADD CODE HERE.
	 */
	sobel_filter_new(image_out, image_in, y2, x2);
	
	time_measure(&end);
	diff = time_diff(&start, &end);
	
	gimp_pixel_rgn_set_rect(&rgn_out, image_out, x1, y1, x2-x1, y2-y1);
	
	gimp_drawable_flush(drawable);
	gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
	gimp_drawable_update(drawable->drawable_id, x1, y1, x2-x1, y2-y1);
	
	g_free(image_in);
	g_free(image_out);
	
	g_message("Execution time: %lf s.\n", diff);
}
