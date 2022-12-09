/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: target_image.cc
 * Purpose:	  Class for image of a paper target. The purpose of this class is
 *            to find the paper target within the image and score the target
 *            within the image. This class will provide the ability to preprocess
 *            the image (adjust colors), select finder algorithm, find and align
 *            image, and score the target.
 * Author:	  Michael Eaton
 *
 * ============================================================================*/

#include <string>
#include <opencv2/core.hpp>

#include "target_image.h"
#include "object_detect_strategy.h"
#include "opencv_strategy.h"

using namespace cv;

namespace TactNib {

    //================================================
    // Default constructor
    //================================================
    TargetImage::TargetImage()
    {
        target_finder_strategy_ = nullptr;
    }

    //================================================
    // Member Function: SetTargetFinderStrategy
    //================================================
    void TargetImage::SetTargetFinderStrategy(TargetFinderStrategyType id)
    {
        delete target_finder_strategy_;

        switch(id)
        {
            case TargetFinderStrategyType::ObjectDetect:
                target_finder_strategy_ = new ObjectDetectStrategy(image_target_file_, image_template_file_);
                break;
            case TargetFinderStrategyType::OpenCvRect:
                target_finder_strategy_ = new OpenCvStrategy(image_target_file_, image_template_file_);
                break;
            default:
                break;
        }
    }

    //================================================
    // Member Function: SetTargetImage
    //================================================
    void TargetImage::SetTargetImage(std::string image_file)
    {
        image_target_file_ = image_file;
    }

    //================================================
    // Member Function: SetTemplateImage
    //================================================
    void TargetImage::SetTemplateImage(std::string image_file)
    {
        image_template_file_ = image_file;
    }

    //================================================
    // Member Function: ProcessTarget
    //================================================
    void TargetImage::ProcessTarget()
    {
        // Find the paper target in the image
        target_finder_strategy_->ProcessImage();
    }

} // TactNib