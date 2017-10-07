#include "FolderScanner.h"
#include <opencv2/opencv.hpp>

inline bool isGreaterWithBounds(int color1, int color2){
    const int lowerThr = 40;
    const int upperThr = 130;
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

int main(int argc, char **argv) {
    std::string imagesIn = "/media/mes/f9289561-58c2-46ea-9c7e-42c8e63fb3da/rtsd-d1-frames/train/";
    
    std::vector<std::string> imgpaths;
    FolderScanner::getImagePaths(imagesIn, imgpaths);

    int count = 0;
    for (const std::string &path : imgpaths){
	cv::Mat frame = cv::imread("/" + path, CV_LOAD_IMAGE_COLOR);
        std::cout << path << std::endl;
        cv::Mat blurred;
        cv::blur(frame, blurred, cv::Size(5,5));
        cv::Mat br = findBlueRed(blurred);
        cv::imshow("frame", br);
        cv::waitKey(100);

        cv::Mat vis;
        cv::vconcat(frame, br, vis);
	cv::imwrite("output/" + std::to_string(count++)+".png", vis);
    }

    return 0;
}
