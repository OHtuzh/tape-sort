#include "Config.h"

#include <stdexcept>

namespace ohtuzh {
Config::Config(const std::string& filename) {
    std::ifstream fin(filename);
    std::string line;

    if (!fin) {
        throw std::runtime_error("There's no such file: " + filename);
    }

    while (std::getline(fin, line)) {
        auto equals_sign = std::find(line.begin(), line.end(), '=');
        if (equals_sign == line.end()) {
            throw std::runtime_error("Bad config line: " + line);
        }
        std::string_view param_name(line.begin(), equals_sign);
        std::string_view value(std::next(equals_sign), line.end());

        if (param_name == "read_delay") {
            read_delay_ = ParseMilliseconds(value);
        } else if (param_name == "write_delay") {
            write_delay_ = ParseMilliseconds(value);
        } else if (param_name == "rewind_delay") {
            rewind_delay_ = ParseMilliseconds(value);
        } else if (param_name == "shift_delay") {
            shift_delay_ = ParseMilliseconds(value);
        } else {
            throw std::runtime_error("Unknown parametr: " +
                                     std::string(param_name));
        }
    }
}

std::chrono::milliseconds Config::GetReadDelay() const noexcept {
    return read_delay_;
}

std::chrono::milliseconds Config::GetWriteDelay() const noexcept {
    return write_delay_;
}

std::chrono::milliseconds Config::GetRewindDelay() const noexcept {
    return rewind_delay_;
}

std::chrono::milliseconds Config::GetShiftDelay() const noexcept {
    return shift_delay_;
}

std::chrono::milliseconds Config::ParseMilliseconds(std::string_view sv) const {
    int64_t result;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), result);

    if (ec == std::errc() && ptr != sv.data() + sv.size()) {
        throw std::runtime_error("This is not a number: '" + std::string(sv) + '\'');
    }
    if (ec == std::errc::invalid_argument) {
        throw std::runtime_error("This is not a number: '" + std::string(sv) + '\'');
    }
    if (ec == std::errc::result_out_of_range) {
        throw std::runtime_error("This number is larger than an int: '" + std::string(sv) + '\'');
    }

    if (result < 0) {
        throw std::runtime_error("Negative delay: " + std::string(sv));
    }
    return std::chrono::milliseconds(result);
}
}  // namespace ohtuzh
