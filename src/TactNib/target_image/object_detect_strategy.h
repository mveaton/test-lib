/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: object_detect_strategy.h
 * Purpose:	  Concrete class for image finder algorithm. The purpose of this class is
 *            to implement the strategy design pattern to allow various algorithms
 *            to be used at runtime. This class will implement the "FindTarget" function
 *            that will use an object detection model to find the paper target.
 * Author:	  Michael Eaton
 *
 * ============================================================================*/

#ifndef SYSTEM_API_OBJECT_DETECT_STRATEGY_H
#define SYSTEM_API_OBJECT_DETECT_STRATEGY_H

#include <string>
#include "target_finder_strategy.h"

namespace TactNib {

    class ObjectDetectStrategy: public TargetFinderStrategy {
        public:
            ObjectDetectStrategy(std::string, std::string);

        private:
            void FindTarget() override;

        protected:
    };

} // TactNib

#endif //SYSTEM_API_OBJECT_DETECT_STRATEGY_H
