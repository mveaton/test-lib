/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: main.cc
 * Purpose:	  Test function for system API
 * Author:	  Michael Eaton
 *
 * All Rights Reserved.
 * NOTICE:  All information contained herein is, and remains the property of
 * TactNib, LCC and its suppliers, if any.  The intellectual and technical
 * concepts contained herein are proprietary to TactNib, LCC and its suppliers
 * and may be covered by U.S. and Foreign Patents, patents in process, and are
 * protected by trade secret or copyright law. Dissemination of this information
 * or reproduction of this material is strictly forbidden unless prior written
 * permission is obtained from TactNib, LCC.
 * ============================================================================*/

#include <iostream>
#include "TactNib/target_image//target_image.h"

int main() {

    std::cout << "System-API: Main!" << std::endl;

    TactNib::TargetImage image;

    // Set up image for processing
    image.SetTargetImage("../data/target_bullet_hole.jpg");
    image.SetTemplateImage("../data/target_template_image.jpeg");
    image.SetTargetFinderStrategy(TactNib::TargetFinderStrategyType::OpenCvRect);

    // Process image for find the desired paper target
    image.ProcessTarget();

    std::cout << "End of Main" << std::endl;
    return 0;
}
