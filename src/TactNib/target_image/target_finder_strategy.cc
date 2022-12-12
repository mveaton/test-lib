/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: target_finder_strategy.cc
 * Purpose:	  Abstract class for image finder algorithm. The purpose of this class is
 *            to implement the strategy design pattern to allow various algorithms
 *            to be used at runtime.
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
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
    TargetFinderStrategy::TargetFinderStrategy(std::string image_scene_file, std::string image_target_file)
    {
        image_scene_file_  = image_scene_file;
        image_target_file_ = image_target_file;
    }

    //================================================
    // Member Function: ProcessImage
    //================================================
    void TargetFinderStrategy::ProcessImage()
    {

        // Convert jpeg image to opencv matrix
        Mat image_scene  = imread(image_scene_file_, IMREAD_COLOR);
        Mat image_target = imread(image_target_file_, IMREAD_COLOR);

        //////////////////////////////////////////
        // Draw a box around paper target edge
        // https://learnopencv.com/automatic-document-scanner-using-opencv/
        //////////////////////////////////////////

        // Step (1) Start with morphological operations to get a blank paper target.
        // Create 5x5 kernel aka structuring element, anchor is at center
        int morph_size = 2;
        Mat element = getStructuringElement(MORPH_RECT,Size(2 * morph_size + 1,2 * morph_size + 1),Point(morph_size, morph_size));

        // Repeated Closing operation to remove content from paper target.
        Mat image_step1;
        morphologyEx(image_scene, image_step1, MORPH_CLOSE, element, Point(-1, -1), 10);
        imshow("morphologyEx", image_step1);

        // Step (2) Find the paper target in the image
        TargetObjectImage target = FindTarget();

        imshow("LAST IMAGE", target.image_);
        int k = waitKey(0); // Wait for a keystroke in the window
    }
} // Image