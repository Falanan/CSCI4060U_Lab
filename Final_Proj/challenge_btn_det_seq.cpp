#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <algorithm>

// g++ challenge_btn_det_seq.cpp `pkg-config --cflags --libs opencv4` -- x86 Ubuntu
// g++ challenge_btn_det_seq.cpp -std=c++11 `pkg-config --cflags --libs opencv4` --arm64 Ubuntu


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


cv::Mat draw_rect(cv::Mat I, cv::Rect bbox) {
    /*
    This function is used to draw the final match box
    */
    cv::Mat I_ = I.clone();
    cv::Scalar c = (I_.type() == CV_32F) ? cv::Scalar(1.0, 0, 0) : cv::Scalar(255, 0, 0);
    cv::rectangle(I_, bbox, c, 3);
    return I_;
}



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
std::vector<cv::Mat> gen_gaussian_pyramid(cv::Mat I, int levels=6)
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
    // cout << n_levels << endl;
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


cv::Point find_match_box(vector<cv::Point> match_box_list){

    vector<int> x_val;
    vector<int> y_val;

    for (int index = 0; index < match_box_list.size(); index++)
    {
        x_val.push_back(match_box_list.at(index).x);
        y_val.push_back(match_box_list.at(index).y);
    }

    vector<int> highest_possible_x_pos;
    vector<int> highest_possible_y_pos;


    for (int index = 0; index < x_val.size(); index++)
    {
        int count = 0;

        if (find(x_val.begin(), x_val.end(), x_val.size()) == x_val.end())
        {
            for (int num = x_val.at(index)-3; num < x_val.at(index)+3; num++)
            {
                if (find(x_val.begin(), x_val.end(), num) != x_val.end())
                {
                    count += std::count(x_val.begin(), x_val.end(), num);
                }          
                
            }
        }

        if (count >= 7)
        {
            highest_possible_x_pos.push_back(x_val.at(index));
        }

        // int count = std::count(x_val.begin(), x_val.end(), x_val.at(index));
        // cout << x_val.at(index) << " appear " << count << " times" << endl;
    }

    // cout << "X pos:" << endl;
    // for (int i = 0; i < highest_possible_x_pos.size(); i++)
    // {
    //     cout << highest_possible_x_pos.at(i) << endl;
    // }


    
    for (int index = 0; index < y_val.size(); index++)
    {
        int count = 0;

        if (find(y_val.begin(), y_val.end(), y_val.size()) == y_val.end())
        {
            for (int num = y_val.at(index)-3; num < y_val.at(index)+3; num++)
            {
                if (find(y_val.begin(), y_val.end(), num) != y_val.end())
                {
                    count += std::count(y_val.begin(), y_val.end(), num);
                }          
                
            }
        }

        if (count >= 7)
        {
            highest_possible_y_pos.push_back(y_val.at(index));
        }

        // int count = std::count(y_val.begin(), y_val.end(), y_val.at(index));
        // cout << y_val.at(index) << " appear " << count << " times" << endl;
    }

    // cout << "Y pos:" << endl;
    // for (int i = 0; i < highest_possible_y_pos.size(); i++)
    // {
    //     cout << highest_possible_y_pos.at(i) << endl;
    // }

    int x_avg = 0;
    int y_avg = 0;

    for (int index = 0; index < highest_possible_x_pos.size(); index++)
    {
        x_avg += highest_possible_x_pos.at(index);
    }

    for (int index = 0; index < highest_possible_y_pos.size(); index++)
    {
        y_avg += highest_possible_y_pos.at(index);
    }
    
    x_avg = x_avg / highest_possible_x_pos.size();
    y_avg = y_avg / highest_possible_y_pos.size();
    
    // cout << "x_avg: " << x_avg << " y_avg: " <<  y_avg << endl;

    cv::Point match_pos = {x_avg, y_avg};

    cout << match_pos << endl;


    return match_pos;


}



