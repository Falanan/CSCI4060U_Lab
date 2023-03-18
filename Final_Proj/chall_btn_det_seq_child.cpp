
#include "detector_seq.h"

class clallenge_btn_detector : public detector
{
private:
    /* data */
public:
    clallenge_btn_detector(string template_path) : detector(template_path){

    }
    ~clallenge_btn_detector(){
        detector::~detector();
    }
};


int main(){

    // cout << "Hello World" << endl;
    string template_path = "pics/03-T.jpg";
    cv::Mat img = cv::imread("testimg/03-t-0-1.jpg");
    clallenge_btn_detector* bt = new clallenge_btn_detector(template_path);

    bt->find_sign(img);



    const int n = 1000000;
    int arr[n];
    int sum = 0;

    // Initialize the array
    for (int i = 0; i < n; i++)
    {
        arr[i] = i + 1;
    }

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
    }

    std::cout << "The sum is: " << sum << std::endl;



    return 0;
}