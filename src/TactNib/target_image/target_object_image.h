/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: target_object_image.h
 * Purpose:	  Class for target object image.
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
 *
 * ============================================================================*/

#ifndef SYSTEM_API_TARGET_OBJECT_IMAGE_H
#define SYSTEM_API_TARGET_OBJECT_IMAGE_H

#include <opencv2/core.hpp>

namespace TactNib {

    struct CornerPoint
    {
        int x, y;
    };

    class TargetObjectImage {
        public:
            // Initialize functions
            TargetObjectImage();

            // TODO: Create set/get functions for these member variables
            cv::Mat image_;
            CornerPoint corner_points_[4];
            double score_;
        private:

        protected:

    };
}


#endif //SYSTEM_API_TARGET_OBJECT_IMAGE_H
