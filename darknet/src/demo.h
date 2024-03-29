#ifndef DEMO_H
#define DEMO_H
#include "image.h"

#ifdef __cplusplus
extern "C" {
#endif
void demo(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names, int classes,
    int frame_skip, char *prefix, char *out_filename, int mjpeg_port, int json_port, int dont_show, int ext_output);
void mld(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names, int classes, int frame_skip, const char * filename2);
void mld_model(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names,
    int classes, int frame_skip, const char * filename2);
void mld_last(char *cfgfile, char *cfgfile_model, char *weightfile, char *weightfile_model, float thresh, float hier_thresh, int cam_index, const char *filename, char **names, char **names_model,
    int classes, int classes_model, int frame_skip, const char * filename2);
#ifdef __cplusplus
}
#endif

#endif
