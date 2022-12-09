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
 * ============================================================================*/

#include "opencv_algo.h"
#include "opencv_strategy.h"
#include "support.h"

namespace TactNib {


    //================================================
    // Default constructor
    //================================================
    OpenCvStrategy::OpenCvStrategy(std::string image_target_file, std::string image_template_file): TargetFinderStrategy(image_target_file, image_template_file){}


    //================================================
    // Member Function: FindTarget
    //================================================
    void OpenCvStrategy::FindTarget() {

        // Binary-string descriptors: ORB, BRIEF, BRISK, FREAK, AKAZE, etc. (FLANN + LSH index) or (Brute Force + Hamming distance).
        // Floating-point descriptors: SIFT, SURF, GLOH, etc.
        // Feature-Based Image Alignment
        // cv::Ptr<cv::DescriptorMatcher> matcher = cv::makePtr<cv::FlannBasedMatcher>(cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2));

        // Move this configuration logic into a config file
        FeatureDetectorType detector_type = TactNib::FeatureDetectorType::Detect_SIFT;
        FeatureExtractType extract_type   = TactNib::FeatureExtractType::Extract_SIFT;
        MatchType match_type              = TactNib::MatchType::Match_FLANN;
        FilterType filter_type            = TactNib::FilterType::Filter_LOWES;

        double beta{};
        double alpha = 0.5;
        const int MAX_FEATURES = 500;
        OpencvAlgo algo;

        Mat h;
        Mat align_image;
        Mat im1Reg;
        Mat im1 = imread(image_target_file_, IMREAD_COLOR);
        Mat im2 = imread(image_template_file_, IMREAD_COLOR);

        // Convert images to grayscale
        Mat im1Gray, im2Gray;
        cvtColor(im1, im1Gray, cv::COLOR_BGR2GRAY);
        cvtColor(im2, im2Gray, cv::COLOR_BGR2GRAY);

        // Variables to store keypoints and descriptors
        std::vector<KeyPoint> keypoints1, keypoints2;
        Mat descriptors1, descriptors2;
        std::vector<Point2f> points1, points2;
        std::vector<DMatch> matches;
        std::vector<std::vector<DMatch> > knn_matches;

        // Step 1: Detect features in image
        std::cout << "Step 1: Detect features in image: " << detector_type << std::endl;
        switch (detector_type) {
            case FeatureDetectorType::Detect_FAST:
            {
                Ptr <FastFeatureDetector> detector_ff = FastFeatureDetector::create();
                detector_ff->detect(im1, keypoints1);
                detector_ff->detect(im2, keypoints2);
                break;
            }
            case FeatureDetectorType::Detect_SIFT: {
                Ptr <cv::SIFT> detector_sift = SIFT::create();
                detector_sift->detect(im1, keypoints1);
                detector_sift->detect(im2, keypoints2);
                break;
            }
            case FeatureDetectorType::Detect_ORB: {
                Ptr <Feature2D> detector_orb = ORB::create(MAX_FEATURES);
                detector_orb->detect(im1, keypoints1);
                detector_orb->detect(im2, keypoints2);
                break;
            }
            default: {
                Ptr <cv::SIFT> detector_sift2 = SIFT::create();
                detector_sift2->detect(im1, keypoints1);
                detector_sift2->detect(im2, keypoints2);
                break;
            }
        }

        // Step 2: Extract features in image
        std::cout << "Step 2: Extract features in image: " << extract_type << std::endl;
        switch (extract_type) {
            case FeatureExtractType::Extract_FAST:
            {
                Ptr <FastFeatureDetector> detector_ff = cv::FastFeatureDetector::create();
                detector_ff->compute(im1, keypoints1, descriptors1);
                detector_ff->compute(im2, keypoints2, descriptors2);
                break;
            }
            case FeatureExtractType::Extract_SIFT: {
                Ptr <cv::SIFT> detector_sift = SIFT::create();
                detector_sift->compute(im1, keypoints1, descriptors1);
                detector_sift->compute(im2, keypoints2, descriptors2);
                break;
            }
            case FeatureExtractType::Extract_ORB: {
                Ptr <Feature2D> detector_orb = ORB::create(MAX_FEATURES);
                detector_orb->compute(im1, keypoints1, descriptors1);
                detector_orb->compute(im2, keypoints2, descriptors2);
                break;
            }
            default: {
                Ptr <cv::SIFT> detector_sift2 = SIFT::create();
                detector_sift2->compute(im1, keypoints1, descriptors1);
                detector_sift2->compute(im2, keypoints2, descriptors2);
                break;
            }
        }

        // Step 3: Match descriptors together
        std::cout << "Step 3: Match descriptors together: " << match_type << std::endl;
        switch (match_type) {
            case MatchType::Match_BRUTEFORCE: {
                Ptr <DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
                if (extract_type == FeatureExtractType::Extract_SIFT) {
                    matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);
                } else {
                    matcher->match(descriptors1, descriptors2, matches, Mat());
                }
                break;
            }
            case MatchType::Match_FLANN: {
                if (extract_type == FeatureExtractType::Extract_SIFT) {
                    Ptr <DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
                    matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);
                } else {
                    Ptr <DescriptorMatcher> matcher = cv::makePtr<cv::FlannBasedMatcher>(
                            cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2));
                    matcher->match(descriptors1, descriptors2, matches, Mat());
                }

