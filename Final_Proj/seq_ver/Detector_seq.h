#include "Build_In_Funcs.h"

class Detector
{
protected:
    std::vector<cv::Mat> template_list;
public:

Detector(){}

Detector(std::string template_path){
    // read the image
    // cv::Mat template_orig = cv::imread("pics/03-T.jpg");
    cv::Mat template_orig = cv::imread(template_path);
    // resize the template to 128*128
    cv::Mat template_resize;
    cv::resize(template_orig, template_resize, cv::Size(128,128));

    // cv::Mat template_cvtcolor;
    // cv::cvtColor(template_resize, template_cvtcolor, cv::COLOR_BGR2RGB);

    struct img_n_level sq_temp_and_nlevels = make_square(template_resize);
    this->template_list.push_back(sq_temp_and_nlevels.img);

    // generate half-resulation images for the original template
    std::vector<cv::Mat> hr_temp = half_resolution_image(sq_temp_and_nlevels.img, sq_temp_and_nlevels.level-3);

    // generate gaussian down pyramid for each resolution level, then append the image to the template list
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
~Detector(){
    for (auto& mat : template_list) {
            mat.release();
        }
    template_list.clear();
}

virtual cv::Point get_relative_size(int img_width, int img_height){

    double temp_box_width = img_width / 672.0;
    int width = std::floor(temp_box_width * 64);
    
    return cv::Point(width, width);
}


virtual cv::Rect find_sign(cv::Mat orig_img){
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
        
        std::vector<HighlightResult> R_val;
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

    


    cv::Point match_point = validate_pos(find_match_box(match_box_list), orig_img_copy);

    // std::cout << match_point << std::endl;

    cv::Point box_size = get_relative_size(orig_img.cols, orig_img.rows);
    

    cv::Rect draw_box = {match_point.x, match_point.y, box_size.x, box_size.y};
    // std::cout << draw_box << std::endl;
    // cv::Mat match_img = draw_rect(orig_img_copy, draw_box);
    // namedWindow("Original Image", cv::WINDOW_NORMAL);
    // imshow("Original Image",match_img);
    // cv::waitKey(0);
    return draw_box;

}


virtual cv::Point validate_pos(cv::Point match, cv::Mat orig_img){
    if (match.y > orig_img.cols/2 || match.x < orig_img.rows/3)
    {
        cv::Point new_box = {0,0};
        return new_box;
    }else{
        return match;
    }
    

}

virtual cv::Point find_match_box(std::vector<cv::Point> match_box_list){

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


};