void find_sign(cv::Mat orig_img, cv::Mat orig_template){

    // make a copy of the tamplate and original image to avoid modify the original image
    cv::Mat orig_img_copy = orig_img.clone();
    cv::Mat orig_temp_copy = orig_template.clone();

    // squared the original image and generate the gaussian down pyramid
    struct img_n_level sq_orig_img_and_nlevels = make_square(orig_img_copy);
    std::vector<cv::Mat> gp_orig = gen_gaussian_pyramid(sq_orig_img_and_nlevels.img, sq_orig_img_and_nlevels.level-3);

    // squated the original template and generate gaussian down pyramid
    struct img_n_level sq_orig_temp_and_nlevels = make_square(orig_temp_copy);
    std::vector<cv::Mat> hr_temp = half_resolution_image(sq_orig_temp_and_nlevels.img, sq_orig_img_and_nlevels.level-4);

    vector<cv::Point> match_box_list;

    for (int i = 0; i < hr_temp.size(); i++)
    {
        struct img_n_level sq_temp_img = make_square(hr_temp.at(i));
        std::vector<cv::Mat> gpT = gen_gaussian_pyramid(sq_temp_img.img, sq_temp_img.level);

        for (int j = 0; j < gpT.size(); j++)
        {

            std::vector<cv::Mat> useful_match;

            for (int  k = 0; k < gp_orig.size(); k++)
            {
                // cout << "K = " << k;
                if (gpT.at(j).cols <= gp_orig.at(k).cols && gpT.at(j).rows <= gp_orig.at(k).rows)
                {
                    // cout << " Usefur matches" << endl;
                    cv::Mat R;
                    cv::matchTemplate(gp_orig.at(k), gpT.at(j), R, cv::TM_CCORR_NORMED);
                    useful_match.push_back(R);
                    // namedWindow("Original Image", cv::WINDOW_NORMAL);
                    // imshow("Original Image", R);
                }
                // cv::Mat R_val(useful_match.size(), 3, CV_64F);
                // cout << R_val.at<double>(0, 1) << endl;
                // cout << endl;
            }
            vector<HighlightResult> R_val;
            for (int x = 0; x < useful_match.size(); x++)
            {
                cv::Mat R_ = useful_match.at(x);
                cv::Mat T_ = gpT.at(j);
                cv::Mat I_ = gp_orig.at(x);

                HighlightResult res = highlight(R_, T_, I_);
                R_val.push_back(res);
                // R_val.at<double>(i, 0) =  = res.img
                // R_val.at<double>(i, 1) =  = res.loc
                // R_val.at<double>(i, 2) =  = res.val
                // cout << res.val << " at " << res.loc << endl;
                // namedWindow("Original Image", cv::WINDOW_NORMAL);
                // imshow("Original Image",res.img);
                // cv::waitKey(0);
            }
            // cout << endl;

            int highest_match_pos = 0;
            for (int index = 0; index < R_val.size(); index++)
            {
                if (R_val.at(index).val > R_val.at(highest_match_pos).val)
                {
                        highest_match_pos = index;
                }
            }

            cv::Point highest_match_loc = R_val.at(highest_match_pos).loc;
            match_box_list.push_back(highest_match_loc * pow(2, highest_match_pos));
            // cout << pow(2, highest_match_pos) << endl;
            // cout << highest_match_loc * pow(2, highest_match_pos) << endl;

            
        }
    }
    cv::Point match_pos = find_match_box(match_box_list);

    cv::Rect draw_box = {match_pos.x, match_pos.y, 32, 32};

    cv::Mat match_img = draw_rect(orig_img_copy, draw_box);
    namedWindow("Original Image", cv::WINDOW_NORMAL);
    imshow("Original Image",match_img);
    cv::waitKey(0);




    
}







int main()
{
    // Read input image
    // cv::Mat I = cv::imread("test_img/03-t-0-0.jpg");
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


    // cv::Mat img = cv::imread("test_img/03-t-0-0.jpg");
    // if (img.empty()) {
    //     cerr << "Error: could not read image file." << endl;
    //     return -1;
    // }

    // // make the image square
    // // cv::Mat square_img = make_square(img);
    // struct img_n_level imgNLevel = make_square(img);

    // // display the original and square images
    // namedWindow("Original Image", cv::WINDOW_NORMAL);
    // imshow("Original Image", img);
    // namedWindow("Square Image", cv::WINDOW_NORMAL);
    // imshow("Square Image", imgNLevel.img);
    // cout << "Square Image Levels; " << imgNLevel.level << endl;
    // cv::waitKey(0);

    // return 0;


    cv::Mat img = cv::imread("testimg/03-t-0-1.jpg");

    cv::Mat temp_orig = cv::imread("pics/03-T.jpg");

    cv::Mat template_resize;
    cv::resize(temp_orig, template_resize, cv::Size(128,128));

    // namedWindow("Original Image", cv::WINDOW_NORMAL);
    // imshow("Original Image",temp_orig);
    // cv::waitKey(0);
    find_sign(img, template_resize);

    return 0;




}
