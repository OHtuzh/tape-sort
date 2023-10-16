#include "TapeImpl.h"

#include <stdexcept>
#include <thread>

namespace ohtuzh {
TapeImpl::TapeImpl(std::string filename, Mode mode, Config config)
    : filename_(std::move(filename)), mode_(mode), config_(std::move(config)) {
    if (mode_ == kRead) {
        file_.open(filename_, std::ios::in);
    } else {
        file_.open(filename_, std::ios::out);
    }
    if (!file_) {
        throw std::runtime_error("There's no such file: '" + filename_);
    }
}

bool TapeImpl::Next() {
    std::this_thread::sleep_for(config_.GetShiftDelay());
    if (mode_ == kWrite) {
        return true;
    }
    if (file_ >> current_) {
        ++index_;
        return true;
    }
    return false;
}

bool TapeImpl::Prev() {
    std::this_thread::sleep_for(config_.GetShiftDelay());
    if (mode_ == kWrite) {
        throw std::runtime_error("cannot prev");
    }
    if (index_ == 0) {
        return false;
    }
    Back();
    Back();
    file_ >> current_;
    --index_;
    return true;
}

int32_t TapeImpl::Read() {
    std::this_thread::sleep_for(config_.GetReadDelay());
    if (mode_ == kWrite) {
        throw std::runtime_error("cannot prev");
    }
    return current_;
}

void TapeImpl::Write(int32_t val) {
    std::this_thread::sleep_for(config_.GetWriteDelay());
    if (mode_ == kRead) {
        throw std::runtime_error("cannot write");
    }
    file_ << val << ' ';
}

void TapeImpl::RewindToStart() {
    std::this_thread::sleep_for(config_.GetRewindDelay());
    if (mode_ == kWrite) {
        file_.close();
        file_.open(filename_);
        return;
    }
    file_.seekg(file_.beg);
}

void TapeImpl::RewindToEnd() {
    std::this_thread::sleep_for(config_.GetRewindDelay());
    if (mode_ == kWrite) {
        return;
    }

    file_.seekg(-1, file_.end);
    Back();
}

void TapeImpl::SwitchMode(Mode mode) {
    if (mode_ == mode) {
        RewindToStart();
        return;
    }
    mode_ = mode;

    index_ = 0;
    file_.close();
    if (mode_ == kWrite) {
        file_.open(filename_, std::ios::out);
        return;
    }
    file_.open(filename_, std::ios::in);
}

void TapeImpl::Back() {
    while (file_.tellg() != 0 && std::isspace(file_.peek())) {
        file_.seekg(-1, std::ios_base::cur);
    }
    while (file_.tellg() != 0 && std::isdigit(file_.peek())) {
        file_.seekg(-1, std::ios_base::cur);
    }
}
}  // namespace ohtuzh