                break;
            }
            default: {
                Ptr <DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
                matcher->match(descriptors1, descriptors2, matches, Mat());
                break;
            }
        }

        // Step 4: Filter descriptors to improve results
        std::cout << "Step 4: Filter descriptors to improve results: " << filter_type << std::endl;
        switch (filter_type) {
            case FilterType::Filter_SCORE: {
                if (extract_type == FeatureExtractType::Extract_SIFT) {
                    std::cout << "NOT SUPPORTED: FILTER type SCORE does not support SIFT " << matches.size() << std::endl;
                } else {
                    const float GOOD_MATCH_PERCENT = 0.15f;
                    // Sort matches by score
                    std::cout << "Step 4a: Filter the matches: size = " << matches.size() << std::endl;
                    std::sort(matches.begin(), matches.end());

                    // Remove not so good matches
                    const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
                    matches.erase(matches.begin() + numGoodMatches, matches.end());

                    // Draw top matches
                    Mat imMatches;
                    drawMatches(im1, keypoints1, im2, keypoints2, matches, imMatches);
                    imwrite("matches.jpg", imMatches);
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
                    Mat img_matches;
                    drawMatches(im1, keypoints1, im2, keypoints2, matches, img_matches, Scalar::all(-1),
                                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
                    //-- Show detected matches
                    imshow("Good Matches", img_matches);
                } else {
                    std::cout << "NOT SUPPORTED: FILTER type LOWES does not support ORB/FAST " << matches.size()
                              << std::endl;
                }
                break;
            }
            default: {
                Ptr <DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
                matcher->match(descriptors1, descriptors2, matches, Mat());
                break;
            }
        }

        std::cout << "Step 5: Convert matches to points array: size = " << matches.size() << std::endl;
        for (size_t i = 0; i < matches.size(); i++) {
            points1.push_back(keypoints1[matches[i].queryIdx].pt);
            points2.push_back(keypoints2[matches[i].trainIdx].pt);
        }

        // Find homography
        std::cout << "Step 6: findHomography" << std::endl;
        h = findHomography(points1, points2, RANSAC);

        // Use homography to warp image
        std::cout << "Step 7: Warp" << std::endl;
        warpPerspective(im1, im1Reg, h, im2.size());

        // Print estimated homography
        std::cout << "Estimated homography : \n" << h << std::endl;
        imshow("align", im1Reg);

        // Overlay the target and aligned image
        beta = (1.0 - alpha);
        addWeighted(im2, alpha, im1Reg, beta, 0.0, align_image);
        imshow("overlay", align_image);
        imwrite("align.jpg", im1Reg);

        Scalar results;
        results = algo.GetMSSIM(im2, im1Reg);
        std::cout << "Image Similarity: " << ((results.val[0] + results.val[1] + results.val[2]) / 3) * 100 << std::endl;

        int k = waitKey(0); // Wait for a keystroke in the window
    }
} // TactNib