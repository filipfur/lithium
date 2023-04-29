#pragma once

#ifndef __has_include
static_assert(false, "__has_include not supported");
#else
#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#elif __has_include(<boost/filesystem.hpp>)
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif
#endif

#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>

namespace lithium
{
    class FileWatch
    {
    public:
        virtual ~FileWatch() noexcept;

        void update();

        static std::shared_ptr<FileWatch> start(fs::path path, std::function<void(const fs::path&)> callback);

        FileWatch(fs::path path, std::function<void(const fs::path&)> callback);

        void stop();
    
    private:        
        FileWatch() = default;
        FileWatch(const FileWatch&) = delete;
        FileWatch& operator=(const FileWatch&) = delete;
        FileWatch(FileWatch&&) = delete;
        FileWatch& operator=(FileWatch&&) = delete;

        int _id;
        fs::path _path;
        std::function<void(const fs::path&)> _callback;
        fs::file_time_type _lastWriteTime;
        bool _stopped{false};
    };
}