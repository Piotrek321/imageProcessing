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

void gaussForRGB(Mat &imageToFilter, const Mat & original)
{
 Mat tempImage = original.clone();
 int y1, x1;

 for(int y=0;y<original.rows;y++)
    {
        for(int x=0;x<original.cols;x++)
        { vector<double> sum(original.channels(), 0.0);
            for (int i =-2; i<=2; ++i)         
            {
              y1 = reflect(original.rows, y - i);
              sum[0] += coeffs[i+2] * original.at<Vec3b>(Point(x, y1))[0] ;
              sum[1] += coeffs[i+2] * original.at<Vec3b>(Point(x, y1))[1] ;
              sum[2] += coeffs[i+2] * original.at<Vec3b>(Point(x, y1))[2] ;          
            }
          Vec3b color;   
            color[0] = sum[0];
            color[1] = sum[1];
            color[2] = sum[2];
           tempImage.at<Vec3b>(Point(x,y)) = color;
        }
	}
		    	    //imshow("tempImage", tempImage);
for(int y=0;y<original.rows;y++)
    {
        for(int x=0;x<original.cols;x++)
        { vector<double> sum(original.channels(), 0.0);
            for (int i =-2; i<=2; ++i)         
            {
              x1 = reflect(original.rows, x - i);
              sum[0] += coeffs[i+2] * tempImage.at<Vec3b>(Point(x1, y))[0] ;
              sum[1] += coeffs[i+2] * tempImage.at<Vec3b>(Point(x1, y))[1] ;
              sum[2] += coeffs[i+2] * tempImage.at<Vec3b>(Point(x1, y))[2] ;          
            }
          Vec3b color;
            color[0] = sum[0];
            color[1] = sum[1];
            color[2] = sum[2];
           imageToFilter.at<Vec3b>(Point(x,y)) = color;
        }
	}
}

void gaussForGrayscale(Mat &imageToFilter, const Mat & original)
{
 Mat tempImage = original.clone();
 int y1, x1;

 for(int y=0;y<original.rows;y++)
    {
        for(int x=0;x<original.cols;x++)
        {
           double sum =0.0;
            for (int i =-2; i<=2; ++i)         
            {
              y1 = reflect(original.rows, y - i);
              sum += coeffs[i+2] * original.at<uchar>(Point(x, y1)) ;
            }

           tempImage.at<uchar>(Point(x,y)) = sum;
        }
	}

for(int y=0;y<original.rows;y++)
    {
        for(int x=0;x<original.cols;x++)
        {
            double sum =0.0;
            for (int i =-2; i<=2; ++i)         
            {
              x1 = reflect(original.rows, x - i);
              sum += coeffs[i+2] * tempImage.at<uchar>(Point(x1, y)) ;    
            }
        
           imageToFilter.at<uchar>(Point(x,y)) = sum;
        }
	}

}

void runGaussianBlurFilter(Mat &imageToFilter, const Mat & original)
{
 if(original.channels() ==3)
 {
   gaussForRGB(imageToFilter,original);
	}else
	{
	  gaussForGrayscale(imageToFilter, original);
	}
}


void SobelFilterGrayScale(Mat &imageToFilter, Mat &original)
{
	int pixelx,pixely;

	for(int y=1;y<original.rows-1;y++)
    {
        for(int x=1;x<original.cols-1;x++)
        {
              pixelx = (Sobelx[0][0] * original.at<uchar>(Point(x-1, y-1))) + 
                       (Sobelx[1][0] * original.at<uchar>(Point(x-1, y)))   +
                       (Sobelx[2][0] * original.at<uchar>(Point(x-1, y+1))) +
                       (Sobelx[0][2] * original.at<uchar>(Point(x+1, y-1))) +
                       (Sobelx[1][2] * original.at<uchar>(Point(x+1, y)))   +
                       (Sobelx[2][2] * original.at<uchar>(Point(x+1, y+1)));
                                           
              pixely = (Sobely[0][0] * original.at<uchar>(Point(x-1, y-1))) + 
                       (Sobely[1][0] * original.at<uchar>(Point(x-1, y)))   +
                       (Sobely[2][0] * original.at<uchar>(Point(x-1, y+1))) +
                       (Sobely[0][2] * original.at<uchar>(Point(x+1, y-1))) +
                       (Sobely[1][2] * original.at<uchar>(Point(x+1, y)))   +
                       (Sobely[2][2] * original.at<uchar>(Point(x+1, y+1)));

               auto value = ceil(sqrt((pixelx * pixelx) + (pixely*pixely)));
               if(value > 255) value =255;
               else if(value < 0) value =0;
            	imageToFilter.at<uchar>(Point(x,y)) = value;
         }     

    }          
}


