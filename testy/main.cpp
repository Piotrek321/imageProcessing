#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
 
using namespace std;
using namespace cv;
 
// reflected indexing for border processing
int reflect(int M, int x)
{
    if(x < 0)
    {
        return -x - 1;
    }
    if(x >= M)
    {
        return 2*M - x - 1;
    }
}

int main()
{
 
      Mat src, dst, temp;
      float sum, x1, y1;
 
      /// Load an image
      src = imread("Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
 
      if( !src.data )
      { return -1; }
 
 
      // coefficients of 1D gaussian kernel with sigma = 1
      double coeffs[] = {0.0545, 0.2442, 0.4026, 0.2442, 0.0545};
 
      dst = src.clone();
      temp = src.clone();
 
        // along y - direction
        for(int y = 0; y < src.rows; y++){
            for(int x = 0; x < src.cols; x++){
                sum = 0.0;
                for(int i = -2; i <= 2; i++){
                    y1 = reflect(src.rows, y - i);
                    sum = sum + coeffs[i + 2]*src.at<uchar>(y1, x);
                }
                temp.at<uchar>(y,x) = sum;
            }
        }
 
        // along x - direction
        for(int y = 0; y < src.rows; y++){
            for(int x = 0; x < src.cols; x++){
                sum = 0.0;
                for(int i = -2; i <= 2; i++){
                    x1 = reflect(src.cols, x - i);
                    sum = sum + coeffs[i + 2]*temp.at<uchar>(y, x1);
                }
                dst.at<uchar>(y,x) = sum;
            }
        }
 
 
 
        namedWindow("final");
        imshow("final", dst);
 
        namedWindow("initial");
        imshow("initial", src);
 
      waitKey();
 
 
    return 0;
}
