#include "ErrorLibrary.hpp"

using namespace std;

ErrorLibrary::ErrorLibrary() : CustomLibrary()
{
	this->responseType = "text/plain";
	CustomLibrary::is_empty = true;
}

ErrorLibrary::~ErrorLibrary()
{
}

void ErrorLibrary::root(string& plain_text)
{
	plain_text = "404 - Não encontrado.";
}

String ErrorLibrary::toString()
{
	return "";
}