#pragma once

namespace mygl
{
    class SoundEngine
    {
    public:
        SoundEngine();
        virtual ~SoundEngine() noexcept;

        static SoundEngine& getInstance();

        //static void _playSound(const char* path);

        SoundEngine(SoundEngine const&) = delete;
        void operator=(SoundEngine const&) = delete;

    private:
    };
}