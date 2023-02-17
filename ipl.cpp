#include "ipl.hpp"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

#include "opencv2/objdetect.hpp"
#include "opencv2/face.hpp"
#include <cmath>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <filesystem>

void ipl::Screen::set_ScreenResolution(int width, int height)
{
    scr_width = width;
    scr_height = height;
}
int ipl::Screen::get_scr_width()
{
    return scr_width;
}
int ipl::Screen::get_scr_height()
{
    return scr_height;
}

std::vector<cv::Rect> ipl::Faces::get_Faces()
{
    return faces;
}
std::vector<std::vector<cv::Point2f>> ipl::Faces::get_Landmarks()
{
    return landmarks;
}
void ipl::Faces::set_Features(cv::Mat img, cv::CascadeClassifier faceDetector, cv::Ptr<cv::face::Facemark> facemark)
{
    /* cv::CascadeClassifier faceDetector;
    cv::Ptr<cv::face::Facemark> facemark = cv::face::FacemarkLBF::create();
    faceDetector.load(face_cascade);
    std::cout<<"Loaded face cascade successfully!"<<std::endl;
    facemark->loadModel(model);
    std::cout<<"Loaded model successfully!"<<std::endl; */

    cv::Mat gray;
    std::vector<cv::Mat> img_vector;
    if(img.channels()>1){
        std::cout<<"Converting image to grayscale duplicate for processing..."<<std::endl;
        cvtColor(img,gray,cv::COLOR_BGR2GRAY);
    }
    else{
        std::cout<<"Image already in grayscale, cloning for processing..."<<std::endl;
        gray = img.clone();
    }
    std::cout<<"Equalizing grayscale duplicate for processing..."<<std::endl;
    equalizeHist( gray, gray );
    std::cout<<"Resizing grayscale duplicate to one fourth the size for processing..."<<std::endl;
    cv::resize(gray, gray, cv::Size(), 0.25, 0.25);

    std::cout<<"Detecting faces and landmarks..."<<std::endl;
    faceDetector.detectMultiScale(gray, faces);
    facemark->fit(gray,faces,landmarks); //gray?

    for (unsigned long i=0;i<faces.size();i++){
        std::cout<<"Resizing face rectangle number "<< i <<" to image size (4x)..."<<std::endl;
        faces[i].x = faces[i].x*4;
        faces[i].y = faces[i].y*4;
        faces[i].height = faces[i].height*4;
        faces[i].width = faces[i].width*4;

        cv::Rect temp_myRoi;

        std::cout << "Cutting face ["<<i<<"]: "<< faces[i] << " from original image... with padding width of " << (faces[i].width/2) << " and with padding height of " << (faces[i].height/3) << std::endl;
        std::cout << "Note: If padding exceeds limits of image, limits applied." << std::endl;
        temp_myRoi.x = ((faces[i].x-(faces[i].width/2))>0)?(faces[i].x-(faces[i].width/2)):0;
        temp_myRoi.y = ((faces[i].y-(faces[i].height/3))>0)?(faces[i].y-(faces[i].height/3)):0;
        temp_myRoi.width = ((faces[i].x+faces[i].width+(faces[i].width/2))<img.cols)?(2*(faces[i].width)):(img.cols-temp_myRoi.x);
        temp_myRoi.height = ((faces[i].y+faces[i].height+(faces[i].height/3))<img.rows)?(faces[i].height+2*(faces[i].height/3)):(img.rows-temp_myRoi.y);
        std::cout << "Rectangle applied to cut face: " << temp_myRoi << std::endl;
        faces_imgs.push_back (img(temp_myRoi));

        std::cout<<"Resizing landmark points to image size (4x)..."<<std::endl;
        for(unsigned long k=0;k<landmarks[i].size();k++){
            landmarks[i][k] = landmarks[i][k]*4;
            landmarks[i][k] = landmarks[i][k]-cv::Point2f((float)temp_myRoi.x,(float)temp_myRoi.y);
        }

        temp_myRoi = cv::Rect(landmarks[i][17]-cv::Point2f(30,30),landmarks[i][15]+cv::Point2f(0,30));
        std::cout << "Rectangle applied to cut eyes w/ padding: " << temp_myRoi << std::endl;
        eyes_imgs.push_back(adjust_ImageToScreen(faces_imgs[i](temp_myRoi)));

        temp_myRoi = cv::Rect(landmarks[i][20]-cv::Point2f(30,30),landmarks[i][35]+cv::Point2f(30,30));
        std::cout << "Rectangle applied to cut nose w/ padding: " << temp_myRoi << std::endl;
        nose_imgs.push_back (adjust_ImageToScreen(faces_imgs[i](temp_myRoi)));

        temp_myRoi = cv::Rect(landmarks[i][48]-cv::Point2f(40,40),landmarks[i][54]+cv::Point2f(40,40));
        std::cout << "Rectangle applied to cut mouth w/ padding: " << temp_myRoi << std::endl;
        mouth_imgs.push_back (adjust_ImageToScreen(faces_imgs[i](temp_myRoi)));

        faces_imgs[i] = adjust_ImageToScreen(faces_imgs[i]);
    }
}
std::vector<cv::Mat> ipl::Faces::get_FacesImgs()
{
    return faces_imgs;
}
std::vector<cv::Mat> ipl::Faces::get_EyesImgs()
{
    return eyes_imgs;
}
std::vector<cv::Mat> ipl::Faces::get_NoseImgs()
{
    return nose_imgs;
}
std::vector<cv::Mat> ipl::Faces::get_MouthImgs()
{
    return mouth_imgs;
}

