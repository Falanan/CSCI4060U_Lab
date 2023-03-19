// #include <iostream>
// #include <vector>
// #include <opencv2/opencv.hpp>
// #include <opencv2/imgproc.hpp>
// #include <cmath>
// #include <algorithm>

#include "Build_In_Funcs.h"
#define NUM_THREADS 4

using namespace std;

class challenge_btn_det_class
{
private:
    std::vector<cv::Mat> template_list;
public:

challenge_btn_det_class(string template_path){
    // read the image
    // cv::Mat template_orig = cv::imread("pics/03-T.jpg");
    cv::Mat template_orig = cv::imread(template_path);
    // resize the template to 128*128
    cv::Mat template_resize;
    cv::resize(template_orig, template_resize, cv::Size(128,128));

    struct img_n_level sq_temp_and_nlevels = make_square(template_resize);
    this->template_list.push_back(sq_temp_and_nlevels.img);

    // generate half-resulation images for the original template
    std::vector<cv::Mat> hr_temp = half_resolution_image(sq_temp_and_nlevels.img, sq_temp_and_nlevels.level-3);

    // generate gaussian down pyramid for each resolution level, then append the image to the template list
    // #pragma omp parallel for
    for (int index_hr = 0; index_hr < hr_temp.size(); index_hr++)
    {
        struct img_n_level sq_temp_img = make_square(hr_temp.at(index_hr));
        std::vector<cv::Mat> gpT = gen_gaussian_pyramid(sq_temp_img.img, sq_temp_img.level);
        for (int gpt_index = 0; gpt_index < gpT.size(); gpt_index++)
        {
            this->template_list.push_back(gpT.at(gpt_index));
        }  
    }
    
}

// Deconstructor
~challenge_btn_det_class(){
    for (auto& mat : template_list) {
            mat.release();
        }
    template_list.clear();
}

cv::Point get_relative_size(int img_width, int img_height){

    double temp_box_width = img_height / 672.0;
    int width = std::floor(temp_box_width * 64);
    
    return cv::Point(width, width);
}


cv::Point find_sign(cv::Mat orig_img){
    cv::Mat orig_img_copy = orig_img.clone();
    struct img_n_level sq_orig_img_and_nlevels = make_square(orig_img_copy);

    std::vector<cv::Mat> gp_orig = gen_gaussian_pyramid(sq_orig_img_and_nlevels.img, sq_orig_img_and_nlevels.level-3);

    std::vector<cv::Point> match_box_list;

    for (int temp_index = 0; temp_index < this->template_list.size(); temp_index++)
    {
        // cout << "Index: " << temp_index << endl;
        std::vector<cv::Mat> useful_match;
        for (int img_index = 0; img_index < gp_orig.size(); img_index++)
        {
            if (this->template_list.at(temp_index).cols <= gp_orig.at(img_index).cols && this->template_list.at(temp_index).rows <= gp_orig.at(img_index).rows)
                {
                    // cout << " Usefur matches" << endl;
                    cv::Mat R;
                    cv::matchTemplate(gp_orig.at(img_index), this->template_list.at(temp_index), R, cv::TM_CCORR_NORMED);
                    useful_match.push_back(R);
                }
        }

        vector<HighlightResult> R_val;
        for (int x = 0; x < useful_match.size(); x++)
        {
            cv::Mat R_ = useful_match.at(x);
            cv::Mat T_ = this->template_list.at(temp_index);
            cv::Mat I_ = gp_orig.at(x);

            HighlightResult res = highlight(R_, T_, I_);
            R_val.push_back(res);
        }

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
        
    }

    cv::Point match_point = find_match_box(match_box_list);

    cv::Point box_size = get_relative_size(orig_img.rows, orig_img.cols);
    

    cv::Rect draw_box = {match_point.x, match_point.y, box_size.x, box_size.y};
    // cv::Mat match_img = draw_rect(orig_img_copy, draw_box);
    // namedWindow("Original Image", cv::WINDOW_NORMAL);
    // imshow("Original Image",match_img);
    // cv::waitKey(0);
    return match_point;

}

};




int main(){

    omp_set_num_threads(NUM_THREADS);
    // cout << "Hello World" << endl;
    string template_path = "../pics/03-T.jpg";
    cv::Mat img = cv::imread("../testimg/03-t-0-1.jpg");
    challenge_btn_det_class* bt = new challenge_btn_det_class(template_path);

    bt->find_sign(img);


    return 0;
}