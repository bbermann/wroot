#pragma once

#include <include/core/Core.hpp>
#include <exception>

enum class CryptoEngine {
    NoEngine,
    EngineBase64,
    EngineMd5,
};

class CryptoEngineNotImplemented : public std::exception {
public:
    virtual char const *what() const noexcept override {
        return "CryptoEngine not implemented yet.";
    }
};

class Crypto {
public:
    Crypto(String value, CryptoEngine engine = CryptoEngine::EngineMd5);

    ~Crypto();

    void setValue(String value);

    void setEngine(CryptoEngine engine);

    String &value();

    String &decode();

    String &encode();

protected:
    String value_;
    String crypto_value_;
    CryptoEngine engine_;
};