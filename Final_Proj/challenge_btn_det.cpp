#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>


using namespace std;

std::vector<cv::Mat> gen_down_sample(cv::Mat I, int levels=6)
{
    std::vector<cv::Mat> pI;
    cv::Mat G = I.clone();
    pI.push_back(G);

    for (int i = 0; i < levels; i++)
    {
        cv::pyrDown(G, G);
        pI.push_back(G);
    }

    return pI;
}


std::vector<cv::Mat> half_resolution_image(cv::Mat I, int levels=6)
{

    std::vector<cv::Mat> pI;
    cv::Mat G = I.clone();
    pI.push_back(G);
    for (int i = 0; i < levels; i++)
    {
        cv::resize(G, G, cv::Size(I.cols/2, I.rows/2));
        pI.push_back(G);
    }
    
    

    return pI;
}


int main()
{
    // Read input image
    cv::Mat I = cv::imread("test_img/03-t-0-0.jpg");
    // cout << I.row(0) << endl;

    // Generate down-sampled images
    // std::vector<cv::Mat> pI = gen_down_sample(I);
    std::vector<cv::Mat> pI = half_resolution_image(I);

    // Save down-sampled images to disk
    for (int i = 0; i < pI.size(); i++)
    {
        std::string filename = "down_sampled_" + std::to_string(i) + ".jpg";
        cv::imwrite(filename, pI[i]);
    }

    return 0;
}
