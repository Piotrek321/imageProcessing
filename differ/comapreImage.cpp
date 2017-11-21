#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
bool equal(const Mat & a, const Mat & b)
{
    if ( (a.rows != b.rows) || (a.cols != b.cols) )
        return false;
    Scalar s = sum( a - b );
    return (s[0]==0) && (s[1]==0) && (s[2]==0);
}
int main(int argc, char** argv )
{

Mat original= imread("new.jpg", 1 );
Mat toCompare = imread( argv[1], 1 );
Mat diff = original - toCompare;
imwrite("ss.jpeg",diff);
 for(int y=0;y<original.rows;y++)
    {
        for(int x=0;x<original.cols;x++)
        {
            if(original.at<Vec3b>(Point(y, x)) != toCompare.at<Vec3b>(Point(y, x)))
            {
           // std::cout <<"x: " << x << " y: " << y <<"\n";
            std::cout <<"NOT EQUAL!!!\n";
            return 0 ;
            }
        }
        
        
    }

};
