#include "TapeSorter.h"
#include <stdexcept>

namespace ohtuzh {
TapeSorter::TapeSorter(std::shared_ptr<ITape> reader,
                       std::shared_ptr<ITape> writer)
    : reader_(std::move(reader)), writer_(std::move(writer)) {}

void TapeSorter::Sort(const size_t memory_limit) {
    const size_t kBufferSize = memory_limit / sizeof(int32_t);

    if (kBufferSize == 0) {
        throw std::runtime_error("Not enough memory!");
    }

    reader_->SwitchMode(ITape::kRead);
    writer_->SwitchMode(ITape::kWrite);

    std::filesystem::create_directory("tmp");
    TapeImpl helper("tmp/helper.txt", ITape::kWrite);

    std::unique_ptr<int32_t[]> buffer(new int32_t[kBufferSize]);
    size_t index = 0;
    while (index < kBufferSize && reader_->Next()) {
        buffer[index++] = reader_->Read();
    }
    std::sort(buffer.get(), buffer.get() + index);
    for (size_t i = 0; i < index; ++i) {
        writer_->Next();
        writer_->Write(buffer[i]);
    }
    while (true) {
        helper.SwitchMode(ITape::kWrite);
        writer_->SwitchMode(ITape::kRead);

        index = 0;
        while (index < kBufferSize && reader_->Next()) {
            buffer[index++] = reader_->Read();
        }
        if (index == 0) {
            writer_->SwitchMode(ITape::kRead);
            return;
        }
        std::sort(buffer.get(), buffer.get() + index);
        while (writer_->Next()) {
            helper.Next();
            helper.Write(writer_->Read());
        }

        int i = 0;
        helper.SwitchMode(ITape::kRead);
        writer_->SwitchMode(ITape::kWrite);
        bool has_helper = helper.Next();
        bool has_buffer = (i < index);
        while (has_helper && has_buffer) {
            int32_t helper_val = helper.Read();
            int32_t buffer_val = buffer[i];
            writer_->Next();
            if (helper_val < buffer_val) {
                writer_->Write(helper_val);
                has_helper = helper.Next();
            } else {
                writer_->Write(buffer_val);
                ++i;
                has_buffer = (i < index);
            }
        }
        while (has_helper) {
            writer_->Next();
            writer_->Write(helper.Read());
            has_helper = helper.Next();
        }
        while (has_buffer) {
            writer_->Next();
            writer_->Write(buffer[i]);
            ++i;
            has_buffer = (i < index);
        }
    }
}
}  // namespace ohtuzh
