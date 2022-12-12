/** ===========================================================================
 * Copyright (c) 2022 TactNib, LCC
 *
 * File Name: opencv_strategy.cc
 * Purpose:	  Concrete class for image finder algorithm. The purpose of this class is
 *            to implement the strategy design pattern to allow various algorithms
 *            to be used at runtime. This class will implement the "FindTarget" function
 *            that will use OpenCV to find the paper target.
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
 *
 * ============================================================================*/

#include <chrono>
#include "opencv_algo.h"
#include "opencv_strategy.h"
#include "target_object_image.h"
#include "enum_support.h"

using namespace std::chrono;

namespace TactNib {


    //================================================
    // Default constructor
    //================================================
    OpenCvStrategy::OpenCvStrategy(std::string image_scene_file, std::string image_object_file): TargetFinderStrategy(image_scene_file, image_object_file){}


    //================================================
    // Member Function: FindTarget
    //================================================
    TargetObjectImage OpenCvStrategy::FindTarget() {

        // Binary-string descriptors: ORB, BRIEF, BRISK, FREAK, AKAZE, etc. (FLANN + LSH index) or (Brute Force + Hamming distance).
        // Floating-point descriptors: SIFT, SURF, GLOH, etc.
        // Feature-Based Image Alignment
        // cv::Ptr<cv::DescriptorMatcher> matcher = cv::makePtr<cv::FlannBasedMatcher>(cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2));
        // Working Combinations: Object (3600 x 5400) & Scene (2543 x 3233)
        //     1) Detect_SIFT, Extract_SIFT, Match_FLANN,      Filter_LOWES # Score: 68.5  Time 9.5s
        //     2) Detect_FAST, Extract_ORB,  Match_FLANN,      Filter_SCORE # Score: 59.3  Time 6.8s
        //     3) Detect_ORB,  Extract_ORB,  Match_BRUTEFORCE, Filter_SCORE # Score: 63.3  Time 4.8s
        //     4) Detect_ORB,  Extract_ORB,  Match_FLANN,      Filter_SCORE # Score: 63.3  Time 3.7s


        // Move this configuration logic into a config file
        FeatureDetectorType detector_type = TactNib::FeatureDetectorType::Detect_SIFT;
        FeatureExtractType extract_type = TactNib::FeatureExtractType::Extract_SIFT;
        MatchType match_type = TactNib::MatchType::Match_FLANN;
        FilterType filter_type = TactNib::FilterType::Filter_LOWES;

        // Convert jpg file to opencv matrix
        Mat image_scene = imread(image_scene_file_, IMREAD_COLOR);
        Mat image_object = imread(image_target_file_, IMREAD_COLOR);

        // Adjust the brightness of scene image to match the object image
        image_scene = OpencvAlgo::AdjustBrightness(image_object, image_scene);

        // Get image template size
        cv::Size sz = image_object.size();
        int template_height = sz.height;
        int template_width = sz.width;
        std::cout << "Object Image: width - " << template_width << " height - " << template_height << std::endl;
        sz = image_scene.size();
        int scene_height = sz.height;
        int scene_width = sz.width;
        std::cout << "Scene Image: width - " << scene_width << " height - " << scene_height << std::endl;

        // Scale scene image to match size of template; for comparison purposes
        Mat scale_image;
        double scale = static_cast< double > (template_height) / scene_height;
        resize(image_scene, scale_image, Size(), scale, scale, INTER_LINEAR);
        image_scene = scale_image.clone();

        // Variables to store keypoints and descriptors
        std::vector<KeyPoint> keypoints_scene, keypoints_object;
        Mat descriptors_scene, descriptors_object;
        std::vector<Point2f> points_scene, points_object;
        std::vector<DMatch> matches;
        std::vector<std::vector<DMatch> > knn_matches;

        // Step 1: Detect features in scene and object image
        auto start_time = time_point_cast<milliseconds>(system_clock::now());
        auto start_time_summary = time_point_cast<milliseconds>(system_clock::now());
        std::cout << "Step 1: Detect features in image: " << detector_type << std::endl;
        const int MAX_FEATURES = 500;
        switch (detector_type) {
            case FeatureDetectorType::Detect_FAST: {
                Ptr<FastFeatureDetector> detector_ff = FastFeatureDetector::create();
                detector_ff->detect(image_scene, keypoints_scene);
                detector_ff->detect(image_object, keypoints_object);
                break;
            }
            case FeatureDetectorType::Detect_SIFT: {
                Ptr<cv::SIFT> detector_sift = SIFT::create();
                detector_sift->detect(image_scene, keypoints_scene);
                detector_sift->detect(image_object, keypoints_object);
                break;
            }
            case FeatureDetectorType::Detect_ORB: {
                Ptr<Feature2D> detector_orb = ORB::create(MAX_FEATURES);
                detector_orb->detect(image_scene, keypoints_scene);
                detector_orb->detect(image_object, keypoints_object);
                break;
            }
            default: {
                Ptr<cv::SIFT> detector_sift2 = SIFT::create();
                detector_sift2->detect(image_scene, keypoints_scene);
                detector_sift2->detect(image_object, keypoints_object);
                break;
            }
        }
        auto end_time = time_point_cast<milliseconds>(system_clock::now());
        std::cout << "Step 1: Compute time is  " << (end_time - start_time).count() / 1000.0 << " s" << std::endl;

        // Step 2: Extract features in images
        std::cout << "Step 2: Extract features in image: " << extract_type << std::endl;
        start_time = time_point_cast<milliseconds>(system_clock::now());
        switch (extract_type) {
            case FeatureExtractType::Extract_FAST: {
                Ptr<FastFeatureDetector> detector_ff = FastFeatureDetector::create();
                detector_ff->compute(image_scene, keypoints_scene, descriptors_scene);
                detector_ff->compute(image_object, keypoints_object, descriptors_object);
                break;
            }
            case FeatureExtractType::Extract_SIFT: {
                Ptr<SIFT> detector_sift = SIFT::create();
                detector_sift->compute(image_scene, keypoints_scene, descriptors_scene);
                detector_sift->compute(image_object, keypoints_object, descriptors_object);
                break;
            }
            case FeatureExtractType::Extract_ORB: {
                Ptr<Feature2D> detector_orb = ORB::create(MAX_FEATURES);
                detector_orb->compute(image_scene, keypoints_scene, descriptors_scene);
                detector_orb->compute(image_object, keypoints_object, descriptors_object);
                break;
            }
            default: {
                Ptr<cv::SIFT> detector_sift2 = SIFT::create();
                detector_sift2->compute(image_scene, keypoints_scene, descriptors_scene);
                detector_sift2->compute(image_object, keypoints_object, descriptors_object);
                break;
            }
        }
        end_time = time_point_cast<milliseconds>(system_clock::now());
        std::cout << "Step 2: Compute time is  " << (end_time - start_time).count() / 1000.0 << " s" << std::endl;

        // Step 3: Match descriptors together
        std::cout << "Step 3: Match descriptors together: " << match_type << std::endl;
        start_time = time_point_cast<milliseconds>(system_clock::now());
        switch (match_type) {
            case MatchType::Match_BRUTEFORCE: {
                Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
                if (extract_type == FeatureExtractType::Extract_SIFT) {
                    matcher->knnMatch(descriptors_object, descriptors_scene, knn_matches, 2);
                } else {
                    matcher->match(descriptors_object, descriptors_scene, matches, Mat());
                }
                break;
            }
            case MatchType::Match_FLANN: {
                if (extract_type == FeatureExtractType::Extract_SIFT) {
                    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
                    // matcher->knnMatch(descriptors_scene, descriptors_object, knn_matches, 2);
                    matcher->knnMatch(descriptors_object, descriptors_scene, knn_matches, 2);
                } else {
                    Ptr<DescriptorMatcher> matcher = cv::makePtr<cv::FlannBasedMatcher>(
                            cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2));
                    matcher->match(descriptors_object, descriptors_scene, matches, Mat());
                }

                break;
            }
            default: {
                Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
                matcher->match(descriptors_object, descriptors_scene, matches, Mat());
                break;
            }
        }
        end_time = time_point_cast<milliseconds>(system_clock::now());
        std::cout << "Step 3: Compute time is  " << (end_time - start_time).count() / 1000.0 << " s" << std::endl;

