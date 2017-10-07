#include "FolderScanner.h"
#include <boost/filesystem.hpp>

void FolderScanner::getPaths(const std::string &folder, const std::vector<std::string> &extensions, std::vector<std::string> &filepaths) {
    filepaths.clear();

    boost::filesystem::directory_iterator iter((folder + "/").c_str());
    for (; iter != boost::filesystem::directory_iterator(); iter++) {
        if (!boost::filesystem::is_regular_file(iter->status())) {
            continue;
        }
        boost::filesystem::path p = iter->path();
        std::string fileExtension = p.extension().string();

        std::vector<std::string>::const_iterator extIter = std::find_if(extensions.begin(), extensions.end(),
        [&fileExtension](const std::string & allowedExtension) {
            return allowedExtension == fileExtension;
        });

        if (extIter != extensions.end()) {
            filepaths.push_back(p.relative_path().string());
        }
    }
}

void FolderScanner::getImagePaths(const std::string &folder, std::vector<std::string> &filepaths) {
    static std::vector<std::string> extensions {".jpg", ".png", ".bmp", ".jpeg"};
    getPaths(folder, extensions, filepaths);
}
