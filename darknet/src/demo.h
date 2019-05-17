#ifndef DEMO_H
#define DEMO_H
#include "image.h"
//insert code(연동 쓰레드 파라미터 구조체 선언)
typedef struct {
    char *cfgfile;
    char *weightfile;
    float thresh;
    float hier_thresh;
    int cam_index;
    const char *filename;
    char **names;
    int classes;
    int frame_skip;
    char *prefix;
    char *out_filename;
    int mjpeg_port;
    int json_port;
    int dont_show;
    int ext_output;
}ARGU;
ARGU setARGU(char *cfg, char *weight, float t, float h_thresh, int cam, const char *file, char **name, int cl, int frame, char*pre,
    char* out_file, int mjpeg, int json, int dont, int ext);
//insert code(연동 쓰레드 파라미터 구조체 선언)
#ifdef __cplusplus
extern "C" {
#endif
void demo(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names, int classes,
    int frame_skip, char *prefix, char *out_filename, int mjpeg_port, int json_port, int dont_show, int ext_output);
void mld(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names, int classes, int frame_skip, const char * filename2);
void mld_model(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names,
    int classes, int frame_skip, const char * filename2);
void demo2(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names, int classes,
    int frame_skip, char *prefix, char *out_filename, int mjpeg_port, int json_port, int dont_show, int ext_output);
void demo3(ARGU argu);
void demo4(ARGU argu);
#ifdef __cplusplus
}
#endif

#endif
