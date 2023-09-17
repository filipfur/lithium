#pragma once

#include <functional>

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

namespace lithium
{
namespace fs
{
    void forEachInDirectory(const std::filesystem::path& directory,
        std::function<void(const std::filesystem::path&)> callback,
        bool recursive=false);
}
}