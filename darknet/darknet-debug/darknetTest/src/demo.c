//insert code(result txt파일 출력)
#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#include "test.h"
#include <stdlib.h>
#include <time.h>
//#include <crtdbg.h>
//insert code(result txt파일 출력)

#include "network.h"
#include "detection_layer.h"
#include "region_layer.h"
#include "cost_layer.h"
#include "utils.h"
#include "parser.h"
#include "box.h"
#include "image.h"
#include "demo.h"
#ifdef WIN32
#include <time.h>
#include "gettimeofday.h"
#else
#include <sys/time.h>
#endif


#ifdef OPENCV
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/version.hpp>
#ifndef CV_VERSION_EPOCH
#include <opencv2/videoio/videoio_c.h>
#endif
#include "http_stream.h"
image get_image_from_stream(CvCapture *cap);

static char **demo_names;
static image **demo_alphabet;
static int demo_classes;

static int nboxes = 0;
static detection *dets = NULL;

static network net;
static image in_s ;
static image det_s;
static CvCapture * cap;
static int cpp_video_capture = 0;
static float fps = 0;
static float demo_thresh = 0;
static int demo_ext_output = 0;
static long long int frame_id = 0;
static int demo_json_port = -1;

static float* predictions[NFRAMES];
static int demo_index = 0;
static image images[NFRAMES];
static IplImage* ipl_images[NFRAMES];
static float *avg;

void draw_detections_cv_v3(IplImage* show_img, detection *dets, int num, float thresh, char **names, image **alphabet, int classes, int ext_outputF, FRAME_INFO *frame, int * count);
void draw_detections_cv_v33(IplImage* show_img, detection *dets, int num, float thresh, char **names, image **alphabet, int classes, int ext_output);
void draw_detections_cv_v3_carmodel(IplImage* show_img, detection *dets, int num, float thresh, char **names, image **alphabet,
    int classes, int ext_output, FRAME_INFO *frame);
void show_image_cv_ipl(IplImage *disp, const char *name);
void show_image_cv_ipl2(IplImage *disp, const char *name);
void save_cv_png(IplImage *img, const char *name);
void save_cv_jpg(IplImage *img, const char *name);
image get_image_from_stream_resize(CvCapture *cap, int w, int h, int c, IplImage** in_img, int cpp_video_capture, int dont_close);
image get_image_from_stream_resize2(CvCapture *cap, int w, int h, int c, IplImage** in_img, int cpp_video_capture, int dont_close);
image get_image_from_stream_letterbox(CvCapture *cap, int w, int h, int c, IplImage** in_img, int cpp_video_capture, int dont_close);
int get_stream_fps(CvCapture *cap, int cpp_video_capture);
IplImage* in_img;
IplImage* det_img;
IplImage* show_img;

static int flag_exit;
static int letter_box = 0;

//insert code(ip연동_변수 2개 생성)
static char **demo_names2;
static image **demo_alphabet2;
static int demo_classes2;

static int nboxes2 = 0;
static detection *dets2 = NULL;

static network net2;
static image in_s2;
static image det_s2;
static CvCapture * cap2;
static int cpp_video_capture2 = 0;
static float fps2 = 0;
static float demo_thresh2 = 0;
static int demo_ext_output2 = 0;
static long long int frame_id2 = 0;
static int demo_json_port2 = -1;

static float* predictions2[NFRAMES];
static int demo_index2 = 0;
static image images2[NFRAMES];
static IplImage* ipl_images2[NFRAMES];
static float *avg2;
IplImage* in_img2;
IplImage* det_img2;
IplImage* show_img2;

static int flag_exit2;
static int letter_box2 = 0;
//insert code(ip연동_변수 2개 생성)

void *fetch_in_thread(void *ptr)
{
    //in = get_image_from_stream(cap);
    int dont_close_stream = 0;    // set 1 if your IP-camera periodically turns off and turns on video-stream
    if(letter_box)
        in_s = get_image_from_stream_letterbox(cap, net.w, net.h, net.c, &in_img, cpp_video_capture, dont_close_stream);
    else
        in_s = get_image_from_stream_resize(cap, net.w, net.h, net.c, &in_img, cpp_video_capture, dont_close_stream);
    if(!in_s.data){
        //error("Stream closed.");
        printf("Stream closed.\n");
        flag_exit = 1;
        //exit(EXIT_FAILURE);
        return 0;
    }
    //in_s = resize_image(in, net.w, net.h);

    return 0;
}

void *detect_in_thread(void *ptr)
{
    layer l = net.layers[net.n-1];
    float *X = det_s.data;
    float *prediction = network_predict(net, X);

    memcpy(predictions[demo_index], prediction, l.outputs*sizeof(float));
    mean_arrays(predictions, NFRAMES, l.outputs, avg);
    l.output = avg;

    free_image(det_s);

    ipl_images[demo_index] = det_img;
    det_img = ipl_images[(demo_index + NFRAMES / 2 + 1) % NFRAMES];
    demo_index = (demo_index + 1) % NFRAMES;

    if (letter_box)
        dets = get_network_boxes(&net, in_img->width, in_img->height, demo_thresh, demo_thresh, 0, 1, &nboxes, 1); // letter box
    else
        dets = get_network_boxes(&net, net.w, net.h, demo_thresh, demo_thresh, 0, 1, &nboxes, 0); // resized

    return 0;
}

//insert code (ip연동 thread 복제)
void *fetch_in_thread2(void *ptr)
{
    printf("fetch_in_thread2 start\n");
    //in = get_image_from_stream(cap);
    int dont_close_stream = 0;    // set 1 if your IP-camera periodically turns off and turns on video-stream
    if (letter_box2)
        in_s2 = get_image_from_stream_letterbox(cap2, net2.w, net2.h, net2.c, &in_img2, cpp_video_capture2, dont_close_stream);
    else
        in_s2 = get_image_from_stream_resize2(cap2, net2.w, net2.h, net2.c, &in_img2, cpp_video_capture2, dont_close_stream);
    if (!in_s2.data) {
        //error("Stream closed.");
        printf("Stream closed.\n");
        flag_exit2 = 1;
        //exit(EXIT_FAILURE);
        return 0;
    }
    //in_s = resize_image(in, net.w, net.h);
    printf("fetch_in_thread2 end\n");
    return 0;
}

