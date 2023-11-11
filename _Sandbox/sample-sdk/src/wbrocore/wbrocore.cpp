#include "wbrocore.h"

WbroCore::WbroCore(){
    mVersion = "1.1.0";
    std::cout << "Core initialized." << std::endl;
}

const std::string WbroCore::getVersion(){
    return mVersion;
}


float WbroCore::multiply(float num1, float num2){
    return num1 * num2;
}


cv::Mat WbroCore::loadimage(const std::string path){
   return cv::imread(path, cv::IMREAD_COLOR);
}


std::vector<int> WbroCore::readpixel3(cv::Mat *img, int pos_x, int pos_y){
    std::vector<int> vec;
    cv::Vec3b pixel = img->at<cv::Vec3b>(pos_x, pos_y);
    for (int i = 0; i < 3; i++){
        vec.push_back(pixel.val[i]);
    }
    return vec;
}
