/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: target_scene_image.h
 * Purpose:	  Class for image of scene with paper target. The purpose of this class is
 *            to find the paper target within the scene image and score the target.
 *            This class will provide the ability to preprocess the image
 *            (adjust colors), select finder algorithm, find and align image, and
 *            score the target.
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
 *
 * ============================================================================*/
#ifndef SYSTEM_API_TARGET_SCENE_IMAGE_H
#define SYSTEM_API_TARGET_SCENE_IMAGE_H

#include <string>
#include "target_finder_strategy.h"

namespace TactNib {

    enum class TargetFinderStrategyType
    {
        ObjectDetect, OpenCvRect
    };

    class TargetSceneImage {
        public:
            // Initialize functions
            TargetSceneImage();

            void SetTargetFinderStrategy(TargetFinderStrategyType);

            // Class Support functions
            void SetSceneImage(std::string);
            void SetTargetImage(std::string);

            // Image processing function
            void ProcessScene();

        private:
            TargetFinderStrategy *target_finder_strategy_;

            // std::filesystem is not available on the embedded board, use std::string in the meantime
            std::string image_scene_file_;
            std::string image_target_file_;

        protected:
    }; // TargetSceneImage

} // TactNib

#endif //SYSTEM_API_TARGET_SCENE_IMAGE_H