void *detect_in_thread2(void *ptr)
{
    layer l2 = net2.layers[net2.n - 1];
    float *X2 = det_s2.data;
    float *prediction2 = network_predict(net2, X2);

    memcpy(predictions2[demo_index2], prediction2, l2.outputs * sizeof(float));
    mean_arrays(predictions2, NFRAMES, l2.outputs, avg2);
    l2.output = avg2;

    free_image(det_s2);

    ipl_images2[demo_index2] = det_img2;
    det_img2 = ipl_images2[(demo_index2 + NFRAMES / 2 + 1) % NFRAMES];
    demo_index2 = (demo_index2 + 1) % NFRAMES;

    if (letter_box2)
        dets2 = get_network_boxes(&net2, in_img2->width, in_img2->height, demo_thresh2, demo_thresh2, 0, 1, &nboxes2, 1); // letter box
    else
        dets2 = get_network_boxes(&net2, net2.w, net2.h, demo_thresh2, demo_thresh2, 0, 1, &nboxes2, 0); // resized

    return 0;
}
//insert code (thread 복제)

double get_wall_time()
{
    struct timeval walltime;
    if (gettimeofday(&walltime, NULL)) {
        return 0;
    }
    return (double)walltime.tv_sec + (double)walltime.tv_usec * .000001;
}

