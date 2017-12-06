#include "XmlCode.hpp"

XmlCode::XmlCode(String tag, String id)
{
	tag_ = tag;
	id_ = id;
}

XmlCode::~XmlCode()
{
}

void XmlCode::addChildTag(String value)
{
	value_.append(value);
}

String XmlCode::ToString()
{
	String strret = "<";
	strret.append(tag_);

	if (id_ != "")
	{
		strret.append(" id='" + id_ + "'");
	}

	strret.append(">");
	strret.append(value_);
	strret.append("</");
	strret.append(tag_);
	strret.append(">");
	return strret;
}