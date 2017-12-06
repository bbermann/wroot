#include "Crypto.hpp"
#include "../../3rdParty/Base64/Base64.hpp"

Crypto::Crypto(String value, CryptoEngine engine)
{
	this->setValue(value);
	this->setEngine(engine);
}

Crypto::~Crypto()
{
}

void Crypto::setValue(String value)
{
	value_ = value;
}

void Crypto::setEngine(CryptoEngine engine)
{
	engine_ = engine;
}

String& Crypto::value()
{
	return value_;
}

String& Crypto::decode()
{
	auto str = value_.c_str();
	auto ustr = reinterpret_cast<const unsigned char*>(str);
	unsigned int len = value_.length();

	crypto_value_ = "";

	switch (engine_)
	{
	case NoEngine:
		crypto_value_ = value_;
		break;
	case EngineBase64:
		crypto_value_ = base64_decode(value_);
		break;
	case EngineMd5:
		break;
	case EngineSha1:
		break;
	case EngineSha256:
		break;
	default:
		break;
	}

	return crypto_value_;
}

String& Crypto::encode()
{
	auto str = value_.c_str();
	auto ustr = reinterpret_cast<const unsigned char*>(str);
	unsigned int len = value_.length();

	crypto_value_ = "";

	switch (engine_)
	{
	case NoEngine:
		crypto_value_ = value_;
		break;
	case EngineBase64:
		crypto_value_ = base64_encode(ustr, len);
		break;
	case EngineMd5:
		break;
	case EngineSha1:
		break;
	case EngineSha256:
		break;
	default:
		break;
	}

	return crypto_value_;
}
