#include "../core/Core.hpp"
#include "Html.hpp"

Html::Html(String xml_namespace, String manifest)
{
    setXmlNamespace(xml_namespace);
    setManifest(manifest);

    HtmlElement meta("meta");
    meta.setParameter("charset", "UTF-8");

    head.setTag("head");
    head.addChild(meta);

    body.setTag("body");
}

Html::~Html()
{
}

void Html::setXmlNamespace(String xml_namespace)
{
    xmlns = xml_namespace;
}

void Html::setManifest(String manifest)
{
    manifest = manifest;
}

String Html::toString()
{
    String strret = "<!DOCTYPE HTML>\r\n";
	strret.append("<html");

    if (!xmlns.empty())
    {
        strret.append(" xmlns='");
        strret.append(xmlns);
        strret.append("'");
    }

    if (!manifest.empty())
    {
        strret.append(" manifest='");
        strret.append(manifest);
        strret.append("'");
    }

    strret.append(">\r\n");

    strret.append(head.toString());
    strret.append(body.toString());
    strret.append("</html>");

	return strret; //formatHtml(strret);
}

String Html::getResponse(String htmlCode)
{
    String tmpstr = htmlCode;
    String strret = "";

    StringList strvct = String::regex_search(tmpstr, "response/.+/.+");
    if (strvct.size() > 0)
    {
        strret = strvct.at(0);
        strvct = strret.explode("/");

        if (strvct.size() >= 3)
        {
            strret = strvct.at(3);
            String::replace(strret, "'>", "");
        }
    }

    Core::debugLn("RESPONSE: " + strret);

    return strret;
}

String Html::formatHtml(String value)
{
    String strRet = value;
    String::replace(strRet, "</", "\r\n</");
    String::replace(strRet, ">", ">\r\n");
    String::replace(strRet, "\r\n\r\n", "\r\n");

    return strRet;
}
