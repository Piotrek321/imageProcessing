#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
string type2str(Mat& mat) {
  int type = mat.type();
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');
  std::cout <<"mat.type(): " << r << " " <<mat.cols << "x" <<  mat.rows <<"\n";
  std::cout <<"mat.channels(): " <<mat.channels() << "\n";
  return r;
}

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

vector<vector<int>> Sobelx = {
{-1, 0 ,1},
{-2, 0, 2},
{-1, 0, 1}
};
vector<vector<int>> Sobely = {
{-1, -2 ,-1},
{0, 0, 0},
{1, 2, 1}
};

double coeffs[] = {0.0545, 0.2442, 0.4026, 0.2442, 0.0545};

void runGaussianBlurFilter(Mat &imageToFilter, const Mat & original)
{
Mat tempImage = original.clone();
int y1, x1;
 for(int y=0;y<original.rows;y++)
    {
        for(int x=0;x<original.cols;x++)
        {
         // Vec3b color = original.at<Vec3b>(Point(x,y));
         // vector<double> sum(original.channels(), 0.0);  
          Vec3b sum;
            for (int i =-2; i<=2; ++i)         
            {
              y1 = reflect(original.rows, y - i);
              if(original.channels() ==3)
              {
                sum[0] += coeffs[i+2] * original.at<Vec3b>(Point(y1, x))[0] ;
                sum[1] += coeffs[i+2] * original.at<Vec3b>(Point(y1, x))[1] ;
                sum[2] += coeffs[i+2] * original.at<Vec3b>(Point(y1, x))[2] ;
              }
              else
              {
                sum += coeffs[i+2] * original.at<Vec3b>(Point(y1, x)) ;      
              }            
            }
          Vec3b color = sum ;
          tempImage.at<Vec3b>(Point(y,x)) = color;
        }
	}
  imshow("Partial gauss", tempImage);
for(int y=0;y<original.rows;y++)
    {
        for(int x=0;x<original.cols;x++)
        {
          Vec3b sum;
          for(int channels = 0; channels< original.channels(); channels++)
          {
            for (int i =-2; i<=2; ++i)         
            {
              x1 = reflect(original.rows, x - i);
              if(original.channels() ==3)
              {
                sum[0] += coeffs[i+2] * tempImage.at<Vec3b>(Point(y1, x))[0] ;
                sum[1] += coeffs[i+2] * tempImage.at<Vec3b>(Point(y1, x))[1] ;
                sum[2] += coeffs[i+2] * tempImage.at<Vec3b>(Point(y1, x))[2] ;
              }
              else
              {
                sum += coeffs[i+2] * tempImage.at<Vec3b>(Point(y1, x)) ;      
              }    
            }
            
          }
          Vec3b color = sum;
           imageToFilter.at<Vec3b>(Point(y,x)) = color;
        }
	}
}


int main(int argc, char** argv )
{
vector<Vec3b> vectors = {
{1, 4, 7, 4, 1},
{4, 16, 26, 16, 4},
{7, 26, 41, 26, 7},
{4, 16, 26, 16, 4},
{1, 4, 7, 4, 1}
};

int threshold = 100;
Mat original= imread( argv[1], 1 ); //IMREAD_GRAYSCALE
Mat imgToProcess = original.clone();
Mat endImage = original.clone();


//cv::Mat invSrc =  cv::Scalar::all(255) - original;
//namedWindow("image", CV_WINDOW_AUTOSIZE);
cout <<"original.type(): " <<type2str(original) <<"\n"; 
//imshow("image3", invSrc);
//waitKey();


runGaussianBlurFilter(imgToProcess, original);


	    imshow("original", original);
 Mat difference1 = original - imgToProcess;
    imshow("difference1", difference1);
    //imwrite( "imgToProcess.jpg", imgToProcess );
    Mat original3= imread( "imgToProcess.jpg", 1 );
    Mat difference3 = imgToProcess - original3;
    
        imshow("difference3333331", difference3);
     Mat difference2 = original - endImage;
    imshow("difference2", difference2);
        imshow("endImage", endImage);
        
        
	int pixelx,pixely;
	Mat temp = endImage.clone();
for(int y=1;y<endImage.rows-1;y++)
    {
        for(int x=1;x<endImage.cols-1;x++)
        {
           Vec3b value;
           for(int channels = 0; channels< endImage.channels(); channels++)
           {
              pixelx = (Sobelx[0][0] * endImage.at<Vec3b>(Point(x-1, y-1))[channels]) + 
                       (Sobelx[1][0] * endImage.at<Vec3b>(Point(x-1, y))[channels])   +
                       (Sobelx[2][0] * endImage.at<Vec3b>(Point(x-1, y+1))[channels]) +
                       (Sobelx[0][2] * endImage.at<Vec3b>(Point(x+1, y-1))[channels]) +
                       (Sobelx[1][2] * endImage.at<Vec3b>(Point(x+1, y))[channels])   +
                       (Sobelx[2][2] * endImage.at<Vec3b>(Point(x+1, y+1))[channels]);
                                           
              pixely = (Sobely[0][0] * endImage.at<Vec3b>(Point(x-1, y-1))[channels]) + 
                       (Sobely[1][0] * endImage.at<Vec3b>(Point(x-1, y))[channels])   +
                       (Sobely[2][0] * endImage.at<Vec3b>(Point(x-1, y+1))[channels]) +
                       (Sobely[0][2] * endImage.at<Vec3b>(Point(x+1, y-1))[channels]) +
                       (Sobely[1][2] * endImage.at<Vec3b>(Point(x+1, y))[channels])   +
                       (Sobely[2][2] * endImage.at<Vec3b>(Point(x+1, y+1))[channels]);

               auto val = ceil(sqrt((pixelx * pixelx) + (pixely*pixely)));
               if(val > 255) val =255;
               else if(val < 0) val =0;
               value[channels] = val;
           }     
            temp.at<Vec3b>(Point(x,y)) = value;
        }          
    }
            imshow("endImage!!!!", temp);

    waitKey(0);
    
    
 
    return 0;
}
