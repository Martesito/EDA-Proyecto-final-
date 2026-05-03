#pragma once
#include <chrono>
#include <functional>

class Timer {
public:
    static double measure(const std::function<void()>& func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();

        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};