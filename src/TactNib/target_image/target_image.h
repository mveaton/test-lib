/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: target_image.h
 * Purpose:	  Class for image of a paper target. The purpose of this class is
 *            to find the paper target within the image and score the target
 *            within the image. This class will provide the ability to preprocess
 *            the image (adjust colors), select finder algorithm, find and align
 *            image, and score the target.
 * Author:	  Michael Eaton
 *
 * ============================================================================*/
#ifndef SYSTEM_API_TARGET_IMAGE_H
#define SYSTEM_API_TARGET_IMAGE_H

#include <string>
#include "target_finder_strategy.h"

namespace TactNib {

    enum class TargetFinderStrategyType
    {
        ObjectDetect, OpenCvRect
    };

    class TargetImage {
        public:
            // Initialize functions
            TargetImage();

            void SetTargetFinderStrategy(TargetFinderStrategyType);

            // Class Support functions
            void SetTargetImage(std::string);
            void SetTemplateImage(std::string);

            // Image processing function
            void ProcessTarget();

        private:
            TargetFinderStrategy *target_finder_strategy_;

            // std::filesystem is not available on the embedded board, use std::string in the meantime
            std::string image_target_file_;
            std::string image_template_file_;

        protected:
    }; // TargetImage

} // TactNib

#endif //SYSTEM_API_TARGET_IMAGE_H
