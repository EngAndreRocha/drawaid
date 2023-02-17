#ifndef IPL_HPP
#define IPL_HPP

#if WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#endif


#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "opencv2/objdetect.hpp"
#include "opencv2/face.hpp"

#include <vector>
#include <filesystem>

namespace ipl{

    class Screen{
        private:
            int scr_width;
            int scr_height;
            float resizing_factor;
        public:
            Screen(){
                #if WIN32
                    scr_width = (int)GetSystemMetrics(SM_CXSCREEN);
                    scr_height = (int)GetSystemMetrics(SM_CYSCREEN);
                #else
                    Display *disp = XOpenDisplay(NULL);
                    Screen *scrn = DefaultScreenOfDisplay(disp);
                    scr_width = scrn->width;
                    scr_height = scrn->height;
                #endif
                resizing_factor = 1.00;
            };
            ~Screen()
            {
                scr_width = 0;
                scr_height = 0;
            }
            void set_ScreenResolution(int width, int height);
            int get_scr_width();
            int get_scr_height();
    };
    class Faces{
        private:
            /* cv::CascadeClassifier faceDetector;
            cv::Ptr<cv::face::Facemark> facemark; */
            std::vector<cv::Rect> faces;
            std::vector<std::vector<cv::Point2f>> landmarks;
            std::vector<cv::Mat> faces_imgs;
            std::vector<cv::Mat> eyes_imgs;
            std::vector<cv::Mat> nose_imgs;
            std::vector<cv::Mat> mouth_imgs;
        public:
            Faces(){};
            std::vector<cv::Rect> get_Faces();
            std::vector<std::vector<cv::Point2f>> get_Landmarks();
            void set_Features(cv::Mat img, cv::CascadeClassifier faceDetector, cv::Ptr<cv::face::Facemark> facemark);
            std::vector<cv::Mat> get_FacesImgs();
            std::vector<cv::Mat> get_EyesImgs();
            std::vector<cv::Mat> get_NoseImgs();
            std::vector<cv::Mat> get_MouthImgs();
    };

    cv::Mat add_Grid(cv::Mat img, int grid_gap_in_pixels, cv::Scalar grid_color, int offset);
    cv::Mat adjust_ImageToScreen(cv::Mat img);
    cv::Mat posterize(const cv::Mat bgrmat, double lvls);
    cv::Mat simp_Face(cv::Mat img, bool fast);
    std::vector<cv::Mat> get_Highlights(cv::Mat img, int gap);
    bool path_exists(const std::filesystem::path& p, std::filesystem::file_status s = std::filesystem::file_status{});
    void save_ImagesResult(std::string filename, cv::Mat img, std::vector<int> compression_params);
    void process_Image(std::string filename, cv::Mat img, cv::CascadeClassifier faceDetector, cv::Ptr<cv::face::Facemark> facemark);
} // namespace hello

#endif //ipl
