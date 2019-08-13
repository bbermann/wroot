#include "Crypto.hpp"
#include "include/helper/Base64.hpp"

Crypto::Crypto(String value, CryptoEngine engine) {
    this->setValue(value);
    this->setEngine(engine);
}

Crypto::~Crypto() {
}

void Crypto::setValue(String value) {
    value_ = value;
}

void Crypto::setEngine(CryptoEngine engine) {
    engine_ = engine;
}

String &Crypto::value() {
    return value_;
}

String &Crypto::decode() {
    auto str = value_.c_str();
    auto ustr = reinterpret_cast<const unsigned char *>(str);
    unsigned int len = value_.length();

    crypto_value_ = "";

    switch (engine_) {
        case CryptoEngine::NoEngine:
            crypto_value_ = value_;
            break;
        case CryptoEngine::EngineBase64:
            crypto_value_ = base64_decode(value_);
            break;
        default:
            throw CryptoEngineNotImplemented();
    }

    return crypto_value_;
}

String &Crypto::encode() {
    auto str = value_.c_str();
    auto ustr = reinterpret_cast<const unsigned char *>(str);
    unsigned int len = value_.length();

    crypto_value_ = "";

    switch (engine_) {
        case CryptoEngine::NoEngine:
            crypto_value_ = value_;
            break;
        case CryptoEngine::EngineBase64:
            crypto_value_ = base64_encode(ustr, len);
            break;
        default:
            break;
    }

    return crypto_value_;
}
