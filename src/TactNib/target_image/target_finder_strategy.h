/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: target_finder_strategy.h
 * Purpose:	  Abstract class for image finder algorithm. The purpose of this class is
 *            to implement the strategy design pattern to allow various algorithms
 *            to be used at runtime.
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
 *
 * ============================================================================*/

#ifndef SYSTEM_API_TARGET_FINDER_STRATEGY_H
#define SYSTEM_API_TARGET_FINDER_STRATEGY_H

#include <string>
#include <vector>
#include <iostream>
#include "target_object_image.h"

namespace TactNib {

    class TargetFinderStrategy {
        public:
            TargetFinderStrategy(std::string, std::string);
            virtual ~TargetFinderStrategy() {
                std::cout << "Base destructor called." << std::endl;
            }
            void ProcessImage();

        protected:
            std::string image_scene_file_;
            std::string image_target_file_;

        private:
            virtual TargetObjectImage FindTarget() = 0;

    };

} // TactNib

#endif //SYSTEM_API_TARGET_FINDER_STRATEGY_H
