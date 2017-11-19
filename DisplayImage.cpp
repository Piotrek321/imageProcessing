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
Mat img= imread( argv[1], 1 );
Mat imgToProcess = img.clone();

cv::Mat invSrc =  cv::Scalar::all(255) - img;
//namedWindow("image", CV_WINDOW_AUTOSIZE);
cout <<"img.type(): " <<type2str(img) <<"\n"; 
//imshow("image", invSrc);
//waitKey();

 for(int y=2;y<img.rows;y++)
    {
        for(int x=2;x<img.cols;x++)
        {
        //cout <<"TU\n";
          Vec3b color = img.at<Vec3b>(Point(x,y));
          vector<vector<Vec3b>> gauss ;
              for(int row =y-2; row <= y+2 ; row++)
              {
              vector<Vec3b> gaussTemp ;
                for(int k =x-2; k<=x+2; k++)
                {
                  gaussTemp.push_back(img.at<Vec3b>(Point(k,row)));             
                }
                // cout <<"gaussTemp " << gaussTemp << "\n";
                           // cout <<"gaussTemp.[0] " << gaussTemp[0] << "\n";
                            // cout <<"gaussTemp.[0] " << (int)gaussTemp[0][0] << "\n";
                gauss.push_back(gaussTemp);
              }
             //
             
             
             
          Mat mat;   
             

 //    mat.at <uchar>(0,0) = gauss[0][0][0];
  //  mat.at <uchar>(1,0) = gauss[0] [0][1];
  //  mat.at <uchar>(2,0) = gauss [0][0][2];
 
             
             
    //  imshow("image", mat);       
             
             
             
             ///
            //  cout <<"gauss.size(): " << gauss.size() << "\n";
                          //  cout <<"gauss.[0].size(): " << gauss[0].size() << "\n";
                            //cout <<"gauss.[0][0].size(): " << gauss[0][0].size() << "\n";
            for(int channels = 0;channels <3;channels++)
            {//cout <<"TU3\n";
              int sum=0;
              int i =0;
              for(int rr = 0; rr<5 ;rr++)
              {
                for(int row= 0; row <5; row++)
                {
                  for(int col= 0;col <5; col++)
                  {
                     //cout <<"i: " << i << " " << gauss[rr][row] << "\n";
                    // i++;
                     sum += vectors[rr][row] * (int)gauss[col][row][channels] ;
                  }
                }

              } 
             // cout <<"sum: " << sum << "\n";
                 color[channels] = sum/273;
                // getchar();
              //cout <<"color: " << color << "\n";
            }
           
	      imgToProcess.at<Vec3b>(Point(x,y)) = color;
        }

	}
  

/*
    for(int y=0;y<img.rows;y++)
    {
        for(int x=0;x<img.cols;x++)
        {
        Vec3b color = img.at<Vec3b>(Point(x,y));
	//cout <<"color[0]: "<< color[0] << " color[1]: " << color[1] << " color[2]: " << color[2] << "\n";
        if(color[0] > threshold && color[1] > threshold && color[2] > threshold)
        {
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;

            //cout << "Pixel >200 :" << x << "," << y << endl;
        }
        else
        {
            color[0] = 255;
            color[1] = 255;
            color[2] = 255;
        }
	img.at<Vec3b>(Point(x,y)) = color;
	}

    }*/
    std::cout <<"XXXX\n";
    imshow("image22", imgToProcess);
   // imshow("image24442", img);
    waitKey(0);

    return 0;
}

////

/*

 for(int y=0;y<img.rows;y++)
    {
        for(int x=0;x<img.cols;x++)
        {
        Vec3b color = img.at<Vec3b>(Point(x,y));
	//cout <<"color[0]: "<< color[0] << " color[1]: " << color[1] << " color[2]: " << color[2] << "\n";
        if(color[0] > 150 && color[1] > 150 && color[2] > 150)
        {
            color[0] = 255;
            color[1] = 255;
            color[2] = 255;

            //cout << "Pixel >200 :" << x << "," << y << endl;
        }
        else
        {
            color.val[0] = 0;
            color.val[1] = 0;
            color.val[2] = 0;
        }
	img.at<Vec3b>(Point(x,y)) = color;
	}

    }
    imshow("image22", img);*/
