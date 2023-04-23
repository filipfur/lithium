#include "glfilewatch.h"

namespace
{
    static std::thread thread;
    static std::mutex mutex;
    static bool running = false;
    static int watchIndex{0};
    static std::vector<std::shared_ptr<lithium::FileWatch>> watches;
}

void watchThread()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock(mutex);
        std::shared_ptr<lithium::FileWatch> watch = watches[watchIndex];
        watch->update();
        watchIndex = watchIndex < watches.size() - 1 ? watchIndex + 1 : 0; // next index;
    }
}

void addWatch(std::shared_ptr<lithium::FileWatch> watch)
{
    std::lock_guard<std::mutex> lock(mutex);
    watches.push_back(watch);
}

lithium::FileWatch::FileWatch(fs::path path, std::function<void(const fs::path&)> callback) : _path{path}, _callback{callback}
{
    static int nextId{0};
    _id = nextId++;
    if(!fs::exists(_path))
    {
        throw std::runtime_error("FileWatch: Error: File does not exist: " + _path.string());
    }
    _lastWriteTime = fs::last_write_time(_path);
}

lithium::FileWatch::~FileWatch() noexcept
{
    std::lock_guard<std::mutex> lock(mutex);
    
    watches.erase(std::remove_if(watches.begin(), watches.end(), [this](std::shared_ptr<lithium::FileWatch> watch) { return watch->_id == _id; }), watches.end());

    if(watches.size() == 0)
    {
        running = false;
        thread.join();
    }
    if(watchIndex >= watches.size())
    {
        watchIndex = 0;
    }
}

void lithium::FileWatch::update()
{
    fs::file_time_type lastWriteTime = fs::last_write_time(_path);

    if (lastWriteTime != _lastWriteTime)
    {
        _lastWriteTime = lastWriteTime;
        _callback(_path);
    }
}

std::shared_ptr<lithium::FileWatch> lithium::FileWatch::start(fs::path path, std::function<void(const fs::path&)> callback)
{
    std::shared_ptr<lithium::FileWatch> watch = std::make_shared<lithium::FileWatch>(path, callback);
    addWatch(watch);
    if(!running)
    {
        running = true;
        thread = std::thread(watchThread);
    }
    return watch;
}