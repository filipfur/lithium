
#pragma once

#include <ostream>
#include <fstream>

class DebugOstream
{
public:
    DebugOstream(std::ostream *os) : _os{os}
    {
    }

    void setDebugFile(const std::string& fileName)
    {
        if(_ofs)
        {
            delete _ofs;
        }
        _ofs = new std::ofstream(fileName);
    }

    virtual ~DebugOstream() noexcept
    {
        _os = nullptr;
        if(_ofs)
        {
            _ofs->flush();
            _ofs->close();
            delete _ofs;
            _ofs = nullptr;
        }
    }

    template <class T>
    DebugOstream &operator<<(T &&x)
    {
        if (_debug)
        {
            *_os << std::forward<T>(x);
        }
        if (_fileDebug && _ofs)
        {
            *_ofs << std::forward<T>(x);
        }
        return *this;
    }

    DebugOstream &operator<<(std::ostream &(*manip)(std::ostream &))
    {
        if (_debug)
        {
            *_os << manip;
        }
        if (_fileDebug && _ofs)
        {
            *_ofs << manip;
        }
        return *this;
    }

private:
    std::ostream *_os{nullptr};
    bool _debug{false};
    std::ofstream *_ofs{nullptr};
    bool _fileDebug{true};
};

extern DebugOstream dout;