cv::Mat ipl::add_Grid(cv::Mat img, int grid_gap_in_pixels, cv::Scalar grid_color, int offset)
{
    int width = img.size().width;
    int height = img.size().height;

    for (int i = offset; i < height; i += grid_gap_in_pixels)
        cv::line(img, cv::Point(0, i), cv::Point(width, i), grid_color);

    for (int i = offset; i < width; i += grid_gap_in_pixels)
        cv::line(img, cv::Point(i, 0), cv::Point(i, height), grid_color);

    return img;
}
cv::Mat ipl::adjust_ImageToScreen(cv::Mat img){
    ipl::Screen scr;
    int scr_height = scr.get_scr_height();
    int scr_width = scr.get_scr_width();
    float resizing_factor = 0.0;
    float resizing_factor_w = (((float)scr_width*(float)0.8) / img.cols);
    float resizing_factor_h = (((float)scr_height*(float)0.8) / img.rows);
    resizing_factor = (resizing_factor_w>resizing_factor_h)?resizing_factor_h:resizing_factor_w;
    std::cout << "Resizing image from [" << img.cols << "," << img.rows << "] using resizing_factor: " << resizing_factor << std::endl;
    cv::resize(img, img, cv::Size(), resizing_factor, resizing_factor);
    std::cout << "Resized to: [" << img.cols << "," << img.rows << "]" << std::endl;

    return img;
}
cv::Mat ipl::posterize(const cv::Mat img, double lvls)
{
    std::vector<double> possible_values;
    double current = 0.00;
    std::vector<double> distance_from_values;
    double minimum = 255.00;
    double minimum_index = 0.00;

    for (double i = 0.00; i < lvls; i++)
    {
        current = std::ceil(0.00 + ((i * 255.00) / (lvls - 1.00)));
        possible_values.push_back(current);
    }

    cv::Mat posterize_output_image = cv::Mat::zeros(img.size(), img.type());
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            for (int c = 0; c < img.channels(); c++)
            {
                for (int i = 0; i < lvls; i++)
                {
                    if (img.channels() == 1)
                        distance_from_values.push_back(std::abs((int)img.at<uchar>(y, x) - possible_values.at(i)));
                    else
                        distance_from_values.push_back(std::abs((int)img.at<cv::Vec3b>(y, x)[c] - possible_values.at(i)));
                    if (distance_from_values.at(i) < minimum)
                    {
                        minimum = distance_from_values.at(i);
                        minimum_index = i;
                    }
                }
                minimum = 255.00;
                if (img.channels() == 1)
                    posterize_output_image.at<uchar>(y, x) = cv::saturate_cast<uchar>((uchar)possible_values.at((size_t)minimum_index));
                else
                    posterize_output_image.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>((uchar)possible_values.at((size_t)minimum_index));
                distance_from_values.clear();
            }
        }
    }

    return posterize_output_image;
} // end of posterize function code block
cv::Mat ipl::simp_Face(cv::Mat img, bool fast)
{
    std::cout << "Simplifing image..."<<std::endl;
    int posterization_levels = 9;
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    img = posterize(img, posterization_levels);
    if(!fast){
        cv::fastNlMeansDenoising(img, img, 12, 7, 21);
        img = posterize(img, posterization_levels); //takes a long time, may not be needed! or put flal speed -s 1 or 2
    }

    return img;
}
std::vector<cv::Mat> ipl::get_Highlights(cv::Mat img, int gap){

    std::vector<cv::Mat> img_vector;

    cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC3);
    cv::Mat mask_inv = cv::Mat::zeros(img.size(), CV_8UC3);
    cv::Mat green_image(img.size(), CV_8UC3, cv::Scalar(0, 255, 0));
    cv::Mat result = cv::Mat::zeros(img.size(), CV_8UC3);

    /* int gap = (green_image.cols / 90); */
    green_image = add_Grid(green_image, gap, cv::Scalar(51, 51, 255), 0);
    gap = gap*5;

    for(int i = 0; i < 8; i++){
        cv::inRange(img, cv::Scalar(std::ceil(0 + (((i) * 255) / (9 - 1.00)))), cv::Scalar(std::ceil(0 + (((i+1) * 255) / (9 - 1.00)))-1), mask);
        mask_inv = cv::Scalar(255) - mask;
        green_image.copyTo(mask, mask);
        img.copyTo(mask_inv, mask_inv);
        cv::cvtColor(mask_inv, mask_inv, cv::COLOR_GRAY2BGR);
        cv::bitwise_xor(mask_inv, mask, result);
        result = add_Grid(result, gap, cv::Scalar(0, 255, 255), 0);

        img_vector.push_back(result.clone());

        result = cv::Mat::zeros(img.size(), CV_8UC3);
        mask = cv::Mat::zeros(img.size(), CV_8UC3);
        mask_inv = cv::Mat::zeros(img.size(), CV_8UC3);
    }

    return img_vector;
}

