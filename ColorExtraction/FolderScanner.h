#ifndef FOLDERSCANNER_H
#define FOLDERSCANNER_H

#include <vector>
#include <string>

class FolderScanner {
public:
    static void getPaths(const std::string &folder, const std::vector<std::string> &extensions, std::vector<std::string> &filepaths);
    static void getImagePaths(const std::string &folder, std::vector<std::string> &filepaths);
};

#endif // FOLDERSCANNER_H

