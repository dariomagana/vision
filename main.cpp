#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>     


using namespace cv;

int main(int argc, char* argv[])
{
 //VideoCapture video("C:/Users/hashan/Desktop/test.mov");
 VideoCapture video(0); //Webcam

 if (!video.isOpened())
 {
  std::cout << "Cannot open the video cam" << std::endl;
  return -1;
 }

 
 while (1)
 {
  Mat frame;
  
  bool bSuccess = video.read(frame);

  if (!bSuccess)
  {
   std::cout << "Cannot read a frame from video stream" << std::endl;
   break;
  }
  int bv = 50;                              //Sets value for brighness adjustment from -255 to 255
  double crv = 1.5;                           //Sets value for the contrast adjusment from 0 to 100
  Mat cav = frame * crv;
  Mat NewVideo  = cav + Scalar(bv, bv, bv);


  imshow("Original Video", frame);
  imshow("Adjusted video", NewVideo);


  if (waitKey(30) == 27)
  {
   std::cout << "esc key pressed " << std::endl;
   break;
  }
 }
 return 0;

}