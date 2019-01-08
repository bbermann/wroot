#include "FileLibrary.hpp"
#include "../core/Core.hpp"
#include "../type/Process.hpp"
#include <fstream>
#include "../../3rdParty/zlib/ZLib.hpp"

using namespace std;

FileLibrary::FileLibrary() : CustomLibrary()
{
	//This library implicitly compress the output
	this->compressedOutput = false;
}

FileLibrary::~FileLibrary()
{

}

String FileLibrary::toString()
{
	this->setResponseType();

	String returnString, currentLine;
	String fullPath = this->getFullPath();
	
	Core::debugLn("[FileLibrary] Reading file: " + fullPath);

    for (FileIndex index : this->file_list_)
    {
        if (index.first == fullPath)
        {
            Core::debugLn("[FileLibrary] Retrieving file cache for \"" + fullPath + "\".");
            return index.second;
        }
    }

    try
    {
		ifstream file;
        
		file.open(fullPath, ios::in | ios::binary);

        if (file.is_open())
        {
            while (file.good())
            {
                getline(file, currentLine);
                returnString.append(currentLine + LINE_BREAK);
            }

            file.close();
        }

    }
    catch (exception e)
    {
        Core::error("Um erro foi disparado em FileLibrary::toString(): " + String(e.what()));
    }

    FileIndex newIndex;
    newIndex.first = fullPath;
    newIndex.second = returnString;
    
    this->file_list_.insert(this->file_list_.end(), newIndex);

    return returnString;
}

String FileLibrary::getFileName()
{
	String fileName = this->getRequestUrl();
	
	if (fileName.empty() || fileName == "/") 
	{
		fileName = "/index.html";
	}

	return fileName;
}

String FileLibrary::getFullPath()
{
	String fileName = this->getFileName();
	String fullPath = Core::DocumentRoot;
	if (fullPath.endsWith("/")) {
		fullPath.append(Core::PathSeparator);
	}
	fullPath.append(fileName);
	return fullPath;
}

String FileLibrary::getFileExtension()
{
	String fileName = this->getFileName();
	String file_extension = fileName.explode(".").back();
	return file_extension;
}

void FileLibrary::setResponseType() 
{
	String fileExtension = this->getFileExtension();

	if (fileExtension == "html")
	{
		this->responseType = "text/html";
	}
	else if (fileExtension == "xml")
	{
		this->responseType = "application/xhtml";
	}
	else if (fileExtension == "css")
	{
		this->responseType = "text/css";
	}
	else if (fileExtension == "js")
	{
		this->responseType = "text/javascript";
	}
	else if (fileExtension == "png" || fileExtension == "ico")
	{
		this->responseType = "image/png";
	}
	else if (fileExtension == "jpg" || fileExtension == "jpeg")
	{
		this->responseType = "image/jpeg";
	}
	else if (fileExtension == "gif")
	{
		this->responseType = "image/gif";
	}
	else
	{
		this->responseType = "application/octet-stream";
	}
}