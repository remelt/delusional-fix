#pragma once
#include <vector>
#include <memory>
#include <cstring>
#include <cwchar>
#include <winrt/Windows.Foundation.h>

class StringPool {
public:
    char* allocate(const winrt::hstring& source) {
        size_t len = source.size() + 1;
        auto buf = std::make_unique<char[]>(len);
        std::wcstombs(buf.get(), source.c_str(), len);

        char* ptr = buf.get();
        pool_.push_back(std::move(buf));
        return ptr;
    }

    void clear() noexcept {
        pool_.clear();
    }

private:
    std::vector<std::unique_ptr<char[]>> pool_;
};