bool ipl::path_exists(const std::filesystem::path& p, std::filesystem::file_status s)
{
    std::cout << "Searching for: [" << p << "] ..." << std::endl;
    if(std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(p))
        return true;
    else
        return false;
}
void ipl::save_ImagesResult(std::string filename, cv::Mat img, std::vector<int> compression_params)
{
    bool is_saved_sucess = false;
    std::cout << "Saving -> " << filename <<std::endl;
    is_saved_sucess = cv::imwrite(filename, img, compression_params);
    if (is_saved_sucess)
        std::cout << "Saved " << filename << " successfully!" <<std::endl;
    else
        std::cout << "ERROR: Couldn't save  " << filename << "!" << std::endl;
}

void ipl::process_Image(std::string filename, cv::Mat img, cv::CascadeClassifier faceDetector, cv::Ptr<cv::face::Facemark> facemark)
{
    std::cout << "Size of image to process: ["<< img.cols << "(px), "<< img.rows << "(px)] [width(px),height(px)]." << std::endl;
    std::cout << "Number of Channels of image to process: [" << img.channels() << "]." << std::endl;
    std::cout << "Uncompressed Size in [(width(px)*height(px)*channels)/1024] Kb of image to process: [" << (img.cols*img.rows*img.channels())/1024 << "]Kb." << std::endl;
    Faces myfaces;
    myfaces.set_Features(img, faceDetector, facemark);
    
    std::vector<cv::Mat> faces = myfaces.get_FacesImgs();
    std::vector<cv::Mat> eyes = myfaces.get_EyesImgs();
    std::vector<cv::Mat> noses = myfaces.get_NoseImgs();
    std::vector<cv::Mat> mouths = myfaces.get_MouthImgs();
    
    std::vector<cv::Mat> face_Highlights;
    std::vector<cv::Mat> eye_Highlights;
    std::vector<cv::Mat> nose_Highlights;
    std::vector<cv::Mat> mouth_Highlights;

    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);
    std::vector<std::string> graphite_degrees_of_hardness{"8B", "6B", "4B", "2B", "HB", "2H", "4H", "6H"};
    std::string filename_noextention = filename.substr(0, filename.find_last_of("."));
    std::string filename_extension = filename.substr(filename.find_last_of("."));
    std::string final_filename;

    for (unsigned long i=0;i<faces.size();i++){

        final_filename = filename_noextention+"/person_"+std::to_string(i)+"/face/"+"Original_Face"+filename_extension;
        if(!path_exists(filename_noextention+"/person_"+std::to_string(i)+"/face/"))
            std::filesystem::create_directories(filename_noextention+"/person_"+std::to_string(i)+"/face/");
        save_ImagesResult(final_filename, faces[i], compression_params);

        final_filename = filename_noextention+"/person_"+std::to_string(i)+"/eyes/"+"Original_Eyes"+filename_extension;
        if(!path_exists(filename_noextention+"/person_"+std::to_string(i)+"/eyes/"))
            std::filesystem::create_directory(filename_noextention+"/person_"+std::to_string(i)+"/eyes/");
        save_ImagesResult(final_filename, eyes[i], compression_params);

        final_filename = filename_noextention+"/person_"+std::to_string(i)+"/nose/"+"Original_Nose"+filename_extension;
        if(!path_exists(filename_noextention+"/person_"+std::to_string(i)+"/nose/"))
            std::filesystem::create_directory(filename_noextention+"/person_"+std::to_string(i)+"/nose/");
        save_ImagesResult(final_filename, noses[i], compression_params);

        final_filename = filename_noextention+"/person_"+std::to_string(i)+"/mouth/"+"Original_Mouth"+filename_extension;
        if(!path_exists(filename_noextention+"/person_"+std::to_string(i)+"/mouth/"))
            std::filesystem::create_directory(filename_noextention+"/person_"+std::to_string(i)+"/mouth/");
        save_ImagesResult(final_filename, mouths[i], compression_params);

        faces[i] = simp_Face(faces[i], false);
        eyes[i] = simp_Face(eyes[i], true);
        noses[i] = simp_Face(noses[i], true);
        mouths[i] = simp_Face(mouths[i], true);

        int gap = (faces[i].cols / 90);
        face_Highlights = get_Highlights(faces[i], gap);
        eye_Highlights = get_Highlights(eyes[i], gap);
        nose_Highlights = get_Highlights(noses[i], gap);
        mouth_Highlights = get_Highlights(mouths[i], gap);

        if(!path_exists("./drawaid_folder/"))//If output does not exist, create
            std::filesystem::create_directory("drawaid_folder");
        for (unsigned long j=0;j<8;j++){

            final_filename = filename_noextention+"/person_"+std::to_string(i)+"/face/"+"face_"+std::to_string(j)+"_"+graphite_degrees_of_hardness[j]+filename_extension;
            if(!path_exists(filename_noextention+"/person_"+std::to_string(i)+"/face/"))
                std::filesystem::create_directories(filename_noextention+"/person_"+std::to_string(i)+"/face/");
            save_ImagesResult(final_filename, face_Highlights[j], compression_params);

            final_filename = filename_noextention+"/person_"+std::to_string(i)+"/eyes/"+"eyes_"+std::to_string(j)+"_"+graphite_degrees_of_hardness[j]+filename_extension;
            if(!path_exists(filename_noextention+"/person_"+std::to_string(i)+"/eyes/"))
                std::filesystem::create_directory(filename_noextention+"/person_"+std::to_string(i)+"/eyes/");
            save_ImagesResult(final_filename, eye_Highlights[j], compression_params);
            
            final_filename = filename_noextention+"/person_"+std::to_string(i)+"/nose/"+"nose_"+std::to_string(j)+"_"+graphite_degrees_of_hardness[j]+filename_extension;
            if(!path_exists(filename_noextention+"/person_"+std::to_string(i)+"/nose/"))
                std::filesystem::create_directory(filename_noextention+"/person_"+std::to_string(i)+"/nose/");
            save_ImagesResult(final_filename, nose_Highlights[j], compression_params);

            final_filename = filename_noextention+"/person_"+std::to_string(i)+"/mouth/"+"mouth_"+std::to_string(j)+"_"+graphite_degrees_of_hardness[j]+filename_extension;
            if(!path_exists(filename_noextention+"/person_"+std::to_string(i)+"/mouth/"))
                std::filesystem::create_directory(filename_noextention+"/person_"+std::to_string(i)+"/mouth/");
            save_ImagesResult(final_filename, mouth_Highlights[j], compression_params);
        }
    }
}
