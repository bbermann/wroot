/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileHelper.cpp
 * Author: bbermann
 * 
 * Created on 5 de Janeiro de 2016, 00:07
 */

#include "FileHelper.hpp"
#include "../core/Core.hpp"
#include <exception>
#include <fstream>
#include <string>

using namespace std;

FileHelper::FileHelper()
{
}

FileHelper::FileHelper(const FileHelper& orig)
{
}

FileHelper::~FileHelper()
{
}

void FileHelper::SetFilename(String filename)
{
    filename_ = filename;
}

String FileHelper::GetOpenMode(File::OpenMode open_mode)
{
    String ret = "";

    switch (open_mode)
    {
    case File::OpenMode::Read:
        ret.append("r");
		break;

    case File::OpenMode::Write:
        ret.append("w");
		break;

    case File::OpenMode::WriteAppend:
        if (ret.endsWith("w"))
            ret.append("+");
        else
            ret.append("w+");
		break;
    }

    return ret;
}

void FileHelper::Open(File::OpenMode open_mode)
{
    file_ = fopen(filename_.c_str(), GetOpenMode(open_mode).c_str());
    VerifyFile();
}

String FileHelper::Read(String filename)
{
	//TODO: Verificar erro (std::logic_error) ao passar filename nao existente.
	SetFilename(filename);
	Open(File::OpenMode::Read);

    String ret, currentLine;
	try
	{
		ifstream file;
		file.open(filename, ios::in | ios::binary);

		if (file.is_open())
		{
			while (file.good())
			{
				getline(file, currentLine);
				ret.append(currentLine + LINE_BREAK);
			}

			file.close();
		}

	}
	catch (exception e)
	{
		Core::error("Um erro foi disparado em FileHelper::Read(): " + String(e.what()));
	}
    return ret;
}

void FileHelper::Write(String filename, String text)
{
	SetFilename(filename);
	Open(File::OpenMode::Write);

    fwrite(&text, text.size(), text.size(), file_);
}

void FileHelper::VerifyFile()
{
    if (file_ == nullptr)
    {
        throw std::logic_error("Não foi possível abrir o arquivo " + filename_ + "!");
    }
}

bool FileHelper::Exists(String filename, bool canBeFolder)
{
	bool exists = false;

	try
    {
		ifstream file(filename);

        if (file.is_open())
        {
            exists = true;

            if (!canBeFolder) 
            {
                if (filename.endsWith("/")) 
                {
                    exists = false;
                } 
                else 
                {
                    //This will set the fail bit if filename is a directory
                    file.seekg(0, ios::end);
                    
                    if (!file.good()) {
                        exists = false;
                    };
                }
            }

            file.close();
        }

    }
    catch (exception e)
    {
        Core::error("Um erro foi disparado em FileHelper::Exists(String): " + String(e.what()));
    }
	
	return exists;
}

bool FileHelper::CheckExtension(String filename, String extension)
{
	return filename.endsWith("." + extension);
}