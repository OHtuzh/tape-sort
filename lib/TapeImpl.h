#pragma once

#include <cctype>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <string>

#include "ITape.h"
#include "Config.h"

namespace ohtuzh {

class TapeImpl : public ITape {
   public:
    TapeImpl(std::string filename, Mode mode, Config config = Config());

    bool Next() override;

    bool Prev() override;

    int32_t Read() override;

    void Write(int32_t val) override;

    void RewindToStart() override;

    void RewindToEnd() override;

    void SwitchMode(Mode mode) override;

    ~TapeImpl() override = default;

   private:
    void Back();

    std::fstream file_;
    std::string filename_;
    int32_t current_;
    int64_t index_ = -1;
    Mode mode_;
    Config config_;
};

}  // namespace ohtuzh
