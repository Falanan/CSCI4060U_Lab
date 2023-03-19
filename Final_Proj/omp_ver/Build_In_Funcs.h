#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <algorithm>
#include <string>
#include <omp.h>

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
std::vector<cv::Mat> gen_gaussian_pyramid(cv::Mat I, int levels=6)
{
    std::vector<cv::Mat> pI;
    cv::Mat G = I.clone();
    pI.push_back(G);

    #pragma omp parallel for
    for (int i = 0; i < levels; i++)
    {
        #pragma omp critical
        cv::pyrDown(G, G);
        #pragma omp critical
        pI.push_back(G);
    }

    return pI;
}

/**
 * This function is used to generate half resulation images
 * Returns a vector contains different resulation images
 * */
// std::vector<cv::Mat> half_resolution_image(cv::Mat I, int levels = 6)
// {
//     std::vector<cv::Mat> pI;
//     cv::Mat G = I.clone();
//     pI.push_back(G);
//     for (int i = 0; i < levels; i++)
//     {
//         cv::Mat H;
//         cv::resize(G, H, cv::Size(G.cols / 2, G.rows / 2));
//         pI.push_back(H);
//         G = H.clone();
//     }
//     return pI;
// }
std::vector<cv::Mat> half_resolution_image(cv::Mat I, int levels = 6)
{
    std::vector<cv::Mat> pI;
    cv::Mat G = I.clone();
    pI.push_back(G);

    #pragma omp parallel for
    for (int i = 0; i < levels; i++)
    {
        #pragma omp critical
        cv::resize(G, G, cv::Size(G.cols / 2, G.rows / 2));
        #pragma omp critical
        pI.push_back(G);
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


cv::Point find_match_box(std::vector<cv::Point> match_box_list){

    std::vector<int> x_val;
    std::vector<int> y_val;

    for (int index = 0; index < match_box_list.size(); index++)
    {
        x_val.push_back(match_box_list.at(index).x);
        y_val.push_back(match_box_list.at(index).y);
    }

    std::vector<int> highest_possible_x_pos;
    std::vector<int> highest_possible_y_pos;

    // omp_set_num_threads(2);

    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int index = 0; index < x_val.size(); index++)
        {
            // int thread_id = omp_get_thread_num();
            // std::cout << "ID: " << thread_id << std::endl;
            int count = 0;

            if (find(x_val.begin(), x_val.end(), x_val.size()) == x_val.end())
            {
                for (int num = x_val.at(index)-3; num < x_val.at(index)+3; num++)
                {
                    if (find(x_val.begin(), x_val.end(), num) != x_val.end())
                    {
                        #pragma omp atomic
                        count += std::count(x_val.begin(), x_val.end(), num);
                    }          
                    
                }
            }

            if (count >= 7)
            {
                #pragma omp critical
                highest_possible_x_pos.push_back(x_val.at(index));
            }
        }

        #pragma omp for nowait
        for (int index = 0; index < y_val.size(); index++)
        {
            int count = 0;

            if (find(y_val.begin(), y_val.end(), y_val.size()) == y_val.end())
            {
                for (int num = y_val.at(index)-3; num < y_val.at(index)+3; num++)
                {
                    if (find(y_val.begin(), y_val.end(), num) != y_val.end())
                    {
                        #pragma omp atomic
                        count += std::count(y_val.begin(), y_val.end(), num);
                    }          
                    
                }
            }

            if (count >= 7)
            {
                #pragma omp critical
                highest_possible_y_pos.push_back(y_val.at(index));
            }
        }



    }

    
    std::cout << "X pos:" << std::endl;
    for (int i = 0; i < highest_possible_x_pos.size(); i++)
    {
        std::cout << highest_possible_x_pos.at(i) << " ";
    }
    std::cout << std::endl;

    std::cout << "Y pos:" << std::endl;
    for (int i = 0; i < highest_possible_y_pos.size(); i++)
    {
        std::cout << highest_possible_y_pos.at(i) << " ";
    }
    std::cout << std::endl;

    int x_avg = 0;
    int y_avg = 0;

    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int index = 0; index < highest_possible_x_pos.size(); index++)
        {
            #pragma omp atomic
            x_avg += highest_possible_x_pos.at(index);
        }

        #pragma omp for nowait
        for (int index = 0; index < highest_possible_y_pos.size(); index++)
        {
            #pragma omp atomic
            y_avg += highest_possible_y_pos.at(index);
        }
    
    }
    
    x_avg = x_avg / highest_possible_x_pos.size();
    y_avg = y_avg / highest_possible_y_pos.size();
    
    // cout << "x_avg: " << x_avg << " y_avg: " <<  y_avg << endl;

    cv::Point match_pos = {x_avg, y_avg};

    std::cout << match_pos << std::endl;


    return match_pos;

}





cv::Mat draw_rect(cv::Mat I, cv::Rect bbox) {
    /*
    This function is used to draw the final match box
    */
    cv::Mat I_ = I.clone();
    cv::Scalar c = (I_.type() == CV_32F) ? cv::Scalar(1.0, 0, 0) : cv::Scalar(255, 0, 0);
    cv::rectangle(I_, bbox, c, 3);
    return I_;
}

