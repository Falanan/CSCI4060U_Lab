#include "Build_In_Funcs.h"


cv::Point validate_pos(cv::Point match, cv::Mat orig_img){
    if (match.y < orig_img.cols/2 || match.x < orig_img.rows/2)
    {
        cv::Point new_box = {0,0};
        return new_box;
    }else{
        return match;
    }
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
                    count += std::count(x_val.begin(), x_val.end(), num);
                }          
                
            }
        }

        if (count >= 7)
        {
            highest_possible_x_pos.push_back(x_val.at(index));
        }
    }

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
    }

    
    // std::cout << "X pos:" << std::endl;
    // for (int i = 0; i < highest_possible_x_pos.size(); i++)
    // {
    //     std::cout << highest_possible_x_pos.at(i) << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "Y pos:" << std::endl;
    // for (int i = 0; i < highest_possible_y_pos.size(); i++)
    // {
    //     std::cout << highest_possible_y_pos.at(i) << " ";
    // }
    // std::cout << std::endl;

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

    // std::cout << match_pos << std::endl;


    return match_pos;

}


int main(){
    std::vector<cv::Mat> btn_template_list;
    std::vector<cv::Mat> trea_template_list;

    cv::Mat btn_template_orig = cv::imread("pics/03-T.jpg");
    cv::Mat btn_template_resize;
    cv::resize(btn_template_orig, btn_template_resize, cv::Size(128,128));
    struct img_n_level sq_btn_temp_and_nlevels = make_square(btn_template_resize);
    btn_template_list.push_back(sq_btn_temp_and_nlevels.img);

    std::vector<cv::Mat> hr_btn_temp = half_resolution_image(sq_btn_temp_and_nlevels.img, sq_btn_temp_and_nlevels.level-3);

    for (int index_btn_hr = 0; index_btn_hr < hr_temp.size(); index_btn_hr++)
    {
        struct img_n_level sq_temp_img = make_square(hr_temp.at(index_btn_hr));
        std::vector<cv::Mat> gpT = gen_gaussian_pyramid(sq_temp_img.img, sq_temp_img.level);
        for (int gpt_index = 0; gpt_index < gpT.size(); gpt_index++)
        {
            btn_template_list.push_back(gpT.at(gpt_index));
        }  
    }


    return 0;
}