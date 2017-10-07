#include "FolderScanner.h"
#include <opencv2/opencv.hpp>

inline bool isGreaterWithBounds(int color1, int color2){
    const int lowerThr = 50;
    const int upperThr = 190;
    return (color1 > color2 + lowerThr) && (color1 < color2 + upperThr);
}

inline int getSignificantDimension(int b, int g, int r){
    if (isGreaterWithBounds(b, g) && isGreaterWithBounds(b, r)){
        return 0;
    }
    if (isGreaterWithBounds(r, g) && isGreaterWithBounds(r, b)){
        return 2;
    }
    return -1;
}

cv::Mat findBlueRed(const cv::Mat &frame){
    cv::Mat result = frame.clone();
    for (int y = 0; y < frame.rows; y++){
        const uchar *inx = &frame.at<uchar>(y, 0);
        uchar *outx = &result.at<uchar>(y, 0);
        const int endpos = 3 * frame.cols;

        for (int pos = 0; pos < endpos; pos += 3){
            int b = inx[pos];
            int g = inx[pos + 1];
            int r = inx[pos + 2];
            int dimension = getSignificantDimension(b, g, r);

            if (dimension < 0){
        	outx[0] = 0;
                outx[1] = 0;
                outx[2] = 0;
            }

            outx += 3;
        }
    }
    return result;
}


cv::Mat increaseBrightness(const cv::Mat frame){
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, CV_BGR2HSV);
    for (int y = 0; y < hsv.rows; y++){
        for (int x = 0; x < hsv.cols; x++){
            if (hsv.at<uchar>(y, 3*x + 1) > 60){
                int value = hsv.at<uchar>(y, 3*x + 2);
                value = (value + 10)*2.5;
                value = std::min(value, (int)255);
                hsv.at<uchar>(y, 3*x + 2) = value;
            }
        }
    }
    cv::Mat result;
    cv::cvtColor(hsv, result, CV_HSV2BGR);
    return result;
}

cv::Mat filterBigComponents(const cv::Mat &br){
    const int upperSizeThr = 40;
    const int lowerSizeThr = 3;
    cv::Mat mask = cv::Mat::zeros(br.size(), CV_8UC1);
    for (int y = 0; y < br.rows; y++){
        for (int x = 0; x < br.cols; x++){
            const uchar *p = &br.at<uchar>(y, 3*x);
            if ((int)p[0] + (int)p[1] + (int)p[2] > 0){
                mask.at<uchar>(y, x) = 2;
            }
        }
    }
    for (int y = 0; y < mask.rows; y++){
        for (int x = 0; x < mask.cols; x++){
            if (mask.at<uchar>(y, x) != 2){
                continue;
            }
            cv::Rect roi;
            cv::floodFill(mask, cv::Point2i(x, y), cv::Scalar::all(1), &roi);
            if (roi.width > upperSizeThr || roi.height > upperSizeThr || roi.width < lowerSizeThr|| roi.height < lowerSizeThr){
                cv::floodFill(mask, cv::Point2i(x, y), cv::Scalar::all(0));
            }
        }
    }
	
    cv::Mat result;
    br.copyTo(result, mask);
   
    return result;
}

int main(int argc, char **argv) {
    std::string imagesIn = "/media/mes/f9289561-58c2-46ea-9c7e-42c8e63fb3da/rtsd-d1-frames/train/";
    
    std::vector<std::string> imgpaths;
    FolderScanner::getImagePaths(imagesIn, imgpaths);

    int count = 0;
    for (const std::string &path : imgpaths){
	cv::Mat frame = cv::imread("/" + path, CV_LOAD_IMAGE_COLOR);
        std::cout << path << std::endl;
        
        cv::Mat blurred;
        cv::blur(frame, blurred, cv::Size(3,3));
        
        cv::Mat bright = increaseBrightness(blurred);

        cv::Mat br = findBlueRed(bright);

        cv::Mat filtered = filterBigComponents(br);

        cv::Mat vis;
        cv::vconcat(frame, bright, vis);
        cv::vconcat(vis, br, vis);
        cv::vconcat(vis, filtered, vis);
	cv::imwrite("output/" + std::to_string(count++)+".png", vis);
    }

    return 0;
}
