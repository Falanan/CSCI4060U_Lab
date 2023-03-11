#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // Read the image
    Mat img = imread("test_img/03-t-0-0.jpg");

    // Check if the image was successfully read
    if (img.empty())
    {
        cout << "Could not read the image" << endl;
        return 1;
    }

    // Display the image
    imshow("Image", img);
    waitKey(0);

    return 0;
}
