/**
 * @file wbrocore.h
 * @brief Defines SDK Core functions.
 */

#include <string>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/core/core.hpp"


class WbroCore{
public:
    WbroCore();
    /**
    * @brief get the SDK version.
    * This function returns the SDK version.
    * @return Version number as string.
    */
    const std::string Wbro_getVersion();

    /**
    * @brief Load a PNG image.
    * This function loads a PNG image.
    * @return Opencv Mat.
    * @param path Path to the image.   
    */
    cv::Mat Wbro_loadimage(const std::string path);

    /**
    * @brief Read RGB-values of a single pixel.
    * This function uses opencv method to read RGB-values from a pixel.
    * @return A vector containing RGB-values.
    * @param img Pointer to the image to be read.
    * @param pos_x Position x on the image.
    * @param pos_y Position y on the image.    
    */
    std::vector<int> Wbro_readpixel3(cv::Mat *img, int pos_x, int pos_y);


private:
    std::string mVersion;

};