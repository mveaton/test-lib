/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: opencv_strategy.h
 * Purpose:	  Concrete class for image finder algorithm. The purpose of this class is
 *            to implement the strategy design pattern to allow various algorithms
 *            to be used at runtime. This class will implement the "FindTarget" function
 *            that will use OpenCV to find the paper target.
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
 *
 * ============================================================================*/

#ifndef SYSTEM_API_OPENCV_STRATEGY_H
#define SYSTEM_API_OPENCV_STRATEGY_H

#include <string>
#include "target_finder_strategy.h"

namespace TactNib {

    // Enumerations used for the feature detection algorithm
    enum class FeatureDetectorType
    {
        Detect_FAST, Detect_SIFT, Detect_ORB
    };

    enum class FeatureExtractType
    {
        Extract_FAST, Extract_SIFT, Extract_ORB
    };

    enum class MatchType
    {
        Match_FLANN, Match_BRUTEFORCE
    };

    enum class FilterType
    {
        Filter_LOWES, Filter_SCORE
    };

    class OpenCvStrategy: public TargetFinderStrategy
    {
        public:
            OpenCvStrategy(std::string, std::string);

        private:
        TargetObjectImage FindTarget() override;

        protected:
    };

} // TactNib

#endif //SYSTEM_API_OPENCV_STRATEGY_H
