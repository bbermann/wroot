/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileHelper.hpp
 * Author: bbermann
 *
 * Created on 5 de Janeiro de 2016, 00:07
 */

#ifndef FILEHELPER_HPP
#define FILEHELPER_HPP

#include "../type/String.hpp"

namespace File {
    enum OpenMode {
        Read,
        Write,
        WriteAppend,
    };
}

class FileHelper {
public:
    FileHelper();
    FileHelper(const FileHelper& orig);
    virtual ~FileHelper();
    
    String Read(String filename);
    void Write(String filename, String text);
    bool Exists(String filename, bool canBeFolder = false);
    bool CheckExtension(String filename, String extension);
    
private:
	void Open(File::OpenMode open_mode);
	void VerifyFile();
	void SetFilename(String filename);
	String GetOpenMode(File::OpenMode open_mode);

    FILE* file_;
    String filename_;

};

#endif /* FILEHELPER_HPP */

