/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: object_detect_strategy.cc
 * Purpose:	  Concrete class for image finder algorithm. The purpose of this class is
 *            to implement the strategy design pattern to allow various algorithms
 *            to be used at runtime. This class will implement the "FindTarget" function
 *            that will use an object detection model to find the paper target.
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
 *
 * ============================================================================*/

#include "object_detect_strategy.h"
#include "opencv_algo.h"

namespace TactNib {

    //================================================
    // Default constructor
    //================================================
    ObjectDetectStrategy::ObjectDetectStrategy(std::string image_target_file, std::string image_template_file):TargetFinderStrategy(image_target_file, image_template_file){}

    //================================================
    // Member Function: FindTarget
    //================================================
    TargetObjectImage ObjectDetectStrategy::FindTarget() {

        // Convert jpg file to opencv matrix
        Mat image_scene  = imread(image_scene_file_, IMREAD_COLOR);
        Mat image_object = imread(image_target_file_, IMREAD_COLOR);

        // Todo: Process scene image with YOLO5 OBB object detection logic

        // Step : Get target object corners from YOLO5 results
            // TargetObject.corner

        // Step : Get target object image from YOLO5 results
            // TargetObject.image

        // Step : Dewarp  target object image
        Mat image_dewarp;
            // TargetObject.image

        // Step : Get alignment score of target object image
        std::cout << "Step Z: Score the alignment of scene's target and template target" << std::endl;
        Scalar results = OpencvAlgo::GetMSSIM(image_object, image_dewarp);
            // TargetObject.score

        // TODO: Clean up return code
        TargetObjectImage target_object;
        return target_object;
    }

} // TactNib