void demo(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names, int classes,
    int frame_skip, char *prefix, char *out_filename, int mjpeg_port, int json_port, int dont_show, int ext_output)
{
    loadmysql();
    in_img = det_img = show_img = NULL;
    //skip = frame_skip;
    image **alphabet = load_alphabet();
    int delay = frame_skip;
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_ext_output = ext_output;
    demo_json_port = json_port;
    printf("Demo\n");
    net = parse_network_cfg_custom(cfgfile, 1, 1);    // set batch=1
    if(weightfile){
        load_weights(&net, weightfile);
    }
    //set_batch_network(&net, 1);
    fuse_conv_batchnorm(net);
    calculate_binary_weights(net);
    srand(2222222);

    if(filename){
        printf("video file: %s\n", filename);
        cpp_video_capture = 1;
        cap = get_capture_video_stream(filename);
    }else{
        printf("Webcam index: %d\n", cam_index);
        cpp_video_capture = 1;
        cap = get_capture_webcam(cam_index);
    }

    if (!cap) {
#ifdef WIN32
        printf("Check that you have copied file opencv_ffmpeg340_64.dll to the same directory where is darknet.exe \n");
#endif
        error("Couldn't connect to webcam.\n");
    }

    layer l = net.layers[net.n-1];
    int j;

    avg = (float *) calloc(l.outputs, sizeof(float));
    for(j = 0; j < NFRAMES; ++j) predictions[j] = (float *) calloc(l.outputs, sizeof(float));
    for(j = 0; j < NFRAMES; ++j) images[j] = make_image(1,1,3);

    if (l.classes != demo_classes) {
        printf("Parameters don't match: in cfg-file classes=%d, in data-file classes=%d \n", l.classes, demo_classes);
        getchar();
        exit(0);
    }

    flag_exit = 0;

    pthread_t fetch_thread;
    pthread_t detect_thread;

    fetch_in_thread(0);
    det_img = in_img;
    det_s = in_s;

    fetch_in_thread(0);
    detect_in_thread(0);
    det_img = in_img;
    det_s = in_s;

    for (j = 0; j < NFRAMES / 2; ++j) {
        fetch_in_thread(0);
        detect_in_thread(0);
        det_img = in_img;
        det_s = in_s;
    }

    int count = 0;
    if(!prefix && !dont_show){
        cvNamedWindow("Demo", CV_WINDOW_NORMAL);
        cvMoveWindow("Demo", 0, 0);
        cvResizeWindow("Demo", 1352, 1013);
    }

    CvVideoWriter* output_video_writer = NULL;    // cv::VideoWriter output_video;
    if (out_filename && !flag_exit)
    {
        CvSize size;
        size.width = det_img->width, size.height = det_img->height;
        int src_fps = 25;
        src_fps = get_stream_fps(cap, cpp_video_capture);

        //const char* output_name = "test_dnn_out.avi";
        //output_video_writer = cvCreateVideoWriter(out_filename, CV_FOURCC('H', '2', '6', '4'), src_fps, size, 1);
        output_video_writer = cvCreateVideoWriter(out_filename, CV_FOURCC('D', 'I', 'V', 'X'), src_fps, size, 1);
        //output_video_writer = cvCreateVideoWriter(out_filename, CV_FOURCC('M', 'J', 'P', 'G'), src_fps, size, 1);
        //output_video_writer = cvCreateVideoWriter(out_filename, CV_FOURCC('M', 'P', '4', 'V'), src_fps, size, 1);
        //output_video_writer = cvCreateVideoWriter(out_filename, CV_FOURCC('M', 'P', '4', '2'), src_fps, size, 1);
        //output_video_writer = cvCreateVideoWriter(out_filename, CV_FOURCC('X', 'V', 'I', 'D'), src_fps, size, 1);
        //output_video_writer = cvCreateVideoWriter(out_filename, CV_FOURCC('W', 'M', 'V', '2'), src_fps, size, 1);
    }

    double before = get_wall_time();

    //insert code(count 횟수증가 확인)
    int frame = 0;
    FRAME_NODE * list = NULL;
    int framecheck = 0;
    //insert code(count 횟수증가 확인)

    while(1){
        ++count;
        {
            if(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
            if(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");

            float nms = .45;    // 0.4F
            int local_nboxes = nboxes;
            detection *local_dets = dets;

            //if (nms) do_nms_obj(local_dets, local_nboxes, l.classes, nms);    // bad results
            if (nms) do_nms_sort(local_dets, local_nboxes, l.classes, nms);

            printf("\033[2J");
            printf("\033[1;1H");
            printf("\nFPS:%.1f\n", fps);
            printf("Objects:\n\n");

            ++frame_id;
            if (demo_json_port > 0) {
                int timeout = 400000;
                send_json(local_dets, local_nboxes, l.classes, demo_names, frame_id, demo_json_port, timeout);
            }

            //insert code(count 횟수증가 확인)
            //FILE *file = fopen("results/test.txt","a");
            printf("video frame number : %d\n", frame);
            FRAME_INFO newFrame;
            int countnumber = 0;
            memset(&newFrame.car, 0, sizeof(FRAME_INFO));

            //fprintf(file,"frame number : %d\n",frame);
            //fclose(file);

            draw_detections_cv_v3(show_img, local_dets, local_nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes, demo_ext_output, &newFrame, &countnumber);
            free_detections(local_dets, local_nboxes);

            frame++;
            if (countnumber >= 6) {
                printf("count check\n");
                sort_number(&newFrame);
                //정렬된 숫자 출력
                //for (int i = 0; i < 6; i++) {
                //    printf("%d", newFrame.car.full[i].num);
                //}
                //printf("\n");
                time_t timer;
                struct tm *t;
                timer = time(NULL);
                t = localtime(&timer);
                sprintf(newFrame.path, "C:\\Users\\cps435\\Desktop\\test\\darknetTest\\build_win_debug\\Debug\\results\\%d_%d_%d_%d_%d_%d_%d.jpg", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec, frame);
                sprintf(newFrame.time, "%d-%d-%d %d:%d:%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec);
                IplImage* copy_img = cvCreateImage(cvSize(show_img->width, show_img->height), show_img->depth, show_img->nChannels);
                cvCopy(show_img, copy_img, 0);
                newFrame.image = copy_img;
                FRAME_NODE * newNode = create_node(newFrame);
                //save_cv_jpg(newNode->data.image, newNode->data.path);
                insert_frame(&list, newNode);
                framecheck = 0;
            }
            else framecheck++;

            if (framecheck > 3) {
                printf("frame check\n");
                if (!list == NULL && !list->next == NULL) {
                    printf("list check\n");
                    int test = get_total_node(list);
                    int *carnumber = get_car_info(list, test);
                    char *car_model = get_car_model(list);
                    printf("number check\n");
                    FRAME_NODE *save_node = saveNode(&list, carnumber,car_model);
                    //printf("%s\n", save_node->data.path);
                    //printf("%d%d%d%d%d%d\n", save_node->data.car.full[0].num, save_node->data.car.full[1].num,
                    //    save_node->data.car.full[2].num, save_node->data.car.full[3].num,
                    //    save_node->data.car.full[4].num, save_node->data.car.full[5].num);
                    save_cv_jpg(save_node->data.image, save_node->data.path);
                    printf("db test\n");
                    //loadmysql();
                    if (insert_car(carnumber, save_node->data.time, save_node->data.path))
                        printf("insert error!!\n");
                    //closemysql();
                    printf("db close\n");
                    free(carnumber);
                    //print_list(&list);
                    releaselist(&list);
                    printf("Release list\n");
                    list = NULL;
                    printf("list null\n");
                }
            }

            if(!prefix){
                if (!dont_show) {
                    show_image_cv_ipl(show_img, "Demo");
                    int c = cvWaitKey(1);
                    if (c == 10) {
                        if (frame_skip == 0) frame_skip = 60;
                        else if (frame_skip == 4) frame_skip = 0;
                        else if (frame_skip == 60) frame_skip = 4;
                        else frame_skip = 0;
                    }
                    else if (c == 27 || c == 1048603) // ESC - exit (OpenCV 2.x / 3.x)
                    {
                        flag_exit = 1;
                    }
                }
            }else{
                char buff[256];
                sprintf(buff, "%s_%08d.jpg", prefix, count);
                if(show_img) save_cv_jpg(show_img, buff);
            }

            // if you run it with param -mjpeg_port 8090  then open URL in your web-browser: http://localhost:8090
            if (mjpeg_port > 0 && show_img) {
                int port = mjpeg_port;
                int timeout = 400000;
                int jpeg_quality = 40;    // 1 - 100
                send_mjpeg(show_img, port, timeout, jpeg_quality);
            }

            // save video file
            if (output_video_writer && show_img) {
                cvWriteFrame(output_video_writer, show_img);
                printf("\n cvWriteFrame \n");
            }
            cvReleaseImage(&show_img);

            pthread_join(fetch_thread, 0);
            pthread_join(detect_thread, 0);

            if (flag_exit == 1) break;

            if(delay == 0){
                show_img = det_img;
            }
            det_img = in_img;
            det_s = in_s;
        }
        --delay;
        if(delay < 0){
            delay = frame_skip;

            //double after = get_wall_time();
            //float curr = 1./(after - before);
            double after = get_time_point();    // more accurate time measurements
            float curr = 1000000. / (after - before);
            fps = curr;
            before = after;
        }
    }
    printf("input video stream closed. \n");
    if (output_video_writer) {
        cvReleaseVideoWriter(&output_video_writer);
        printf("output_video_writer closed. \n");
    }

    // free memory
    cvReleaseImage(&show_img);
    cvReleaseImage(&in_img);
    free_image(in_s);

    free(avg);
    for (j = 0; j < NFRAMES; ++j) free(predictions[j]);
    for (j = 0; j < NFRAMES; ++j) free_image(images[j]);

    free_ptrs((void **)names, net.layers[net.n - 1].classes);

    int i;
    const int nsize = 8;
    for (j = 0; j < nsize; ++j) {
        for (i = 32; i < 127; ++i) {
            free_image(alphabet[j][i]);
        }
        free(alphabet[j]);
    }
    free(alphabet);
    free_network(net);
    //cudaProfilerStop();

    closemysql();
}

void mld(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names,
    int classes, int frame_skip, const char * filename2)
{
    in_img = det_img = show_img = NULL;
    //skip = frame_skip;
    image **alphabet = load_alphabet();
    int delay = frame_skip;
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;

    in_img2 = det_img2 = show_img2 = NULL;
    //skip = frame_skip;
    image **alphabet2 = load_alphabet();
    int delay2 = frame_skip;
    demo_names2 = names;
    demo_alphabet2 = alphabet2;
    demo_classes2 = classes;
    demo_thresh2 = thresh;

    printf("Demo\n");
    net = parse_network_cfg_custom(cfgfile, 1, 1);    // set batch=1
    if (weightfile) {
        load_weights(&net, weightfile);
    }
    //set_batch_network(&net, 1);
    fuse_conv_batchnorm(net);
    calculate_binary_weights(net);
    net2 = parse_network_cfg_custom(cfgfile, 1, 1);    // set batch=1
    if (weightfile) {
        load_weights(&net2, weightfile);
    }
    //set_batch_network(&net, 1);
    fuse_conv_batchnorm(net2);
    calculate_binary_weights(net2);
    srand(2222222);

    /*
    if (filename) {
        printf("video file: %s\n", filename);
        cpp_video_capture = 1;
        cap = get_capture_video_stream(filename);
    }
    else {
        printf("Webcam index: %d\n", cam_index);
        cpp_video_capture = 1;
        cap = get_capture_webcam(cam_index);
    }

    if (filename2) {
        printf("video file: %s\n", filename2);
        cpp_video_capture2 = 1;
        cap2 = get_capture_video_stream(filename2);
    }
    else {
        printf("Webcam index: %d\n", cam_index);
        cpp_video_capture = 1;
        cap = get_capture_webcam(cam_index);
    }
    */


    if (!cap) {
#ifdef WIN32
        printf("Check that you have copied file opencv_ffmpeg340_64.dll to the same directory where is darknet.exe \n");
#endif
        error("Couldn't connect to ip cam.\n");
    }
    if (!cap2) {
#ifdef WIN32
        printf("Check that you have copied file opencv_ffmpeg340_64.dll to the same directory where is darknet.exe \n");
#endif
        error("Couldn't connect to webcam.\n");
    }

    layer l = net.layers[net.n - 1];
    int j;
    layer l2 = net2.layers[net2.n - 1];
    int j2;

    avg = (float *)calloc(l.outputs, sizeof(float));
    for (j = 0; j < NFRAMES; ++j) predictions[j] = (float *)calloc(l.outputs, sizeof(float));
    for (j = 0; j < NFRAMES; ++j) images[j] = make_image(1, 1, 3);

    avg2 = (float *)calloc(l2.outputs, sizeof(float));
    for (j2 = 0; j2 < NFRAMES; ++j2) predictions2[j2] = (float *)calloc(l2.outputs, sizeof(float));
    for (j2 = 0; j2 < NFRAMES; ++j2) images2[j2] = make_image(1, 1, 3);

    if (l.classes != demo_classes) {
        printf("Parameters don't match: in cfg-file classes=%d, in data-file classes=%d \n", l.classes, demo_classes);
        getchar();
        exit(0);
    }

    if (l2.classes != demo_classes2) {
        printf("Parameters don't match: in cfg-file classes=%d, in data-file classes=%d \n", l2.classes, demo_classes2);
        getchar();
        exit(0);
    }


    flag_exit = 0;

    pthread_t fetch_thread;
    pthread_t detect_thread;

    fetch_in_thread(0);
    det_img = in_img;
    det_s = in_s;

    fetch_in_thread(0);
    detect_in_thread(0);
    det_img = in_img;
    det_s = in_s;

    for (j = 0; j < NFRAMES / 2; ++j) {
        fetch_in_thread(0);
        detect_in_thread(0);
        det_img = in_img;
        det_s = in_s;
    }

    flag_exit2 = 0;

    pthread_t fetch_thread2;
    pthread_t detect_thread2;

    fetch_in_thread2(0);
    det_img2 = in_img2;
    det_s2 = in_s2;

    fetch_in_thread2(0);
    detect_in_thread2(0);
    det_img2 = in_img2;
    det_s2 = in_s2;

    for (j = 0; j < NFRAMES / 2; ++j) {
        fetch_in_thread2(0);
        detect_in_thread2(0);
        det_img2 = in_img2;
        det_s2 = in_s2;
    }

    int count = 0;

    /*
    cvNamedWindow("Demo", CV_WINDOW_NORMAL);
    cvMoveWindow("Demo", 0, 0);
    cvResizeWindow("Demo", 1352, 1013);

    cvNamedWindow("test", CV_WINDOW_NORMAL);
    cvMoveWindow("test", 100, 150);
    cvResizeWindow("test", 1352, 1013);
    */

    double before = get_wall_time();
    double before2 = get_wall_time();

    int frame = 0;
    printf("while comein\n");
    while (1) {
        ++count;
        {
            if (pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
            if (pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");

            if (pthread_create(&fetch_thread2, 0, fetch_in_thread2, 0)) error("Thread creation failed");
            if (pthread_create(&detect_thread2, 0, detect_in_thread2, 0)) error("Thread creation failed");

            float nms = .45;    // 0.4F
            int local_nboxes = nboxes;
            detection *local_dets = dets;

            float nms2 = .45;    // 0.4F
            int local_nboxes2 = nboxes2;
            detection *local_dets2 = dets2;
            printf("local_nboxes : %d, l.classes : %d, nms : %d\n", local_nboxes, l.classes, nms);
            printf("local_nboxes2 : %d, l.classes2 : %d, nms2 : %d\n", local_nboxes2, l2.classes, nms2);
            printf("123\n");
            //if (nms) do_nms_obj(local_dets, local_nboxes, l.classes, nms);    // bad results
            if (nms) do_nms_sort(local_dets, local_nboxes, l.classes, nms);

            if (nms2) do_nms_sort(local_dets2, local_nboxes2, l2.classes, nms2);
            printf("1234\n");

            printf("\033[2J");
            printf("\033[1;1H");
            printf("\nFPS:%.1f\n", fps);
            printf("Objects:\n\n");


            ++frame_id;
            ++frame_id2;

            printf("video frame number : %d\n", frame);
            draw_detections_cv_v33(show_img, local_dets, local_nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes, demo_ext_output);
            free_detections(local_dets, local_nboxes);

            draw_detections_cv_v33(show_img2, local_dets2, local_nboxes2, demo_thresh2, demo_names2, demo_alphabet2, demo_classes2, demo_ext_output2);
            free_detections(local_dets2, local_nboxes2);
            frame++;

            if(frame>4){
            char buff[256], buff2[256];
            sprintf(buff, "C:\\Users\\cps435\\Desktop\\test\\darknetTest\\build_win_debug\\Debug\\results\\%d_%d.jpg", 1,frame);
            sprintf(buff2, "C:\\Users\\cps435\\Desktop\\test\\darknetTest\\build_win_debug\\Debug\\results\\%d_%d.jpg", 2,frame);
            save_cv_jpg(show_img, buff);
            save_cv_jpg(show_img2, buff2);
            }

            //show_image_cv_ipl(show_img, "Demo");
            //show_image_cv_ipl2(show_img2, "test");
            printf("can you come here22?\n");
            cvReleaseImage(&show_img);
            cvReleaseImage(&show_img2);

            pthread_join(fetch_thread, 0);
            pthread_join(detect_thread, 0);

            pthread_join(fetch_thread2, 0);
            pthread_join(detect_thread2, 0);

            if (flag_exit == 1) break;
            if (flag_exit2 == 1) break;

            if (delay == 0) {
                show_img = det_img;
            }
            det_img = in_img;
            det_s = in_s;

            if (delay2 == 0) {
                show_img2 = det_img2;
            }
            det_img2 = in_img2;
            det_s2 = in_s2;
        }

        --delay;
        --delay2;

        if (delay < 0) {
            delay = frame_skip;

            //double after = get_wall_time();
            //float curr = 1./(after - before);
            double after = get_time_point();    // more accurate time measurements
            float curr = 1000000. / (after - before);
            fps = curr;
            before = after;
        }

        if (delay2 < 0) {
            delay2 = frame_skip;

            //double after = get_wall_time();
            //float curr = 1./(after - before);
            double after2 = get_time_point();    // more accurate time measurements
            float curr2 = 1000000. / (after2 - before2);
            fps2 = curr2;
            before2 = after2;
        }
    }

    printf("input video stream closed. \n");
    /*
    // free memory
    cvReleaseImage(&show_img);
    cvReleaseImage(&in_img);
    free_image(in_s);
    cvReleaseImage(&show_img2);
    cvReleaseImage(&in_img2);
    free_image(in_s2);

    free(avg);
    for (j = 0; j < NFRAMES; ++j) free(predictions[j]);
    for (j = 0; j < NFRAMES; ++j) free_image(images[j]);
    free(avg2);
    for (j = 0; j < NFRAMES; ++j) free(predictions2[j]);
    for (j = 0; j < NFRAMES; ++j) free_image(images2[j]);


    free_ptrs((void **)names, net.layers[net.n - 1].classes);
    free_ptrs((void **)names, net2.layers[net2.n - 1].classes);

    int i;
    const int nsize = 8;
    for (j = 0; j < nsize; ++j) {
        for (i = 32; i < 127; ++i) {
            free_image(alphabet[j][i]);
        }
        free(alphabet[j]);
    }
    free(alphabet);
    free_network(net);

    for (j = 0; j < nsize; ++j) {
        for (i = 32; i < 127; ++i) {
            free_image(alphabet2[j][i]);
        }
        free(alphabet2[j]);
    }
    free(alphabet2);
    free_network(net2);

    //cudaProfilerStop();
    */
}

void mld_model(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names,
    int classes, int frame_skip, const char * filename2)
{
    in_img = det_img = show_img = NULL;
    //skip = frame_skip;
    image **alphabet = load_alphabet();
    int delay = frame_skip;
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;

    in_img2 = det_img2 = show_img2 = NULL;
    //skip = frame_skip;
    image **alphabet2 = load_alphabet();
    int delay2 = frame_skip;
    demo_names2 = names;
    demo_alphabet2 = alphabet2;
    demo_classes2 = classes;
    demo_thresh2 = thresh;

    printf("Demo\n");
    net = parse_network_cfg_custom(cfgfile, 1, 1);    // set batch=1
    if (weightfile) {
        load_weights(&net, weightfile);
    }
    //set_batch_network(&net, 1);
    fuse_conv_batchnorm(net);
    calculate_binary_weights(net);
    net2 = parse_network_cfg_custom(cfgfile, 1, 1);    // set batch=1
    if (weightfile) {
        load_weights(&net2, weightfile);
    }
    //set_batch_network(&net, 1);
    fuse_conv_batchnorm(net2);
    calculate_binary_weights(net2);
    srand(2222222);

    if (filename) {
        printf("video file: %s\n", filename);
        cpp_video_capture = 1;
        cap = get_capture_video_stream(filename);
    }
    else {
        printf("Webcam index: %d\n", cam_index);
        cpp_video_capture = 1;
        cap = get_capture_webcam(cam_index);
    }

    if (filename2) {
        printf("video file: %s\n", filename2);
        cpp_video_capture2 = 1;
        cap2 = get_capture_video_stream(filename2);
    }

    if (!cap) {
#ifdef WIN32
        printf("Check that you have copied file opencv_ffmpeg340_64.dll to the same directory where is darknet.exe \n");
#endif
        error("Couldn't connect to webcam.\n");
    }
    if (!cap2) {
#ifdef WIN32
        printf("Check that you have copied file opencv_ffmpeg340_64.dll to the same directory where is darknet.exe \n");
#endif
        error("Couldn't connect to webcam.\n");
    }

    layer l = net.layers[net.n - 1];
    int j;
    layer l2 = net2.layers[net2.n - 1];
    int j2;

    avg = (float *)calloc(l.outputs, sizeof(float));
    for (j = 0; j < NFRAMES; ++j) predictions[j] = (float *)calloc(l.outputs, sizeof(float));
    for (j = 0; j < NFRAMES; ++j) images[j] = make_image(1, 1, 3);

    avg2 = (float *)calloc(l2.outputs, sizeof(float));
    for (j2 = 0; j2 < NFRAMES; ++j2) predictions2[j2] = (float *)calloc(l2.outputs, sizeof(float));
    for (j2 = 0; j2 < NFRAMES; ++j2) images2[j2] = make_image(1, 1, 3);

    if (l.classes != demo_classes) {
        printf("Parameters don't match: in cfg-file classes=%d, in data-file classes=%d \n", l.classes, demo_classes);
        getchar();
        exit(0);
    }

    if (l2.classes != demo_classes2) {
        printf("Parameters don't match: in cfg-file classes=%d, in data-file classes=%d \n", l2.classes, demo_classes2);
        getchar();
        exit(0);
    }

    flag_exit = 0;

    pthread_t fetch_thread;
    pthread_t detect_thread;

    fetch_in_thread(0);
    det_img = in_img;
    det_s = in_s;

    fetch_in_thread(0);
    detect_in_thread(0);
    det_img = in_img;
    det_s = in_s;

    for (j = 0; j < NFRAMES / 2; ++j) {
        fetch_in_thread(0);
        detect_in_thread(0);
        det_img = in_img;
        det_s = in_s;
    }

    flag_exit2 = 0;

    pthread_t fetch_thread2;
    pthread_t detect_thread2;

    fetch_in_thread2(0);
    det_img2 = in_img2;
    det_s2 = in_s2;

    fetch_in_thread2(0);
    detect_in_thread2(0);
    det_img2 = in_img2;
    det_s2 = in_s2;

    for (j = 0; j < NFRAMES / 2; ++j) {
        fetch_in_thread2(0);
        detect_in_thread2(0);
        det_img2 = in_img2;
        det_s2 = in_s2;
    }

    int count = 0;

    /*
    cvNamedWindow("Demo", CV_WINDOW_NORMAL);
    cvMoveWindow("Demo", 0, 0);
    cvResizeWindow("Demo", 1352, 1013);

    cvNamedWindow("test", CV_WINDOW_NORMAL);
    cvMoveWindow("test", 100, 150);
    cvResizeWindow("test", 1352, 1013);
    */

    double before = get_wall_time();
    double before2 = get_wall_time();

    int frame = 0;
    FRAME_NODE * list = NULL;
    int framecheck = 0;

    while (1) {
        ++count;
        {
            if (pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
            if (pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");

            if (pthread_create(&fetch_thread2, 0, fetch_in_thread2, 0)) error("Thread creation failed");
            if (pthread_create(&detect_thread2, 0, detect_in_thread2, 0)) error("Thread creation failed");

            float nms = .45;    // 0.4F
            int local_nboxes = nboxes;
            detection *local_dets = dets;

            float nms2 = .45;    // 0.4F
            int local_nboxes2 = nboxes2;
            detection *local_dets2 = dets2;

            //if (nms) do_nms_obj(local_dets, local_nboxes, l.classes, nms);    // bad results
            if (nms) do_nms_sort(local_dets, local_nboxes, l.classes, nms);

            if (nms2) do_nms_sort(local_dets2, local_nboxes2, l2.classes, nms2);
            printf("1234\n");

            printf("\033[2J");
            printf("\033[1;1H");
            printf("\nFPS:%.1f\n", fps);
            printf("Objects:\n\n");


            ++frame_id;
            ++frame_id2;

            printf("video frame number : %d\n", frame);
            FRAME_INFO newFrame;
            int countnumber = 0;
            memset(&newFrame.car, 0, sizeof(CAR));

            draw_detections_cv_v3(show_img, local_dets, local_nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes, demo_ext_output, &newFrame, &countnumber);
            free_detections(local_dets, local_nboxes);

            draw_detections_cv_v3_carmodel(show_img2, local_dets2, local_nboxes2, demo_thresh2, demo_names2, demo_alphabet2, demo_classes2, demo_ext_output2, &newFrame);
            free_detections(local_dets2, local_nboxes2);
            frame++;

            if (countnumber >= 6) {
                printf("count check\n");
                sort_number(&newFrame);
                //정렬된 숫자 출력
                //for (int i = 0; i < 6; i++) {
                //    printf("%d", newFrame.car.full[i].num);
                //}
                //printf("\n");
                time_t timer;
                struct tm *t;
                timer = time(NULL);
                t = localtime(&timer);
                sprintf(newFrame.path, "results/%d_%d_%d_%d_%d_%d_%d.jpg", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec, frame);
                sprintf(newFrame.path_model, "results/model_%d_%d_%d_%d_%d_%d_%d.jpg", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec, frame);
                sprintf(newFrame.time, "%d-%d-%d %d:%d:%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec);
                IplImage* copy_img = cvCreateImage(cvSize(show_img->width, show_img->height), show_img->depth, show_img->nChannels);
                IplImage* copy_img_model = cvCreateImage(cvSize(show_img2->width, show_img2->height), show_img2->depth, show_img2->nChannels);
                cvCopy(show_img, copy_img, 0);
                cvCopy(show_img2, copy_img_model, 0);
                newFrame.image = copy_img;
                newFrame.image_model = copy_img_model;
                FRAME_NODE * newNode = create_node(newFrame);
                //save_cv_jpg(newNode->data.image, newNode->data.path);
                insert_frame(&list, newNode);
                framecheck = 0;
            }
            else framecheck++;

            if (framecheck > 3) {
                printf("frame check\n");
                if (!list == NULL && !list->next == NULL) {
                    printf("list check\n");
                    int test = get_total_node(list);
                    int *carnumber = get_car_info(list, test);
                    char *car_model = get_car_model(list);
                    FRAME_NODE *save_node = saveNode(&list, carnumber, car_model);
                    //printf("%s\n", save_node->data.path);
                    //printf("%d%d%d%d%d%d\n", save_node->data.car.full[0].num, save_node->data.car.full[1].num,
                    //    save_node->data.car.full[2].num, save_node->data.car.full[3].num,
                    //    save_node->data.car.full[4].num, save_node->data.car.full[5].num);
                    save_cv_jpg(save_node->data.image, save_node->data.path);
                    save_cv_jpg(save_node->data.image_model, save_node->data.path_model);
                    //printf("db test\n");
                    //if (insert_car(carnumber, save_node->data.time, save_node->data.path))
                    //    printf("insert error!!\n");
                    //printf("db close\n");
                    //print_list(&list);
                    releaselist(&list);
                    printf("Release list\n");
                    list = NULL;
                    printf("list null\n");
                }
            }

            //show_image_cv_ipl(show_img, "Demo");
            //show_image_cv_ipl2(show_img2, "test");
            printf("can you come here22?\n");
            cvReleaseImage(&show_img);
            cvReleaseImage(&show_img2);

            pthread_join(fetch_thread, 0);
            pthread_join(detect_thread, 0);

            pthread_join(fetch_thread2, 0);
            pthread_join(detect_thread2, 0);

            if (flag_exit == 1) break;
            if (flag_exit2 == 1) break;

            if (delay == 0) {
                show_img = det_img;
            }
            det_img = in_img;
            det_s = in_s;

            if (delay2 == 0) {
                show_img2 = det_img2;
            }
            det_img2 = in_img2;
            det_s2 = in_s2;
        }

        --delay;
        --delay2;

        if (delay < 0) {
            delay = frame_skip;

            //double after = get_wall_time();
            //float curr = 1./(after - before);
            double after = get_time_point();    // more accurate time measurements
            float curr = 1000000. / (after - before);
            fps = curr;
            before = after;
        }

        if (delay2 < 0) {
            delay2 = frame_skip;

            //double after = get_wall_time();
            //float curr = 1./(after - before);
            double after2 = get_time_point();    // more accurate time measurements
            float curr2 = 1000000. / (after2 - before2);
            fps2 = curr2;
            before2 = after2;
        }
    }

    printf("input video stream closed. \n");
}
void mld_last(char *cfgfile, char *cfgfile_model,char *weightfile,char *weightfile_model, float thresh, float hier_thresh, int cam_index, const char *filename, char **names,char **names_model,
    int classes,int classes_model, int frame_skip, const char * filename2)
{
    in_img = det_img = show_img = NULL;
    //skip = frame_skip;
    image **alphabet = load_alphabet();
    int delay = frame_skip;
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;

    in_img2 = det_img2 = show_img2 = NULL;
    //skip = frame_skip;
    image **alphabet2 = load_alphabet();
    int delay2 = frame_skip;
    demo_names2 = names_model;
    demo_alphabet2 = alphabet2;
    demo_classes2 = classes_model;
    demo_thresh2 = thresh;

    printf("MLD_LAST\n");
    net = parse_network_cfg_custom(cfgfile, 1, 1);    // set batch=1
    if (weightfile) {
        load_weights(&net, weightfile);
    }
    //set_batch_network(&net, 1);
    fuse_conv_batchnorm(net);
    calculate_binary_weights(net);
    net2 = parse_network_cfg_custom(cfgfile_model, 1, 1);    // set batch=1
    if (weightfile_model) {
        load_weights(&net2, weightfile_model);
    }
    //set_batch_network(&net, 1);
    fuse_conv_batchnorm(net2);
    calculate_binary_weights(net2);
    srand(2222222);

    if (filename) {
        printf("video file: %s\n", filename);
        cpp_video_capture = 1;
        cap = get_capture_video_stream(filename);
    }
    else {
        printf("Webcam index: %d\n", cam_index);
        cpp_video_capture = 1;
        cap = get_capture_webcam(cam_index);
    }

    if (filename2) {
        printf("video file: %s\n", filename2);
        cpp_video_capture2 = 1;
        cap2 = get_capture_video_stream(filename2);
    }
    else {
        printf("Webcam index: %d\n", cam_index);
        cpp_video_capture = 1;
        cap2 = get_capture_webcam(cam_index);
    }

    if (!cap) {
#ifdef WIN32
        printf("Check that you have copied file opencv_ffmpeg340_64.dll to the same directory where is darknet.exe \n");
#endif
        error("Couldn't connect to filename.\n");
    }
    if (!cap2) {
#ifdef WIN32
        printf("Check that you have copied file opencv_ffmpeg340_64.dll to the same directory where is darknet.exe \n");
#endif
        error("Couldn't connect to webcam.\n");
    }

    layer l = net.layers[net.n - 1];
    int j;
    layer l2 = net2.layers[net2.n - 1];
    int j2;

    avg = (float *)calloc(l.outputs, sizeof(float));
    for (j = 0; j < NFRAMES; ++j) predictions[j] = (float *)calloc(l.outputs, sizeof(float));
    for (j = 0; j < NFRAMES; ++j) images[j] = make_image(1, 1, 3);

    avg2 = (float *)calloc(l2.outputs, sizeof(float));
    for (j2 = 0; j2 < NFRAMES; ++j2) predictions2[j2] = (float *)calloc(l2.outputs, sizeof(float));
    for (j2 = 0; j2 < NFRAMES; ++j2) images2[j2] = make_image(1, 1, 3);

    if (l.classes != demo_classes) {
        printf("Parameters don't match: in cfg-file classes=%d, in data-file classes=%d \n", l.classes, demo_classes);
        getchar();
        exit(0);
    }

    if (l2.classes != demo_classes2) {
        printf("Parameters don't match: in cfg-file classes=%d, in data-file classes=%d \n", l2.classes, demo_classes2);
        getchar();
        exit(0);
    }

    flag_exit = 0;

    pthread_t fetch_thread;
    pthread_t detect_thread;

    fetch_in_thread(0);
    det_img = in_img;
    det_s = in_s;

    fetch_in_thread(0);
    detect_in_thread(0);
    det_img = in_img;
    det_s = in_s;

    for (j = 0; j < NFRAMES / 2; ++j) {
        fetch_in_thread(0);
        detect_in_thread(0);
        det_img = in_img;
        det_s = in_s;
    }

    flag_exit2 = 0;

    pthread_t fetch_thread2;
    pthread_t detect_thread2;

    fetch_in_thread2(0);
    det_img2 = in_img2;
    det_s2 = in_s2;

    fetch_in_thread2(0);
    detect_in_thread2(0);
    det_img2 = in_img2;
    det_s2 = in_s2;

    for (j = 0; j < NFRAMES / 2; ++j) {
        fetch_in_thread2(0);
        detect_in_thread2(0);
        det_img2 = in_img2;
        det_s2 = in_s2;
    }

    int count = 0;

    
    cvNamedWindow("Demo", CV_WINDOW_NORMAL);
    cvMoveWindow("Demo", 0, 0);
    cvResizeWindow("Demo", 1352, 1013);

    //cvNamedWindow("test", CV_WINDOW_NORMAL);
    //cvMoveWindow("test", 100, 150);
    //cvResizeWindow("test", 1352, 1013);
    

    double before = get_wall_time();
    double before2 = get_wall_time();

    int frame = 0;
    FRAME_NODE * list = NULL;
    int framecheck = 0;

    while (1) {
        ++count;
        {
            if (pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
            if (pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");

            if (pthread_create(&fetch_thread2, 0, fetch_in_thread2, 0)) error("Thread creation failed");
            if (pthread_create(&detect_thread2, 0, detect_in_thread2, 0)) error("Thread creation failed");

            float nms = .45;    // 0.4F
            int local_nboxes = nboxes;
            detection *local_dets = dets;

            float nms2 = .45;    // 0.4F
            int local_nboxes2 = nboxes2;
            detection *local_dets2 = dets2;

            //if (nms) do_nms_obj(local_dets, local_nboxes, l.classes, nms);    // bad results
            if (nms) do_nms_sort(local_dets, local_nboxes, l.classes, nms);

            if (nms2) do_nms_sort(local_dets2, local_nboxes2, l2.classes, nms2);
            printf("1234\n");

            printf("\033[2J");
            printf("\033[1;1H");
            printf("\nFPS:%.1f\n", fps);
            printf("Objects:\n\n");


            ++frame_id;
            ++frame_id2;

            printf("video frame number : %d\n", frame);
            FRAME_INFO newFrame;
            int countnumber = 0;
            memset(&newFrame.car, 0, sizeof(CAR));

            draw_detections_cv_v3(show_img, local_dets, local_nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes, demo_ext_output, &newFrame, &countnumber);
            free_detections(local_dets, local_nboxes);

            draw_detections_cv_v3_carmodel(show_img2, local_dets2, local_nboxes2, demo_thresh2, demo_names2, demo_alphabet2, demo_classes2, demo_ext_output2, &newFrame);
            free_detections(local_dets2, local_nboxes2);
            frame++;

            if (countnumber >= 6) {
                printf("count check\n");
                sort_number(&newFrame);
                //정렬된 숫자 출력
                //for (int i = 0; i < 6; i++) {
                //    printf("%d", newFrame.car.full[i].num);
                //}
                //printf("\n");
                time_t timer;
                struct tm *t;
                timer = time(NULL);
                t = localtime(&timer);
                sprintf(newFrame.path, "C:\\Users\\cps435\\Desktop\\test\\darknetTest\\build_win_debug\\Debug\\results\\%d_%d_%d_%d_%d_%d_%d.jpg", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec, frame);
                sprintf(newFrame.path_model, "C:\\Users\\cps435\\Desktop\\test\\darknetTest\\build_win_debug\\Debug\\results\\model_%d_%d_%d_%d_%d_%d_%d.jpg", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec, frame);
                sprintf(newFrame.time, "%d-%d-%d %d:%d:%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec);
                IplImage* copy_img = cvCreateImage(cvSize(show_img->width, show_img->height), show_img->depth, show_img->nChannels);
                IplImage* copy_img_model = cvCreateImage(cvSize(show_img2->width, show_img2->height), show_img2->depth, show_img2->nChannels);
                cvCopy(show_img, copy_img, 0);
                cvCopy(show_img2, copy_img_model, 0);
                newFrame.image = copy_img;
                newFrame.image_model = copy_img_model;
                FRAME_NODE * newNode = create_node(newFrame);
                //save_cv_jpg(newNode->data.image, newNode->data.path);
                insert_frame(&list, newNode);
                framecheck = 0;
            }
            else framecheck++;

            if (framecheck > 5) {
                printf("frame check\n");
                if (!list == NULL && !list->next == NULL) {
                    printf("list check\n");
                    int test = get_total_node(list);
                    int *carnumber = get_car_info(list, test);
                    char *car_model = get_car_model(list);
                    FRAME_NODE *save_node = saveNode(&list, carnumber, car_model);
                    save_cv_jpg(save_node->data.image, save_node->data.path);
                    save_cv_jpg(save_node->data.image_model, save_node->data.path_model);
                    printf("carnumber : %d%d%d%d%d%d, model : %s, time : %s, path_number : %s, path_model : %s\n", carnumber[0], carnumber[1], carnumber[2], carnumber[3], carnumber[4], carnumber[5],
                        save_node->data.car.model.name, save_node->data.time, save_node->data.path, save_node->data.path_model);
                    free(carnumber);
                    free(car_model);
                    releaselist(&list);
                    printf("Release list\n");
                    list = NULL;
                    printf("list null\n");
                }
            }

            show_image_cv_ipl(show_img, "Demo");
            //show_image_cv_ipl2(show_img2, "test");
            printf("can you come here22?\n");
            cvReleaseImage(&show_img);
            cvReleaseImage(&show_img2);

            pthread_join(fetch_thread, 0);
            pthread_join(detect_thread, 0);

            pthread_join(fetch_thread2, 0);
            pthread_join(detect_thread2, 0);

            if (flag_exit == 1) break;
            if (flag_exit2 == 1) break;

            if (delay == 0) {
                show_img = det_img;
            }
            det_img = in_img;
            det_s = in_s;

            if (delay2 == 0) {
                show_img2 = det_img2;
            }
            det_img2 = in_img2;
            det_s2 = in_s2;
        }

        --delay;
        --delay2;

        if (delay < 0) {
            delay = frame_skip;

            //double after = get_wall_time();
            //float curr = 1./(after - before);
            double after = get_time_point();    // more accurate time measurements
            float curr = 1000000. / (after - before);
            fps = curr;
            before = after;
        }

        if (delay2 < 0) {
            delay2 = frame_skip;

            //double after = get_wall_time();
            //float curr = 1./(after - before);
            double after2 = get_time_point();    // more accurate time measurements
            float curr2 = 1000000. / (after2 - before2);
            fps2 = curr2;
            before2 = after2;
        }
    }

    printf("input video stream closed. \n");
}
#else
void demo(char *cfgfile, char *weightfile, float thresh, float hier_thresh, int cam_index, const char *filename, char **names, int classes,
    int frame_skip, char *prefix, char *out_filename, int mjpeg_port, int json_port, int dont_show, int ext_output)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif
