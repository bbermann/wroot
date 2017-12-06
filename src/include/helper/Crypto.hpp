#pragma once

#include "../core/Core.hpp"

enum CryptoEngine {
	NoEngine,
	EngineBase64,
	EngineMd5,
	EngineSha1,
	EngineSha256
};

class Crypto
{
public:
	Crypto(String value, CryptoEngine engine = CryptoEngine::EngineMd5);
	~Crypto();

	void setValue(String value);
	void setEngine(CryptoEngine engine);

	String& value();
	String& decode();
	String& encode();

protected:
	String value_;
	String crypto_value_;
	CryptoEngine engine_;
};