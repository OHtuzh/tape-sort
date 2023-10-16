#pragma once

#include <cinttypes>

namespace ohtuzh {
    class ITape {
    public:
        enum Mode {
            kRead,
            kWrite
        };

        virtual bool Next() = 0;

        virtual bool Prev() = 0;

        virtual void Write(int32_t) = 0;

        virtual int32_t Read() = 0;

        virtual void RewindToStart() = 0;

        virtual void RewindToEnd() = 0;

        virtual void SwitchMode(Mode mode) = 0;

        virtual ~ITape() = default;
    };

} // namespace ohtuzh
