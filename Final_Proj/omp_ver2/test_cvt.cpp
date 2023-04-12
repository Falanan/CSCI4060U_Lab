#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;

int main()
{
    // Load the input image
    Mat image = imread("../pics/07-T.png");

    for (int col = 0; col < image.cols; col++)
    {
        cout << image.col(col).row(1) << endl;
    }
    
    

    return 0;
}