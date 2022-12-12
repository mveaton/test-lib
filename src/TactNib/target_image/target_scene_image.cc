/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: target_scene_image.cc
 * Purpose:	  Class for image of a paper target. The purpose of this class is
 *            to find the paper target within the image and score the target
 *            within the image. This class will provide the ability to preprocess
 *            the image (adjust colors), select finder algorithm, find and align
 *            image, and score the target.
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
 *
 * ============================================================================*/

#include <string>

#include "target_scene_image.h"
#include "object_detect_strategy.h"
#include "opencv_strategy.h"

namespace TactNib {

    //================================================
    // Default constructor
    //================================================
    TargetSceneImage::TargetSceneImage()
    {
        target_finder_strategy_ = nullptr;
    }

    //================================================
    // Member Function: SetTargetFinderStrategy
    //================================================
    void TargetSceneImage::SetTargetFinderStrategy(TargetFinderStrategyType id)
    {
        delete target_finder_strategy_;

        switch(id)
        {
            case TargetFinderStrategyType::ObjectDetect:
                target_finder_strategy_ = new ObjectDetectStrategy(image_scene_file_, image_target_file_);
                break;
            case TargetFinderStrategyType::OpenCvRect:
                target_finder_strategy_ = new OpenCvStrategy(image_scene_file_, image_target_file_);
                break;
            default:
                break;
        }
    }

    //================================================
    // Member Function: SetSceneImage
    //================================================
    void TargetSceneImage::SetSceneImage(std::string image_file)
    {
        image_scene_file_ = image_file;
    }

    //================================================
    // Member Function: SetTargetImage
    //================================================
    void TargetSceneImage::SetTargetImage(std::string image_file)
    {
        image_target_file_ = image_file;
    }

    //================================================
    // Member Function: ProcessTarget
    //================================================
    void TargetSceneImage::ProcessScene()
    {
        // Find the paper target in the image
        target_finder_strategy_->ProcessImage();
    }

} // TactNib