        // Step 4: Filter descriptors to improve results
        std::cout << "Step 4: Filter descriptors to improve results: " << filter_type << std::endl;
        start_time = time_point_cast<milliseconds>(system_clock::now());
        Mat image_matches;
        switch (filter_type) {
            case FilterType::Filter_SCORE: {
                if (extract_type == FeatureExtractType::Extract_SIFT) {
                    std::cout << "NOT SUPPORTED: FILTER type SCORE does not support SIFT " << matches.size()
                              << std::endl;
                } else {
                    const float GOOD_MATCH_PERCENT = 0.15f;
                    // Sort matches by score
                    std::cout << "Step 4a: Filter the matches: size = " << matches.size() << std::endl;
                    std::sort(matches.begin(), matches.end());

                    // Remove not so good matches
                    const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
                    matches.erase(matches.begin() + numGoodMatches, matches.end());

                    // Draw top matches
                    drawMatches(image_object, keypoints_object, image_scene, keypoints_scene, matches, image_matches);
                    imwrite("matches.jpg", image_matches);
                    break;
                }
            }
            case FilterType::Filter_LOWES: {
                if (extract_type == FeatureExtractType::Extract_SIFT) {

                    //-- Filter matches using the Lowe's ratio test
                    std::cout << "Step 4a: Filter matches using Lowe's ratio test: size - " << knn_matches.size()
                              << std::endl;
                    const float ratio_thresh = 0.55f;
                    for (size_t i = 0; i < knn_matches.size(); i++) {
                        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
                            matches.push_back(knn_matches[i][0]);
                        }
                    }

                    //-- Draw matches

                    //drawMatches(image_scene, keypoints_scene, image_object, keypoints_object, matches, image_matches, Scalar::all(-1),
                    //            Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
                    drawMatches(image_object, keypoints_object, image_scene, keypoints_scene, matches, image_matches,
                                Scalar::all(-1),
                                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
                    //-- Show detected matches
                    imshow("Good Matches", image_matches);
                } else {
                    std::cout << "NOT SUPPORTED: FILTER type LOWES does not support ORB/FAST " << matches.size()
                              << std::endl;
                }
                break;
            }
            default: {
                std::cout << "NOT SUPPORTED: Unknown FILTER type " << std::endl;
                break;
            }
        }
        end_time = time_point_cast<milliseconds>(system_clock::now());
        std::cout << "Step 4: Compute time is  " << (end_time - start_time).count() / 1000.0 << " s" << std::endl;

