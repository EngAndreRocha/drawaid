#include <iostream>
#include <vector>
#include <cmath>
#include "hello.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"

#define MIN_VALUE 0.00
#define MAX_VALUE 255.00

//"C:\Users\arlin\Documents\DOCUMENTOS_ANDRE_CORREIA_ROCHA_LAPTOP_7_12_2021\projects\graphite-drawing-helper\input-images\test.jpg"

cv::Mat posterize(const cv::Mat bgrmat, double lvls)
{
    /* std::cout << "bgrmat = " << std::endl << cv::format(bgrmat, cv::Formatter::FMT_PYTHON) << std::endl << std::endl; */
    std::vector<double> possible_values;
    std::vector<double> distance_from_values;
    double minimum = MAX_VALUE;
    double minimum_index = 0.00;


    for(double i = 0.00; i < lvls; i++)
        possible_values.push_back (
            std::ceil(
                MIN_VALUE+((i*MAX_VALUE)/(lvls-1.00))
                )
    );

    cv::Mat result = cv::Mat::zeros( bgrmat.size(), bgrmat.type() );
    for( int y = 0; y < bgrmat.rows; y++ ) {
        for( int x = 0; x < bgrmat.cols; x++ ) {
            for( int c = 0; c < bgrmat.channels(); c++ ) {
                for(double i = 0.00; i < lvls; i++){
                    /* std::cout << (int)bgrmat.at<cv::Vec3b>(y,x)[c] << "-" << possible_values.at(i) << "= " << std::abs((int)bgrmat.at<cv::Vec3b>(y,x)[c] - possible_values.at(i)) << std::endl; */

                    distance_from_values.push_back (std::abs((int)bgrmat.at<cv::Vec3b>(y,x)[c] - possible_values.at(i)));

                    /* std::cout << "distance_from_values.at("<<i<<"):" << distance_from_values.at(i) << " minimum:" << minimum << std::endl; */
                    if(distance_from_values.at(i) < minimum){
                        minimum = distance_from_values.at(i);
                        minimum_index = i;
                        /* std::cout << "minimum:" << minimum << " minimum_index:" << minimum_index << std::endl; */
                    }
                }
                minimum = MAX_VALUE;
                /* std::cout << "minimum_index => " << minimum_index<< std::endl << "possible_values.at(minimum_index) => " << possible_values.at(minimum_index) << std::endl; */
                result.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( (uchar)possible_values.at(minimum_index) );
                /* result.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( 50 ); */
                distance_from_values.clear();
            }
        }
    }

    /* for(int g = 0; g < possible_values.size() ; g++){
        std::cout << possible_values.at(g);
    } */
    /* std::cout << "result = " << std::endl << cv::format(result, cv::Formatter::FMT_PYTHON) << std::endl << std::endl; */

    return result;
}

int main(int argc, char** argv)
{
    hello::say_hello();
    std::string filename = argc >=2 ? argv[1] : "patricia (3).jpg";

    cv::Mat img;
    /* img = cv::imread( "./input-images/"+filename, cv::IMREAD_COLOR); */
    img = cv::imread( "C:/Users/arlin/Documents/DOCUMENTOS_ANDRE_CORREIA_ROCHA_LAPTOP_7_12_2021/projects/graphite-drawing-helper/input-images/patricia (3).jpg", cv::IMREAD_UNCHANGED);

    if (img.empty())
    {
        std::cerr << "Can't open image ["  << filename << "]" << std::endl;
        return EXIT_FAILURE;
    }

    cv::Mat new_image = cv::Mat::zeros( img.size(), img.type() );
    cv::namedWindow("Input", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("New Image", cv::WINDOW_AUTOSIZE);

    cv::resize(img, img, cv::Size(), 0.5, 0.5,cv::INTER_AREA);
    cv::resize(new_image, new_image, cv::Size(), 0.5, 0.5,cv::INTER_AREA);
    cv::imshow( "Input", img );

    new_image = posterize(img, 8);

    cv::imshow("New Image", new_image);
    cv::waitKey();
    cv::destroyAllWindows();
    return EXIT_SUCCESS;
}    //end of main function code block
