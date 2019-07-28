#pragma once

#include "../type/String.hpp"

enum ValidationCondition {
    NoCondition = 0,
    IsZero = 1,
    IsPositive = 2,
    IsNegative = 4,
    NotNegative = IsZero | IsPositive,
    NotPositive = IsZero | IsNegative,
    NotEmpty = 8,
};

namespace Validation {

    static bool isValid(int value, ValidationCondition condition = NoCondition) {
        switch (condition) {
            case NoCondition:
                return true;
            case IsZero:
                return value == 0;
            case IsPositive:
                return value > 0;
            case IsNegative:
                return value < 0;
            case NotNegative:
                return value >= 0;
            case NotPositive:
                return value <= 0;
            default:
                break;
        }

        return false;
    }

    static bool isValid(String value, ValidationCondition condition = NoCondition) {
        switch (condition) {
            case NoCondition:
                return true;

            case NotEmpty:
                return !value.empty();

            default:
                return false;
        }
    }

    template<class T>
    static void set(T &var, T &value, ValidationCondition condition = NoCondition) {
        if (isValid(value, condition)) {
            var = value;
            return;
        }

        throw std::invalid_argument("Invalid argument in set method.");
    }
};

