#include "wbrocore.h"

WbroCore::WbroCore(){
    mVersion = "1.1.0";
}

const std::string WbroCore::Wbro_getVersion(){
    return mVersion;
}


cv::Mat WbroCore::Wbro_loadimage(const std::string path){
   return cv::imread(path, cv::IMREAD_COLOR);
}


std::vector<int> WbroCore::Wbro_readpixel3(cv::Mat *img, int pos_x, int pos_y){
    std::vector<int> vec;
    cv::Vec3b pixel = img->at<cv::Vec3b>(pos_x, pos_y);
    for (int i = 0; i < 3; i++){
        vec.push_back(pixel.val[i]);
    }
    return vec;
}
