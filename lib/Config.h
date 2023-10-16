#include <charconv>
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace ohtuzh {

using std::chrono_literals::operator""ms;

const std::chrono::milliseconds kBasicDelay = 0ms;

class Config {
   public:
    Config() = default;

    Config(const std::string& file);

    std::chrono::milliseconds GetReadDelay() const noexcept;

    std::chrono::milliseconds GetWriteDelay() const noexcept;

    std::chrono::milliseconds GetRewindDelay() const noexcept;

    std::chrono::milliseconds GetShiftDelay() const noexcept;

   private:
    std::chrono::milliseconds ParseMilliseconds(std::string_view sv) const;

    std::chrono::milliseconds read_delay_{kBasicDelay};
    std::chrono::milliseconds write_delay_{kBasicDelay};
    std::chrono::milliseconds rewind_delay_{kBasicDelay};
    std::chrono::milliseconds shift_delay_{kBasicDelay};
};

}  // namespace ohtuzh
