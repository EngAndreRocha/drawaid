

# DRAWAID

Drawaid is a command-line interface (CLI) program designed to process a jpeg image 
containing human faces and processing it to form a set of images that serve as a tool 
for drawing using graphite or other drawing tools.

Retrieves every .jpg from either the drawaid folder in this current directory
or the path provided and, if it has a human face,
creates one folder for each photo with several images to help you draw an acurate portrait of a person.

If no path or filename is given, the program will either use or create a folder named "drawaid_folder"
for your convinience and use as the working directory of the program.

If no model is given, the program will try to use lbfmodel.yaml.txt that should be on the same directory as the drawaid executable.

If no cascade xml file is given, the program will try to use haarcascade_frontalface_alt2.xml that should be on the same directory as the drawaid executable.

## Requirements

    To use this program, some basic knowledge in CLI is recommended but not required. Also, your folder structure should contain:
        * The drawaid executable;
        * The ipl library;
        * A trained model which is to be loaded for example [lbfmodel.yaml.txt];
        * A face cascade xml file which you want to use as a face detector for example[haarcascade_frontalface_alt2.xml].
        * All binaries and dlls pertaining opencv and opencv-contrib.

    To work with this source code, some basic knowledge is required:

        * Basic understanding of C++ language;
        * Basic understanding of the Opencv Framework;
        * Basic knowledge in CLI is required

## Use

    To simply use this program, download the latest release onto a directory,run the installer.
    After it installs, transfer jpg images containing human faces into drawaid_folder and run drawaid.exe.

## Example

Input: <br>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/sample.jpg" width="100px"/><br>
Output: <br>
Face: <br>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/face/face_0_8B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/face/face_1_6B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/face/face_2_4B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/face/face_3_2B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/face/face_4_HB.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/face/face_5_2H.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/face/face_6_4H.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/face/face_7_6H.jpg" width="100px"/><br>
Eyes: <br>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/eyes/eyes_0_8B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/eyes/eyes_1_6B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/eyes/eyes_2_4B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/eyes/eyes_3_2B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/eyes/eyes_4_HB.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/eyes/eyes_5_2H.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/eyes/eyes_6_4H.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/eyes/eyes_7_6H.jpg" width="100px"/><br>
Mouth: <br>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/mouth/mouth_0_8B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/mouth/mouth_1_6B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/mouth/mouth_2_4B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/mouth/mouth_3_2B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/mouth/mouth_4_HB.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/mouth/mouth_5_2H.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/mouth/mouth_6_4H.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/mouth/mouth_7_6H.jpg" width="100px"/><br>
Nose: <br>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/nose/nose_0_8B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/nose/nose_1_6B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/nose/nose_2_4B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/nose/nose_3_2B.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/nose/nose_4_HB.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/nose/nose_5_2H.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/nose/nose_6_4H.jpg" width="100px"/>
    <img src="https://github.com/EngAndreRocha/drawaid/blob/main/example/person_0/nose/nose_7_6H.jpg" width="100px"/> <br>


## Installation

This program was developed in Visual Studio Code using CMake and CMake Tools extensions, 
a [Visual Studio Community 2019 Release - amd64] compiler, with C++17 capabilities and OpenCV_VERSION: 4.5.5 with opencv's extra modules of the same version.

## Citation

```
@article{rochandre2023drawaid,
  title={{DRAWAID}: Helps you draw an acurate portrait of a person.},
  author={Andre Correia Rocha},
  website={https://andrecorreiarocha.com/},
  year={2023}
}
```

## Acknowledgements

<details><summary> <b>Expand</b> </summary>

* [https://github.com/CLIUtils/CLI11](https://github.com/CLIUtils/CLI11)
* [https://opencv.org/](https://opencv.org/)
* [https://docs.opencv.org/3.4/d2/d42/tutorial_face_landmark_detection_in_an_image.html](https://docs.opencv.org/3.4/d2/d42/tutorial_face_landmark_detection_in_an_image.html)

</details>
