#include "Detector_seq.h"

class treasure_sign_detector : public Detector
{
private:
    /* data */
public:

    treasure_sign_detector(std::string template_path) : Detector(template_path){}


    ~treasure_sign_detector(){
        Detector::~Detector();
    }


    cv::Point get_relative_size(int img_width, int img_height){

        double temp_box_width = img_width / 672.0;
        int width = std::floor(temp_box_width * 197);

        double temp_box_height = img_height / 672.0;
        int height = std::floor(temp_box_height * 130);
        return cv::Point(width, height);
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


};




int main(){

    // cout << "Hello World" << endl;
    std::string template_path = "../pics/03-T.jpg";
    treasure_sign_detector* bt = new treasure_sign_detector(template_path);

    std::vector<cv::Mat> img_list;
    for (int num = 0; num < 24; num++)
    {
        std::string string_path = "../testimg/03-t-" + std::to_string(num) + "-1.jpg";
        cv::Mat img_o = cv::imread(string_path);
        img_list.push_back(img_o);
    }
    

    for (int index = 0; index < img_list.size(); index++)
    {
        // bt->find_sign(img_list.at(index));
        cv::Rect bt_draw_box = bt->find_sign(img_list.at(index));
        std::cout << "Img Index: " << index << " challenge button " << bt_draw_box << std::endl;
    }
    

    return 0;
}