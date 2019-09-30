#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;

//![variables]
Mat src, src1, src_gray, sobelx, draw, gauss_kernel, gauss_gray;
Mat dst, dst2, detected_edges, detected_edges2, src_prewitt, src_prewitt_gray ,KernelX ,KernelY;


int lowThreshold = 0;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char* window_name = "Edge Map Canny";
const char* window_name2 = "Edge Map Soble";
const char* window_name3 = "Edge Map Prewitt";
const char* window_name4 = "Edge detection on binary mask";
const char* window_name5 = "Original video";


static void CannyThreshold(int, void*)
{
    /// Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, Size(3,3) );
    //![reduce_noise]

    /// Canny detector
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    //![canny]

    /// Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);

    src.copyTo( dst, detected_edges);

}

static void CannyThreshold2(int, void*)
{
    /// Reduce noise with a kernel 3x3
    blur( gauss_gray, detected_edges2, Size(3,3) );
    //![reduce_noise]

    /// Canny detector
    Canny( detected_edges2, detected_edges2, lowThreshold, lowThreshold*ratio, kernel_size );
    //![canny]

    /// Using Canny's output as a mask, we display our result
    dst2 = Scalar::all(0);

    gauss_kernel.copyTo( dst2, detected_edges2);

}








int main( int argc, char** argv )
{
  //VideoCapture video("C:/Users/hashan/Desktop/test.mov");
 VideoCapture video(0); //Webcam

 if (!video.isOpened())
 {
  std::cout << "Cannot open the video cam" << std::endl;
  return -1;
 }


  while(1){
  bool bSuccess = video.read(src);

  if (!bSuccess)
  {
   std::cout << "Cannot read a frame from video stream" << std::endl;
   break;
  }


  //![create_mat]
  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );
  //![convert_to_gray]
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  //![create_window]
  namedWindow( window_name, WINDOW_AUTOSIZE );
  //![create_trackbar]
  /// Create a Trackbar for user to enter threshold

  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  /// Show the image
  CannyThreshold(0, 0);
  imshow( window_name, dst);


 
    cvtColor(src, src1, COLOR_BGR2GRAY);
    Sobel(src1, sobelx, CV_32F, 1, 0);
 
    double minVal, maxVal;
    minMaxLoc(sobelx, &minVal, &maxVal); //find minimum and maximum intensities
    sobelx.convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
 
    namedWindow(window_name2, WINDOW_AUTOSIZE);
    //Display image
    imshow(window_name2, draw);


    cvtColor( src, src_prewitt_gray, COLOR_RGB2GRAY );
    KernelX = (Mat_<double>(3,3) <<1, 1, 1, 0, 0, 0, -1, -1, -1);  
    KernelY = (Mat_<double>(3,3) <<-1, 0, 1, -1, 0, 1, -1, 0, 1); 
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y, grad;
  
    filter2D(src_prewitt_gray, grad_x, CV_16S , KernelX, Point(-1,-1));  
    filter2D(src_prewitt_gray, grad_y, CV_16S , KernelY, Point(-1,-1));
    convertScaleAbs( grad_x, abs_grad_x );
    convertScaleAbs( grad_y, abs_grad_y );
    
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad ); 
    namedWindow(window_name3, WINDOW_AUTOSIZE);

    imshow(window_name3, grad);




     //Mat image_blurred_with_9x9_kernel;
    GaussianBlur(src, gauss_kernel, Size(9, 9), 0);

    dst2.create( gauss_kernel.size(), gauss_kernel.type() );

    namedWindow(window_name4, WINDOW_AUTOSIZE);

    cvtColor( gauss_kernel, gauss_gray, COLOR_BGR2GRAY );
    createTrackbar( "Min Threshold:", window_name4, &lowThreshold, max_lowThreshold, CannyThreshold2 );

    CannyThreshold2(0, 0);


    namedWindow(window_name5, WINDOW_AUTOSIZE);
        imshow(window_name5, src);


    imshow(window_name4, gauss_gray);







  if (waitKey(30) == 27)
  {
   std::cout << "esc key pressed " << std::endl;
   break;
  }


}
  return 0;
}