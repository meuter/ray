#include <ray/platform/FileSystem.hpp>
#include <boost/filesystem.hpp>

namespace boostfs = boost::filesystem;

namespace ray { namespace platform { namespace fs {

    bool exists(const std::string &path)
    {
        auto status = boostfs::status(path);
        return (status.type() != boostfs::file_not_found);
    }
  
    std::string extension(const std::string &path)
    {
        return boostfs::extension(path);
    }

    std::string parent(const std::string &path)
    {
        return boostfs::path(path).parent_path().string() + "/";
    }

    std::string filename(const std::string &path)
    {
        return boostfs::path(path).filename().string();
    }

    std::string join(const std::string &pathPrefix, const std::string pathSuffix)
    {
        return (boostfs::path(pathPrefix) / boostfs::path(pathSuffix)).string();
    }

    bool isAbsolute(const std::string &path)
    {
        return boostfs::path(path).is_absolute();
    }

}}}