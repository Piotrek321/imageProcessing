#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char** argv )
{

Mat original= imread("IMAGE.jpg", 1 );
Mat toCompare = imread( argv[1], 1 );
Mat diff = original -toCompare;

imshow("differece", diff);
};
