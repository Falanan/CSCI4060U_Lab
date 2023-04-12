#include "Detector_pa.h"

class clallenge_btn_detector : public Detector
{
private:
    /* data */
public:
    clallenge_btn_detector(std::string template_path) : Detector(template_path){

    }
    ~clallenge_btn_detector(){
        Detector::~Detector();
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

        // std::cout << match_pos << std::endl;


        return match_pos;

    }


};


int main(){

    omp_set_num_threads(4);
    // cout << "Hello World" << endl;
    std::string template_path = "../pics/03-T.jpg";
    cv::Mat img = cv::imread("../testimg/03-t-0-1.jpg");
    clallenge_btn_detector* bt = new clallenge_btn_detector(template_path);

    std::vector<cv::Mat> img_list;
    for (int num = 0; num < 23; num++)
    {
        std::string string_path = "../testimg/03-t-" + std::to_string(num) + "-1.jpg";
        cv::Mat img_o = cv::imread(string_path);
        img_list.push_back(img_o);
    }
    

    for (int index = 0; index < img_list.size(); index++)
    {
        bt->find_sign(img_list.at(index));
    }
    
    bt->find_sign(img);

    return 0;
}