/** ===========================================================================
 *
 * File Name: opencv_algo.cc
 * Purpose:	  Placeholder for algorithms utilizing OpenCV found on the internet
 * Author:	  Michael Eaton
 *
 * Coding Standard: https://google.github.io/styleguide/cppguide.html
 *
 * ============================================================================*/

#include "opencv_algo.h"

using namespace cv;

namespace TactNib {

    namespace OpencvAlgo {
        //================================================
        // Member Function: GetMSSIM
        //  Note: Found this on the internet
        // https://docs.opencv.org/3.4/d5/dc4/tutorial_video_input_psnr_ssim.html
        //================================================
        Scalar GetMSSIM(const Mat &i1, const Mat &i2) {

            const double C1 = 6.5025, C2 = 58.5225;
            /***************************** INITS **********************************/
            int d = CV_32F;
            Mat I1, I2;
            i1.convertTo(I1, d);
            i2.convertTo(I2, d);
            Mat I2_2 = I2.mul(I2);        // I2^2
            Mat I1_2 = I1.mul(I1);        // I1^2
            Mat I1_I2 = I1.mul(I2);        // I1 * I2
            /*************************** END INITS **********************************/
            Mat mu1, mu2;                   // PRELIMINARY COMPUTING
            GaussianBlur(I1, mu1, Size(11, 11), 1.5);
            GaussianBlur(I2, mu2, Size(11, 11), 1.5);
            Mat mu1_2 = mu1.mul(mu1);
            Mat mu2_2 = mu2.mul(mu2);
            Mat mu1_mu2 = mu1.mul(mu2);
            Mat sigma1_2, sigma2_2, sigma12;
            GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
            sigma1_2 -= mu1_2;
            GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
            sigma2_2 -= mu2_2;
            GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
            sigma12 -= mu1_mu2;

            Mat t1 = 2 * mu1_mu2 + C1;
            Mat t2 = 2 * sigma12 + C2;
            Mat t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
            t1 = mu1_2 + mu2_2 + C1;
            t2 = sigma1_2 + sigma2_2 + C2;
            t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))
            Mat ssim_map;
            divide(t3, t1, ssim_map);   // ssim_map =  t3./t1;
            Scalar mssim = mean(ssim_map);        // mssim = average of ssim map
            return mssim;
        }

        //================================================
        // Member Function: AlignImageMotion
        //  Note: Found this on the internet
        // https://learnopencv.com/image-alignment-ecc-in-opencv-c-python/
        //================================================
        Mat AlignImageMotion(const Mat &im1, const Mat &im2) {

            // Convert images to gray scale;
            Mat im1_gray, im2_gray;
            cvtColor(im1, im1_gray, COLOR_BGR2GRAY);
            cvtColor(im2, im2_gray, COLOR_BGR2GRAY);

            // Define the motion model
            const int warp_mode = MOTION_HOMOGRAPHY;

            // Set a 2x3 or 3x3 warp matrix depending on the motion model.
            Mat warp_matrix;

            // Initialize the matrix to identity
            if (warp_mode == MOTION_HOMOGRAPHY)
                warp_matrix = Mat::eye(3, 3, CV_32F);
            else
                warp_matrix = Mat::eye(2, 3, CV_32F);

            // Specify the number of iterations.
            int number_of_iterations = 5000;

            // Specify the threshold of the increment
            // in the correlation coefficient between two iterations
            double termination_eps = 1e-10;

            // Define termination criteria
            TermCriteria criteria(TermCriteria::COUNT + TermCriteria::EPS, number_of_iterations, termination_eps);

            // Run the ECC algorithm. The results are stored in warp_matrix.
            findTransformECC(
                    im1_gray,
                    im2_gray,
                    warp_matrix,
                    warp_mode,
                    criteria
            );

            // Storage for warped image.
            Mat im2_aligned;

            if (warp_mode != MOTION_HOMOGRAPHY)
                // Use warpAffine for Translation, Euclidean and Affine
                warpAffine(im2, im2_aligned, warp_matrix, im1.size(), INTER_LINEAR + WARP_INVERSE_MAP);
            else
                // Use warpPerspective for Homography
                warpPerspective(im2, im2_aligned, warp_matrix, im1.size(), INTER_LINEAR + WARP_INVERSE_MAP);

            return im2_aligned;
        }

        //================================================
        // Member Function: FindBlob
        //================================================
        Mat FindBlob(const cv::Mat &source) {

            // Initialize parameter setting using cv2.SimpleBlobDetector
            cv::SimpleBlobDetector::Params params;

            // Set inertia filtering parameters
            params.filterByInertia = true;
            params.minInertiaRatio = 0.01;

            // Set Convexity filtering parameters
            params.filterByConvexity = true;
            params.minConvexity = 0.2;

            params.filterByColor = false;

            // Set Area filtering parameters
            params.filterByArea = true;
            params.minArea = 200.0f;
            params.maxArea = 350.0f;

            // Set Circularity filtering parameters
            params.filterByCircularity = false;
            params.minCircularity = 0.2;

            cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
            std::vector<KeyPoint> keypoints;
            detector->detect(source, keypoints);

            Mat source_keypoint;
            drawKeypoints(source, keypoints, source_keypoint, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

            return source_keypoint;
        }

        //================================================
        // Member Function: AdjustBrightness
        //================================================
        Mat AdjustBrightness(const cv::Mat &source, const cv::Mat &target) {

            // Convert to HSV image
            Mat source_hsv, target_hsv;
            cvtColor(source, source_hsv, COLOR_BGR2HSV);
            cvtColor(target, target_hsv, COLOR_BGR2HSV);

            // compute color statistics for the source and target images
            Image_Stat stat_source = GetStat(source_hsv);
            Image_Stat stat_target = GetStat(target_hsv);

            // subtract the means from the target image
            target_hsv -= Scalar(0, stat_target.mean->at<float>(1), 0); // less red.

            // scale by the standard deviations
            float scale_ratio = stat_target.stddev->at<float>(1) / stat_source.stddev->at<float>(1);
            target_hsv = target_hsv.mul(cv::Scalar(1, scale_ratio, 1));

            // add in the source mean
            target_hsv += Scalar(0, stat_source.mean->at<float>(1), 0);

            // clip the pixel intensities to [0, 255] if they fall outside
            target_hsv.setTo(255, target > 255);

            // Convert back to BGR image file
            Mat  target_bgr;
            cvtColor(target_hsv, target_bgr, COLOR_HSV2BGR);

            return target_bgr;
        }

        //================================================
        // Member Function: GetStat
        //================================================
        Image_Stat GetStat(const Mat &src) {

            // Split the image into 3 channels (RGB)
            Mat channels[3];
            split(src, channels);

            // Get the mean and standard deviation of each channel
            Image_Stat stat;
            meanStdDev(channels[0], stat.mean[0], stat.stddev[0]);
            meanStdDev(channels[1], stat.mean[1], stat.stddev[1]);
            meanStdDev(channels[2], stat.mean[2], stat.stddev[2]);

            // return the color statistics
            return stat;
        }
    }
} // TactNib