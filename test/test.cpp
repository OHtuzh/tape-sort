#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>
#include <iterator>
#include <tuple>
#include <vector>

#include "lib/TapeImpl.h"
#include "lib/TapeSorter.h"

const std::string kInputFileName = "input.txt";
const std::string kOutputFileName = "output.txt";
const size_t kDataSize = 100;
const size_t kMemoryLimit = 5;

std::vector<int> GenerateData() {
    srand(time(0));
    std::vector<int> target(kDataSize);

    std::generate(target.begin(), target.end(), rand);

    std::ofstream file(kInputFileName);
    std::copy(target.begin(), target.end(), std::ostream_iterator<int>(file, " "));

    return target;
}


TEST(TapeSorter, RandomRun) {
    auto target = GenerateData();

    auto input_tape = std::make_shared<ohtuzh::TapeImpl>(kInputFileName, ohtuzh::ITape::kRead);
    auto output_tape = std::make_shared<ohtuzh::TapeImpl>(kOutputFileName, ohtuzh::ITape::kWrite);
    ohtuzh::TapeSorter sorter(input_tape, output_tape);
    sorter.Sort(kMemoryLimit);

    std::ifstream file(kOutputFileName, std::ios::in);
    std::vector<int> result(std::istream_iterator<int>(file), std::istream_iterator<int>{});

    std::sort(target.begin(), target.end());
    ASSERT_EQ(result, target);
}

TEST(TapeImpl, Next) {
    auto target = GenerateData();
    ohtuzh::TapeImpl tape(kInputFileName, ohtuzh::ITape::kRead);

    for (size_t i = 0; i < kDataSize; ++i) {
        ASSERT_TRUE(tape.Next());
        ASSERT_EQ(target[i], tape.Read());
    }
}

TEST(TapeImpl, Prev) {
    auto target = GenerateData();
    ohtuzh::TapeImpl tape(kInputFileName, ohtuzh::ITape::kRead);

    for (size_t i = 0; i < kDataSize; ++i) {
        tape.Next();
    }

    ASSERT_EQ(target.back(), tape.Read());
    for (size_t i = 1; i < kDataSize; ++i) {
        ASSERT_TRUE(tape.Prev());
        ASSERT_EQ(target[kDataSize - i - 1], tape.Read());
    }
}
