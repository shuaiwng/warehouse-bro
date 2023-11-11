#include <string>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/core/core.hpp"


class WbroCore{
public:
    WbroCore();
    const std::string getVersion();
    float multiply(float num1, float num2);

    cv::Mat loadimage(const std::string path);
    std::vector<int> readpixel3(cv::Mat *img, int pos_x, int pos_y);


private:
    std::string mVersion;

};