#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <iostream>
using namespace std;

int main(){

    cout << "hello world" << endl;

    cv::Mat image = cv::imread("lenna.png",cv::IMREAD_GRAYSCALE);

    if(image.empty()){
        std::cerr << "Image not Found\n";
        return -1;
    }

    const std::string window_name{"lenna"};
    cv::namedWindow(window_name);
    cv::imshow(window_name,image);
    cv::waitKey(0);
    return 0;
}