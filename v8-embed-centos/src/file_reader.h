#ifndef _V8_PERSISTENT_FILE_LOADER_H
#define _V8_PERSISTENT_FILE_LOADER_H

#include <string>
#include <utility>
#include <map>
#include <vector>


class FileReader {

public:

    explicit FileReader(bool with_watcher) noexcept;

    // returns Pair <file_exists, file_content>
    // file_content will be empty if file has been loaded
    std::pair<bool, std::string> load(const std::string& path) const noexcept;
    std::vector<std::string> loaded() const noexcept;

private:
    bool watch_files;
    mutable std::map<std::string, time_t> file_versions;

};

#endif //_V8_PERSISTENT_FILE_LOADER_H
