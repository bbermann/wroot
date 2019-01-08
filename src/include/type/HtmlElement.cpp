#include "HtmlElement.hpp"
#include "../helper/ValidationHelper.hpp"
#include "../helper/FileHelper.hpp"
#include "../core/Core.hpp"
//#include <algorithm>

using namespace std;

HtmlElement::HtmlElement(string tag, string unique_id)
{
	tag_ = tag;
	id_ = unique_id;
}

HtmlElement::~HtmlElement()
{
}

string HtmlElement::toString(int level)
{
	string ret;
	string tabs;

	for (int it_level = 0; it_level < level; ++it_level)
	{
		tabs += "\t";
	}

	ret.append(tabs + "<" + tag_);

	if (!id_.empty())
	{
		ret.append(" id='" + id_ + "'");
	}

	for (string parameter : parameters_)
	{
		ret.append(parameter);
	}

	if (content_.empty() && elements_.empty() && tag_ != "script")
	{
		ret.append("/>\r\n");

		return ret;
	}
	else
	{
		ret.append(">\r\n");
	}

    if(prependContent_ && !content_.empty())
		ret.append(tabs + "\t" + content_ + "\r\n");

	for (HtmlElement element : elements_)
	{
		ret.append(element.toString(level + 1));
	}

    if(!prependContent_ && !content_.empty())
		ret.append(tabs + "\t" + content_ + "\r\n");

    ret.append(tabs + "</" + tag_ + ">\r\n");

	return ret;
}

void HtmlElement::addChild(HtmlElement element)
{
	elements_.insert(elements_.end(), element);
}

void HtmlElement::setContent(string content, bool prependContent)
{
    prependContent_ = prependContent;
	content_ = content;
}

void HtmlElement::_addParameter(string parameter, string value)
{
	parameters_.insert(parameters_.end(), " " + parameter + "=\"" + String::replace(value, "\"", "'") + "\"");
}

void HtmlElement::clear()
{
	elements_.clear();
	parameters_.clear();
	content_.clear();
}

void HtmlElement::useTemplate(string id)
{
	for (HtmlElement iterator : templates)
	{
		if (iterator.id_ == id)
		{
			this->addChild(iterator);
		}
	}
}

void HtmlElement::setTag(string tag)
{
	tag_ = tag;
}

void HtmlElement::setParameter(string parameter, string value)
{
	for (StringList::iterator it = parameters_.begin(); it != parameters_.end(); it++)
	{
		if (*it == parameter)
		{
			parameters_.erase(it);
			parameters_.shrink_to_fit();
		}
	}

	_addParameter(parameter, value);
}

void HtmlElement::removeChild(string unique_id)
{
	for (vector<HtmlElement>::iterator it = elements_.begin(); it != elements_.end(); it++)
	{
		if (it->getId() == unique_id)
		{
			elements_.erase(it);
			elements_.shrink_to_fit();
		}
	}
}

void HtmlElement::addTemplate(HtmlElement element)
{
	templates.insert(templates.end(), element);
}

void HtmlElement::createTemplates()
{
	HtmlElement bootstrapCss("link", "bootstrap-css");
	bootstrapCss.setParameter("rel", "stylesheet");
	bootstrapCss.setParameter("type", "text/css");
	bootstrapCss.setParameter("href", "/resources/assets/css/bootstrap.css");
	HtmlElement::addTemplate(bootstrapCss);

	HtmlElement bootstrapTheme("link", "bootstrap-theme");
	bootstrapTheme.setParameter("rel", "stylesheet");
	bootstrapTheme.setParameter("type", "text/css");
	bootstrapTheme.setParameter("href", "/resources/assets/css/bootstrap-theme.css");
	HtmlElement::addTemplate(bootstrapTheme);

	HtmlElement styles("link", "default-style");
	styles.setParameter("rel", "stylesheet");
	styles.setParameter("type", "text/css");
	styles.setParameter("href", "/resources/assets/css/default.css");
	HtmlElement::addTemplate(styles);

	HtmlElement jQuery("script", "jQuery");
	jQuery.setParameter("type", "text/javascript");
	jQuery.setParameter("src", "/resources/assets/js/jquery-1.11.3.min.js");
	HtmlElement::addTemplate(jQuery);

	HtmlElement ajax("script", "ajax");
	ajax.setParameter("type", "text/javascript");
	ajax.setParameter("src", "/resources/assets/js/ajax.js");
	HtmlElement::addTemplate(ajax);

	HtmlElement bootstrapJs("script", "bootstrap-js");
	bootstrapJs.setParameter("type", "text/javascript");
	bootstrapJs.setParameter("src", "/resources/assets/js/bootstrap.js");
	HtmlElement::addTemplate(bootstrapJs);

	HtmlElement footer("footer", "footer");
	HtmlElement::addTemplate(footer);
}

string HtmlElement::getId()
{
	return id_;
}

void HtmlElement::setId(string unique_id)
{
	Validation::set(id_, unique_id, ValidationCondition::NotEmpty);
}