void SobelFilterRGB(Mat &imageToFilter, Mat &original)
{
int pixelx,pixely;

for(int y=1;y<original.rows-1;y++)
    {
        for(int x=1;x<original.cols-1;x++)
        {
           Vec3b value;
           for(int channels = 0; channels< original.channels(); channels++)
           {
              pixelx = (Sobelx[0][0] * original.at<Vec3b>(Point(x-1, y-1))[channels]) + 
                       (Sobelx[1][0] * original.at<Vec3b>(Point(x-1, y))[channels])   +
                       (Sobelx[2][0] * original.at<Vec3b>(Point(x-1, y+1))[channels]) +
                       (Sobelx[0][2] * original.at<Vec3b>(Point(x+1, y-1))[channels]) +
                       (Sobelx[1][2] * original.at<Vec3b>(Point(x+1, y))[channels])   +
                       (Sobelx[2][2] * original.at<Vec3b>(Point(x+1, y+1))[channels]);
                                           
              pixely = (Sobely[0][0] * original.at<Vec3b>(Point(x-1, y-1))[channels]) + 
                       (Sobely[1][0] * original.at<Vec3b>(Point(x-1, y))[channels])   +
                       (Sobely[2][0] * original.at<Vec3b>(Point(x-1, y+1))[channels]) +
                       (Sobely[0][2] * original.at<Vec3b>(Point(x+1, y-1))[channels]) +
                       (Sobely[1][2] * original.at<Vec3b>(Point(x+1, y))[channels])   +
                       (Sobely[2][2] * original.at<Vec3b>(Point(x+1, y+1))[channels]);

               auto val = ceil(sqrt((pixelx * pixelx) + (pixely*pixely)));
               if(val > 255) val =255;
               else if(val < 0) val =0;
               value[channels] = val;
           }     
            imageToFilter.at<Vec3b>(Point(x,y)) = value;
        }          
    }
}

void SobelFilter(Mat &imageToFilter, Mat &original)
{
if(original.channels() ==3)
 {
   SobelFilterRGB(imageToFilter,original);
	}else
	{
	  SobelFilterGrayScale(imageToFilter, original);
	}
}

void thresholdFilteringGrayscale(Mat &imageToFilter, int threshold)
{
	for(int y=1;y<imageToFilter.rows-1;y++)
    {
        for(int x=1;x<imageToFilter.cols-1;x++)
        {
			 if(imageToFilter.at<uchar>(Point(x, y)) < threshold) imageToFilter.at<uchar>(Point(x, y)) = 0; 
		  }
	 }
}

void thresholdFilteringRGB(Mat &imageToFilter, int threshold)
{

}

void thresholdFiltering(Mat &imageToFilter, int threshold)
{
if(imageToFilter.channels() ==3)
 {
   thresholdFilteringRGB(imageToFilter, threshold);
	}else
	{
	  thresholdFilteringGrayscale(imageToFilter, threshold);
	}
}

int main(int argc, char** argv )
{
Mat original= imread( argv[1], IMREAD_GRAYSCALE ); //IMREAD_GRAYSCALE
Mat imgToProcess = original.clone();
Mat endImage = original.clone();
cv::Mat invSrc =  cv::Scalar::all(255) - original;
cout <<"original.type(): " <<type2str(original) <<"\n"; 
int threshold = 50;
/*
Mat tempImage = original.clone();
 int y1, x1;
 
 for(int y=0;y<original.rows;y++)
    {
        for(int x=0;x<original.cols;x++)
        {vector<double> sum(original.channels(), 0.0);
            for (int i =-2; i<=2; ++i)         
            {
              y1 = reflect(original.rows, y - i);
              sum[0] += coeffs[i+2] * original.at<Vec3b>(Point(y1, x))[0] ;
              sum[1] += coeffs[i+2] * original.at<Vec3b>(Point(y1, x))[1] ;
              sum[2] += coeffs[i+2] * original.at<Vec3b>(Point(y1, x))[2] ;          
            }
          Vec3b color;   
            color[0] = sum[0];
            color[1] = sum[1];
            color[2] = sum[2];
           tempImage.at<Vec3b>(Point(y,x)) = color;
        }
	}*/
		    	   // imshow("tempImage", tempImage);
runGaussianBlurFilter(imgToProcess, original);

SobelFilter(imgToProcess, original);

imshow("original", original);
imshow("imgToProcess", imgToProcess);
thresholdFiltering(imgToProcess, threshold);
imshow("imgToProcess2", imgToProcess);
 /*Mat difference1 = original - imgToProcess;
    imshow("difference1", difference1);
    
     Mat difference2 = original - endImage;
    imshow("difference2", difference2);
        imshow("endImage", endImage);*/
        
      
	
        //    imshow("endImage!!!!", temp);
//imwrite("new.jpg", temp);
    waitKey(0);
    
    
 
    return 0;
}
