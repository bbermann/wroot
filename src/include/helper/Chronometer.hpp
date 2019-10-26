//
// Created by bbermann on 13/10/2019.
//

#pragma once

#include <chrono>
#include <include/core/Core.hpp>

using std::chrono::high_resolution_clock;

class Chronometer {
public:
    explicit Chronometer(const String &label = "")
            : start_(high_resolution_clock::now()) {
    }

    ~Chronometer() {
        this->stop();
    }

    void stop() {
        if (!this->done_) {
            auto elapsed = high_resolution_clock::now() - this->start_;
            auto count = elapsed.count();
            String unit = "ns";

            if (count > 1000) {
                // Microseconds
                count = count / 1000;
                unit = "µs";

                if (count > 1000) {
                    // Milliseconds
                    count = count / 1000;
                    unit = "ms";
                }
            }

            Core::outLn("Elapsed " + std::to_string(count) + unit);

            this->done_ = true;
        }
    }

private:
    bool done_ = false;
    std::chrono::high_resolution_clock::time_point start_;
};