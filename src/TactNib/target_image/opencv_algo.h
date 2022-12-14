/** ===========================================================================
 *
 * File Name: opencv_algo.h
 * Purpose:	  Placeholder for algorithms utilizing OpenCV found on the internet
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
 *
 * ============================================================================*/

#ifndef SYSTEM_API_OPENCV_ALGO_H
#define SYSTEM_API_OPENCV_ALGO_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

namespace TactNib {
    namespace OpencvAlgo {
        struct Image_Stat {
            cv::Mat mean[3];
            cv::Mat stddev[3];
        };

        Mat FindBlob(const Mat &);
        Mat AlignImageMotion(const Mat &im1, const Mat &im2);
        Scalar GetMSSIM(const Mat &, const Mat &);
        Mat AdjustBrightness(const Mat &, const Mat &);
        Image_Stat GetStat(const Mat &);

    }// OpencvAlgo
} // TactNib

#endif //SYSTEM_API_OPENCV_ALGO_H
