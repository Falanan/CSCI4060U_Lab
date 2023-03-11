#include <opencv2/highgui.hpp>
#include <iostream>

// g++ testcv2.cpp -std=c++11 `pkg-config --cflags --libs opencv4`

int main( int argc, char** argv ) {
  
  cv::Mat image;
  image = cv::imread("test_img/03-t-0-0.jpg" ,cv::IMREAD_COLOR);
  
  if(! image.data ) {
      std::cout <<  "Image not found or unable to open" << std::endl ;
      return -1;
    }
  
  cv::namedWindow( "Techawarey:OpenCV Test Program", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Techawarey:OpenCV Test Program", image );
  
  cv::waitKey(0);
  return 0;
}