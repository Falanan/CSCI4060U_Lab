#include "Build_In_Funcs.h"


cv::Point get_relative_size(int img_width, int img_height){

    double temp_box_width = img_width / 672.0;
    int width = std::floor(temp_box_width * 64);
    
    return cv::Point(width, width);
}

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



struct ThreadArgs {
    int start_index;
    int end_index;
    cv::Mat orig_img;
    std::vector<cv::Mat> template_list;
    std::vector<cv::Mat> gp_orig;
    std::vector<cv::Point> match_box_list;
    pthread_mutex_t* mutex;
};

void* match_template(void* arg) {
    
    ThreadArgs* thread_args = static_cast<ThreadArgs*>(arg);
    for (int temp_index = thread_args->start_index; temp_index < thread_args->end_index; temp_index++) {
        std::vector<cv::Mat> useful_match;
        for (int img_index = 0; img_index < thread_args->gp_orig.size(); img_index++) {
            if (thread_args->template_list.at(temp_index).cols <= thread_args->gp_orig.at(img_index).cols && 
                thread_args->template_list.at(temp_index).rows <= thread_args->gp_orig.at(img_index).rows) {
                cv::Mat R;
                cv::matchTemplate(thread_args->gp_orig.at(img_index), thread_args->template_list.at(temp_index), R, cv::TM_CCORR_NORMED);
                useful_match.push_back(R);
            }
        }
        std::vector<HighlightResult> R_val;
        for (int x = 0; x < useful_match.size(); x++) {
            cv::Mat R_ = useful_match.at(x);
            cv::Mat T_ = thread_args->template_list.at(temp_index);
            cv::Mat I_ = thread_args->gp_orig.at(x);
            HighlightResult res = highlight(R_, T_, I_);
            R_val.push_back(res);
        }
        int highest_match_pos = 0;
        for (int index = 0; index < R_val.size(); index++) {
            if (R_val.at(index).val > R_val.at(highest_match_pos).val) {
                highest_match_pos = index;
            }
        }
        cv::Point highest_match_loc = R_val.at(highest_match_pos).loc;
        pthread_mutex_lock(thread_args->mutex);
        thread_args->match_box_list.push_back(highest_match_loc * pow(2, highest_match_pos));
        pthread_mutex_unlock(thread_args->mutex);
    }
    return NULL;
}

cv::Rect find_sign(cv::Mat orig_img, std::vector<cv::Mat> template_list) {
    cv::Mat orig_img_copy = orig_img.clone();
    struct img_n_level sq_orig_img_and_nlevels = make_square(orig_img_copy);
    std::vector<cv::Mat> gp_orig = gen_gaussian_pyramid(sq_orig_img_and_nlevels.img, sq_orig_img_and_nlevels.level-3);
    std::vector<cv::Point> match_box_list;
    
    int num_threads = template_list.size();
    pthread_t threads[num_threads];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    ThreadArgs thread_args[num_threads];
    // int templates_per_thread = template_list.size() / num_threads;
    int templates_per_thread = 1;

    

    for (int i = 0; i < num_threads; i++) {
        thread_args[i].start_index = i * templates_per_thread;
        thread_args[i].end_index = (i + 1) * templates_per_thread;
        thread_args[i].orig_img = orig_img_copy;
        thread_args[i].template_list = template_list;
        thread_args[i].gp_orig = gp_orig;
        thread_args[i].match_box_list = match_box_list;
        thread_args[i].mutex = &mutex;
        pthread_create(&threads[i], NULL, match_template, &thread_args[i]);
    }
    

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    std::cout << "After join" << std::endl;
    
    // cv::Point highest_match_loc = match_box_list.at(0);
    // float highest_match_val = -1.0f;
    // for (int i = 0; i < match_box_list.size(); i++) {
    //     if (match_box_list.at(i).y > highest_match_val) {
    //         highest_match_loc = match_box_list.at(i);
    //         highest_match_val = match_box_list.at(i).y;
    //     }
    // }
    
    // int sign_width = template_list.at(0).cols;
    // int sign_height = template_list.at(0).rows;
    // int sign_x = highest_match_loc.x - (sign_width / 2);
    // int sign_y = highest_match_loc.y - (sign_height / 2);
    // cv::Rect sign_rect(sign_x, sign_y, sign_width, sign_height);
    // return sign_rect;
    cv::Rect sign_rect={1, 1, 1, 1};
    return sign_rect;
}



int main(){
    std::vector<cv::Mat> btn_template_list;
    std::vector<cv::Mat> trea_template_list;

    cv::Mat btn_template_orig = cv::imread("../pics/03-T.jpg");
    cv::Mat btn_template_resize;
    cv::resize(btn_template_orig, btn_template_resize, cv::Size(128,128));
    struct img_n_level sq_btn_temp_and_nlevels = make_square(btn_template_resize);
    btn_template_list.push_back(sq_btn_temp_and_nlevels.img);

    std::vector<cv::Mat> hr_btn_temp = half_resolution_image(sq_btn_temp_and_nlevels.img, sq_btn_temp_and_nlevels.level-3);

    for (int index_btn_hr = 0; index_btn_hr < hr_btn_temp.size(); index_btn_hr++)
    {
        struct img_n_level sq_temp_img = make_square(hr_btn_temp.at(index_btn_hr));
        std::vector<cv::Mat> gpT = gen_gaussian_pyramid(sq_temp_img.img, sq_temp_img.level);
        for (int gpt_index = 0; gpt_index < gpT.size(); gpt_index++)
        {
            btn_template_list.push_back(gpT.at(gpt_index));
        }  
    }

    

    std::vector<cv::Mat> img_list;
    for (int num = 0; num < 23; num++)
    {
        std::string string_path = "../testimg/03-t-" + std::to_string(num) + "-1.jpg";
        cv::Mat img_o = cv::imread(string_path);
        // cv::cvtColor(img_o, img_o, cv::COLOR_BGR2RGB);
        img_list.push_back(img_o);
    }

    for (int num = 0; num < 23; num++)
    {
        std::string string_path = "../testimg/03-t-" + std::to_string(num) + "-0.jpg";
        cv::Mat img_o = cv::imread(string_path);
        // cv::cvtColor(img_o, img_o, cv::COLOR_BGR2RGB);
        img_list.push_back(img_o);
    }

    for (int index = 0; index < img_list.size(); index++)
    {
        find_sign(img_list.at(index), btn_template_list);
        // cv::Rect bt_draw_box = find_sign(img_list.at(index), btn_template_list);
        // std::cout << "Img Index: " << index << " challenge button " << bt_draw_box << std::endl;
        // cv::Mat match_img = draw_rect(img_list.at(index), draw_box);
        // namedWindow("Square Image", cv::WINDOW_NORMAL);
        // imshow("Square Image", match_img);
        // cv::waitKey(0);

    }


    return 0;
}