        // Step 5: Convert matches to points array
        std::cout << "Step 5: Convert matches to points array: size = " << matches.size() << std::endl;
        start_time = time_point_cast<milliseconds>(system_clock::now());
        for (size_t i = 0; i < matches.size(); i++) {
            //    points_scene.push_back(keypoints_scene[matches[i].queryIdx].pt);
            //    points_object.push_back(keypoints_object[matches[i].trainIdx].pt);

            points_scene.push_back(keypoints_scene[matches[i].trainIdx].pt);
            points_object.push_back(keypoints_object[matches[i].queryIdx].pt);
        }
        end_time = time_point_cast<milliseconds>(system_clock::now());
        std::cout << "Step 5: Compute time is  " << (end_time - start_time).count() / 1000.0 << " s" << std::endl;

        // Step 6: Remove matches that are not in the top/bottom location of markers
        std::cout << "Step 6: Remove matches located where the location markers do not exist" << std::endl;
        std::cout << "        points1 size: " << points_scene.size() << "; points2 size: " << points_object.size()
                  << std::endl;
        start_time = time_point_cast<milliseconds>(system_clock::now());
        const double TOP_MARGIN = 0.14; // 14 percent
        const double BOTTOM_MARGIN = 1.0 - TOP_MARGIN;
        const double LEFT_MARGIN = 0.25;
        const double RIGHT_MARGIN = 1.0 - LEFT_MARGIN;
        for (unsigned short i = 0; auto point: points_object) {
            // Filter out the center are of target and lower right hand corner (made in USA logo)
            //    - for some reason, this step only improves the alignment by 2% instead of the expected
            //      6%
            if (((point.y > (template_height * TOP_MARGIN)) && (point.y < (template_height * BOTTOM_MARGIN))) ||
                // center target
                ((point.y > (template_height * BOTTOM_MARGIN)) && (point.x > (template_width * RIGHT_MARGIN))) ||
                // lower right corner
                ((point.y < (template_height * TOP_MARGIN)) &&
                 (point.x < (template_width * LEFT_MARGIN))))     // upper right corner
            {
                points_object.erase(points_object.begin() + i);
                points_scene.erase(points_scene.begin() + i);
                i--;
            }
            i++;
        }
        end_time = time_point_cast<milliseconds>(system_clock::now());
        std::cout << "Step 6: Compute time is  " << (end_time - start_time).count() / 1000.0 << " s" << std::endl;

