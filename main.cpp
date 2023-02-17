#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include "CLI/CLI11.hpp"
#include "ipl.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <optional>

#if !_HAS_CXX17
#pragma message("The contents of <filesystem> are available only with C++17 or later.")
#pragma message("The contents of <optional> are available only with C++17 or later.")
#else  // _HAS_CXX17
#pragma message("compiler _HAS_CXX17!")
#endif

int main(int argc, char **argv)
{
    CLI::App app{"\n\nDRAWAID\n\nRetrieves every .jpg from either the drawaid folder in this current directory \nor the path provided and, if it has a human face,\ncreates one folder for each photo with several images to help you draw an acurate portrait of a person.\n\nIf no path or filename is given, the program will either use or create a folder named \"drawaid_folder\"\nfor your convinience and use as the working directory of the program.\nIf no model is given, the program will try to use lbfmodel.yaml.txt that should be on the same directory as the drawaid executable.\nIf no cascade xml file is given, the program will try to use haarcascade_frontalface_alt2.xml that should be on the same directory as the drawaid executable.\n\n"};
    std::string p = "";
    std::string f = "";
    std::string c = "";
    std::string m = "";
    app.add_option("-p", p, "Starting directory.");
    app.add_option("-f", f, "Absolute path to filename.");
    app.add_option("-c", c, "Absolute path to the face cascade xml file to be used as a face detector. ex: haarcascade_frontalface_alt2.xml");
    app.add_option("-m", m, "Absolute path yaml text file storing the trained model to be loaded. ex: lbfmodel.yaml.txt");
    CLI11_PARSE(app, argc, argv);
    std::cout << "drawaid program called! starting execution..." << std::endl;
    std::cout << "path value: " << p << std::endl;
    std::cout << "filename value: " << f << std::endl;
    std::cout << "cascade value: " << c << std::endl;
    std::cout << "model value: " << m << std::endl;

    if(c == "" || m =="")//If no cascade or model is given
        if(!std::filesystem::is_regular_file("./haarcascade_frontalface_alt2.xml") || !std::filesystem::is_regular_file("./lbfmodel.yaml.txt")){
            std::cerr << "Both cascade and model needed to execute this program successfully! Please try again or try -help option for more information.";
            return EXIT_FAILURE;
        }else{
            std::cout << "No model or cascade provided, trying default present in current directory." << std::endl;
            c = "./haarcascade_frontalface_alt2.xml";
            m = "./lbfmodel.yaml.txt";
        }

    if(!std::filesystem::is_regular_file(m) || !std::filesystem::is_regular_file(c)){
        std::cerr << "Both cascade and model need to be valid to execute this program successfully! Please try again or try -help option for more information.";
        return EXIT_FAILURE;
    }else{
        cv::CascadeClassifier faceDetector;
        cv::Ptr<cv::face::Facemark> facemark = cv::face::FacemarkLBF::create();
        faceDetector.load(c);
        std::cout<<"Loaded face cascade successfully!"<<std::endl;
        facemark->loadModel(m);
    std::cout<<"Loaded model successfully!"<<std::endl;
        if(p == "" && f ==""){//If no path or filename is given
            std::cout << "No path or filename was provided!" << std::endl;
            if(!ipl::path_exists("./drawaid_folder/")){//If drawaid_folder does not exist, create
                std::cout << "drawaid_folder does not exist, creating folder..." << std::endl;
                std::filesystem::create_directory("drawaid_folder");
                std::cout << "A folder named \"drawaid_folder\" was created successfully in this directory!"<< std::endl <<"Please transfer the files to be processed into it and run the program again." << std::endl;
                std::cout << "Listing drawaid_folder contents..." << std::endl;
                std::system("ls -l drawaid_folder");
            }else{//If drawaid_folder exists, use it
                std::cout << "drawaid_folder exists, using folder..." << std::endl;
                std::cout << "Listing drawaid_folder contents..." << std::endl;
                std::system("ls -l drawaid_folder");
                std::cout << "Beggining Process..." << std::endl;
                for (auto const& dir_entry : std::filesystem::directory_iterator{"./drawaid_folder/"}){//Iterate through directory
                    std::cout << "Processing : ["  << dir_entry.path() << "] ..." << std::endl;
                    cv::Mat current_original_img = cv::imread(dir_entry.path().string(), cv::IMREAD_COLOR);
                    if (!current_original_img.empty())
                    {
                        std::cout << "Image ["<< dir_entry.path() <<"] opened successfully, processing..." << std::endl;
                        ipl::process_Image(dir_entry.path().string(),current_original_img, faceDetector, facemark);
                    }else{
                        std::cerr << "Can't open image [" << dir_entry.path() << "]. Ignoring file..." << std::endl;
                    }
                }
            }
        }else{//If path or filename is given
            if(p != "" && f !=""){//If both are provided, only one of the two can be used.
                std::cerr << "Two options provided, only one of the two can be used! Please try again or try -help option for more information.";
                return EXIT_FAILURE;
            }else{
                if(p != "" && f ==""){//If path is provided
                    if(ipl::path_exists(p) && std::filesystem::is_directory(p)){//If path is found
                        std::cout << "Path found successfully!" << std::endl;
                        std::cout << "Listing [" << p << "] contents..." << std::endl;
                        std::system(("ls -l " + p).c_str());
                        std::cout << "Beggining Process..." << std::endl;
                        for (auto const& dir_entry : std::filesystem::directory_iterator{p}){//Iterate through directory
                            std::cout << "Processing : ["  << dir_entry.path() << "] ..." << std::endl;
                            cv::Mat current_original_img = cv::imread(dir_entry.path().string(), cv::IMREAD_COLOR);
                            if (!current_original_img.empty())
                            {
                                std::cout << "Image ["<< dir_entry.path() <<"] opened successfully, processing..." << std::endl;
                                ipl::process_Image(dir_entry.path().string(), current_original_img, faceDetector, facemark);
                            }else{
                                std::cerr << "Can't open image [" << dir_entry.path() << "]. Ignoring file..." << std::endl;
                            }
                        }
                    }else{
                        std::cerr << "Cannot find [" << p << "] path! Please try again or try -help option for more information.";
                        return EXIT_FAILURE;
                    }
                }else if(p == "" && f !=""){//If filename is provided
                    if(ipl::path_exists(f) && std::filesystem::is_regular_file(f)){//If filename is found
                        std::cout << "Filename  [" << f << "]  found successfully!" << std::endl;
                        std::cout << "Beggining Process..." << std::endl;
                        std::cout << "Processing : ["  << f << "] ..." << std::endl;
                        cv::Mat current_original_img = cv::imread(f, cv::IMREAD_COLOR);
                        if (!current_original_img.empty())
                        {
                            std::cout << "Image ["<< f <<"] opened successfully, processing..." << std::endl;
                            ipl::process_Image(f,current_original_img, faceDetector, facemark);
                        }else{
                            std::cerr << "Can't open image [" << f << "]. Ignoring file..." << std::endl;
                        }
                    }else{
                        std::cerr << "Cannot find [" << f << "] filename! Please try again or try -help option for more information.";
                        return EXIT_FAILURE;
                    }
                }else{//Unexpected error.
                        std::cerr << "Unexpected Error! Please try again or try -help option for more information.";
                        return EXIT_FAILURE;
                }
            }
        }
    }

    std::cout << "\nThanks for using drawaid!\n" << std::endl;
    return EXIT_SUCCESS;
} // end of main function code block
