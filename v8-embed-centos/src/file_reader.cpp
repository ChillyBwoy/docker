#include "file_reader.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>


FileReader::FileReader(bool with_watcher) noexcept : watch_files(with_watcher) {}


std::pair<bool, std::string> FileReader::load(const std::string& path) const noexcept {
    struct stat file_stat{};

    // check file exists
    if(stat(path.c_str(), &file_stat) != 0) {
        return std::make_pair(false, "");
    }

    // check file was loaded || modified since last load
    if(file_versions.find(path) == file_versions.end() || (watch_files && file_versions[path] != file_stat.st_mtime)) {
        std::ifstream file_stream(path.c_str());
        std::string str;

        file_stream.seekg(0, std::ios::end);
        str.reserve(static_cast<unsigned long>(file_stream.tellg()));
        file_stream.seekg(0, std::ios::beg);

        str.assign(
                std::istreambuf_iterator<char>(file_stream),
                std::istreambuf_iterator<char>()
        );
        file_stream.close();
        file_versions[path] = file_stat.st_mtime;

        return std::make_pair(true, str);
    }

    return std::make_pair(true, "");
}


std::vector<std::string> FileReader::loaded() const noexcept {
    std::vector<std::string> file_names;
    file_names.reserve(file_versions.size());

    std::transform(
            std::begin(file_versions),
            std::end(file_versions),
            std::back_inserter(file_names),
            [](const auto& pair) { return pair.first; }
    );

    return file_names;
}