        // Step 7: Find homography
        std::cout << "Step 7: findHomography" << std::endl;
        start_time = time_point_cast<milliseconds>(system_clock::now());
        Mat h_scene_to_obj, h_obj_to_scene;
        h_scene_to_obj = findHomography(points_scene, points_object, RANSAC);
        h_obj_to_scene = findHomography(points_object, points_scene, RANSAC);
        end_time = time_point_cast<milliseconds>(system_clock::now());
        std::cout << "Step 7: Compute time is  " << (end_time - start_time).count() / 1000.0 << " s" << std::endl;

        // Get the corners from the object image ( the object to be "detected" )
        std::vector<Point2f> obj_corners(4);
        obj_corners[0] = Point2f(0, 0);
        obj_corners[1] = Point2f((float) image_object.cols, 0);
        obj_corners[2] = Point2f((float) image_object.cols, (float) image_object.rows);
        obj_corners[3] = Point2f(0, (float) image_object.rows);
        std::vector<Point2f> scene_corners(4);

        perspectiveTransform(obj_corners, scene_corners, h_obj_to_scene);

        // Draw lines between the corners (the mapped object in the scene - image_2 )
        line(image_matches, scene_corners[0] + Point2f((float) image_object.cols, 0),
             scene_corners[1] + Point2f((float) image_object.cols, 0), Scalar(0, 255, 0), 16);
        line(image_matches, scene_corners[1] + Point2f((float) image_object.cols, 0),
             scene_corners[2] + Point2f((float) image_object.cols, 0), Scalar(0, 255, 0), 16);
        line(image_matches, scene_corners[2] + Point2f((float) image_object.cols, 0),
             scene_corners[3] + Point2f((float) image_object.cols, 0), Scalar(0, 255, 0), 16);
        line(image_matches, scene_corners[3] + Point2f((float) image_object.cols, 0),
             scene_corners[0] + Point2f((float) image_object.cols, 0), Scalar(0, 255, 0), 16);

        //-- Show detected matches
        imshow("Good Matches & Object detection", image_matches);

        // Step 8 - Use homography to warp image
        std::cout << "Step 8: Warp" << std::endl;
        start_time = time_point_cast<milliseconds>(system_clock::now());
        Mat image_dewarp;
        warpPerspective(image_scene, image_dewarp, h_scene_to_obj, image_object.size());
        end_time = time_point_cast<milliseconds>(system_clock::now());
        std::cout << "Step 8: Compute time is  " << (end_time - start_time).count() / 1000.0 << " s" << std::endl;
        imshow("align", image_dewarp);

        // Print estimated homography
        std::cout << "Estimated homography : \n" << h_scene_to_obj << std::endl;

        // Overlay the target and aligned image
        double alpha = 0.5; // 50% transparency
        double beta = (1.0 - alpha);
        Mat image_align;
        addWeighted(image_object, alpha, image_dewarp, beta, 0.0, image_align);
        imshow("overlay", image_align);
        imwrite("align.jpg", image_dewarp);

        // Step 9: Score the alignment of scene's target to template target
        std::cout << "Step 9: Score the alignment of scene's target and template target" << std::endl;
        start_time = time_point_cast<milliseconds>(system_clock::now());
        Scalar results = OpencvAlgo::GetMSSIM(image_object, image_dewarp);
        double result = ((results.val[0] + results.val[1] + results.val[2]) / 3) * 100;
        end_time = time_point_cast<milliseconds>(system_clock::now());
        auto end_time_summary = time_point_cast<milliseconds>(system_clock::now());

        std::cout << "Step 9: Compute time is  " << (end_time - start_time).count() / 1000.0 << " s" << std::endl;
        std::cout << "Total : Summary compute time is  " << (end_time_summary - start_time_summary).count() / 1000.0
                  << " s" << std::endl;
        std::cout << "Image Similarity: " << result << std::endl;

        // TODO: Clean up return code by adding additional constructor for TargetObjectImage
        TargetObjectImage target_object;
        target_object.image_ = image_dewarp.clone();
        target_object.score_ = result;
        if (scene_corners.size() > 0){
            for (int i; i < 4; i++) {
                target_object.corner_points_[i].x = scene_corners[i].x;
                target_object.corner_points_[i].y = scene_corners[i].y;
            }
        }

        return target_object;
    }
} // TactNib