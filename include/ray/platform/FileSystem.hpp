#pragma once

#include <string>

namespace ray { namespace platform { namespace fs {

    bool        exists(const std::string &path);    
    bool        isAbsolute(const std::string &path);
    std::string extension(const std::string &path);
    std::string filename(const std::string &path);
    std::string parent(const std::string &path);
    std::string join(const std::string &pathPrefix, const std::string pathSuffix);

}}}