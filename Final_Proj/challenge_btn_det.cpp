#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>


using namespace std;

struct img_n_level
{
    cv::Mat img;
    int level;
};


struct HighlightResult {
    cv::Mat img;
    cv::Point loc;
    double val;
};


HighlightResult highlight(cv::Mat R, cv::Mat T, cv::Mat I, bool use_max=true) {
    cv::Mat I_;
    I.copyTo(I_);

    int W = I.rows, H = I.cols;
    int w = T.rows, h = T.cols;
    int wr = R.rows, hg = R.cols;
            
    double min_val, max_val;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(R, &min_val, &max_val, &min_loc, &max_loc);
    cv::Point loc = use_max ? max_loc : min_loc;
    double val = use_max ? max_val : min_val;

    cv::Point loc1 = loc + cv::Point(h/2, w/2); // Size of R is different from I
    cv::Point tl = loc1 - cv::Point(h/2, w/2);
    cv::Point br = loc1 + cv::Point(h/2, w/2);

    cv::Scalar color = I_.depth() == CV_32F ? cv::Scalar(1.0, 0, 0) : cv::Scalar(255, 0, 0);
    cv::rectangle(I_, tl, br, color, 4);

    HighlightResult result = {I_, loc, val};
    return result;
}







/** This function is used to generate gaussian pyramid
 *  Return a vector contains gaussian pyramid down images
 * */ 
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

/**
 * This function is used to generate half resulation images
 * Returns a vector contains different resulation images
 * */
std::vector<cv::Mat> half_resolution_image(cv::Mat I, int levels = 6)
{
    std::vector<cv::Mat> pI;
    cv::Mat G = I.clone();
    pI.push_back(G);
    for (int i = 0; i < levels; i++)
    {
        cv::Mat H;
        cv::resize(G, H, cv::Size(G.cols / 2, G.rows / 2));
        pI.push_back(H);
        G = H.clone();
    }
    return pI;
}



// cv::Mat make_square(cv::Mat I) {
struct img_n_level make_square(cv::Mat I) {
    // This function is used to construct a rectangular images to be used in generate gaussian pyramid
    // The return value are the square shaped image and the max level can be used to generate gaussian pyramid
    int h = I.rows;
    int w = I.cols;

    struct img_n_level imgNLevel;

    int n_levels = cvCeil(log(std::max(h, w))/log(2));
    cout << n_levels << endl;
    int new_h = pow(2, n_levels);
    int new_w = new_h;

    cv::Mat tmp;
    if (I.channels() == 3) {
        tmp = cv::Mat::zeros(new_h, new_w, I.type());
        I.copyTo(tmp(cv::Rect(0, 0, w, h)));
    } else {
        tmp = cv::Mat::zeros(new_h, new_w, I.type());
        I.copyTo(tmp(cv::Rect(0, 0, w, h)));
    }

    imgNLevel.level = n_levels;
    imgNLevel.img = tmp;

    return imgNLevel;
}



void find_sign(cv::Mat orig_img, cv::Mat orig_template){
    cv::Mat orig_img_copy = orig_img.clone();
    

}







int main()
{
    // Read input image
    cv::Mat I = cv::imread("test_img/03-t-0-0.jpg");
    // cout << I.row(0) << endl;

    // Generate down-sampled images
    // std::vector<cv::Mat> pI = gen_down_sample(I);
    // std::vector<cv::Mat> pI = half_resolution_image(I);

    // // Save down-sampled images to disk
    // for (int i = 0; i < pI.size(); i++)
    // {
    //     std::string filename = "down_sampled_" + std::to_string(i) + ".jpg";
    //     cv::imwrite(filename, pI[i]);
    // }


    cv::Mat img = cv::imread("test_img/03-t-0-0.jpg");
    if (img.empty()) {
        cerr << "Error: could not read image file." << endl;
        return -1;
    }

    // make the image square
    // cv::Mat square_img = make_square(img);
    struct img_n_level imgNLevel = make_square(img);

    // display the original and square images
    namedWindow("Original Image", cv::WINDOW_NORMAL);
    imshow("Original Image", img);
    namedWindow("Square Image", cv::WINDOW_NORMAL);
    imshow("Square Image", imgNLevel.img);
    cout << "Square Image Levels; " << imgNLevel.level << endl;
    cv::waitKey(0);

    return 0;
}
