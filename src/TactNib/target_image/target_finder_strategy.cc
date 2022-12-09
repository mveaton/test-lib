/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: target_finder_strategy.cc
 * Purpose:	  Abstract class for image finder algorithm. The purpose of this class is
 *            to implement the strategy design pattern to allow various algorithms
 *            to be used at runtime.
 * Author:	  Michael Eaton
 *
 * ============================================================================*/

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "target_finder_strategy.h"
#include "opencv_algo.h"

using namespace cv;
using namespace std;

namespace TactNib {

    //================================================
    // Default constructor
    //================================================
    TargetFinderStrategy::TargetFinderStrategy(std::string image_target_file, std::string image_template_file)
    {
        image_target_file_   = image_target_file;
        image_template_file_ = image_template_file;
    }

    //================================================
    // Member Function: ProcessImage
    //================================================
    void TargetFinderStrategy::ProcessImage()
    {
        OpencvAlgo algo;

        // Convert jpeg image to opencv matrix
        Mat img          = imread(image_target_file_, IMREAD_COLOR);
        Mat img_template = imread(image_template_file_, IMREAD_COLOR);

        // Adjust the brightness of target image to match the target template image
        img_template = algo.AdjustBrightness(img, img_template);

        // Find the paper target in the image
        FindTarget();
    }
} // Image