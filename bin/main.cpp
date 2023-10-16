#include <charconv>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>

#include "lib/TapeImpl.h"
#include "lib/TapeSorter.h"

namespace {
const char* kConfigParam = "--config=";
const char* kMemoryLimitParam = "--memory-limit=";
const char* kInputParam = "--input=";
const char* kOutputParam = "--output=";

const size_t kConfigParamLength = strlen(kConfigParam);
const size_t kMemoryLimitParamLength = strlen(kMemoryLimitParam);
const size_t kInputParamLength = strlen(kInputParam);
const size_t kOutputParamLength = strlen(kOutputParam);

struct Arguments {
    size_t memory_limit = 128 * 1024;  // 128KB
    const char* config_path = nullptr;
    const char* input = nullptr;
    const char* output = nullptr;
};

int ParseInteger(const char* str) {
    int result;
    const char* str_end = str + strlen(str);
    auto [ptr, ec] = std::from_chars(str, str_end, result);

    if (ec == std::errc()) {
        if (ptr != str_end) {
            std::cerr << "This is not a number: '" << str << '\'' << std::endl;
            exit(EXIT_FAILURE);
        }
        return result;
    }
    if (ec == std::errc::invalid_argument) {
        std::cerr << "This is not a number: '" << str << '\'' << std::endl;
        exit(EXIT_FAILURE);
    }
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "This number is larger than an int: '" << str << '\''
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    return result;
}

Arguments ParseArguments(int argc, char** argv) {
    Arguments args;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            if (strncmp(argv[i], kConfigParam, kConfigParamLength) == 0) {
                args.config_path = argv[i] + kConfigParamLength;
            } else if (strncmp(argv[i], kMemoryLimitParam, kMemoryLimitParamLength) == 0) {
                args.memory_limit = ParseInteger(argv[i] + kMemoryLimitParamLength);
            } else if (strncmp(argv[i], kInputParam, kInputParamLength) == 0) {
                args.input = argv[i] + kInputParamLength;
            } else if (strncmp(argv[i], kOutputParam, kOutputParamLength) == 0) {
                args.output = argv[i] + kOutputParamLength;
            } else {
                std::cerr << "Unknown argument '" << argv[i] << '\''
                          << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Unknown positional argument '" << argv[i] << '\''
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    if (args.input == nullptr) {
        std::cerr << "Ther's no input file" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (args.output == nullptr) {
        std::cerr << "Ther's no output file" << std::endl;
        exit(EXIT_FAILURE);
    }
    return args;
}

int Run(const Arguments& args) {
    try {
        ohtuzh::Config cfg = [&args]() {
            if (args.config_path == nullptr) {
                return ohtuzh::Config();
            }
            return ohtuzh::Config(args.config_path);
        }();

        auto input = std::make_shared<ohtuzh::TapeImpl>(
            args.input, ohtuzh::ITape::kRead, cfg);
        auto output = std::make_shared<ohtuzh::TapeImpl>(
            args.output, ohtuzh::ITape::kWrite, std::move(cfg));

        ohtuzh::TapeSorter sorter(std::move(input), std::move(output));
        sorter.Sort(args.memory_limit);

        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}

}  // namespace

int main(int argc, char** argv) {
    Arguments args = ParseArguments(argc, argv);
    return Run(args);
}