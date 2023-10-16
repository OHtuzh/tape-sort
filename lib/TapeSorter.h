#pragma once

#include <memory>
#include <filesystem>
#include <iostream>

#include "ITape.h"
#include "TapeImpl.h"

namespace ohtuzh {

    class TapeSorter {
    public:
        TapeSorter(std::shared_ptr<ITape> reader, std::shared_ptr<ITape> writer);

        void Sort(const size_t memory_limit);

    private:
        std::shared_ptr<ITape> reader_;
        std::shared_ptr<ITape> writer_;
        Config config_;
    };

} // namespace ohtuzh
