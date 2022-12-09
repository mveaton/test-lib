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
 * ============================================================================*/

#include "object_detect_strategy.h"


namespace TactNib {

    //================================================
    // Default constructor
    //================================================
    ObjectDetectStrategy::ObjectDetectStrategy(std::string image_target_file, std::string image_template_file):TargetFinderStrategy(image_target_file, image_template_file){}

    //================================================
    // Member Function: FindTarget
    //================================================
    void ObjectDetectStrategy::FindTarget() {
        // Todo: Add YOLO object detection logic
    }

} // TactNib