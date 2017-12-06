#ifndef FILELIBRARY_H
#define FILELIBRARY_H

#define BUFFER_SIZE 1024

#include "CustomLibrary.hpp"

typedef std::vector<std::pair<String, String>> FileIndexList;
typedef std::pair<String, String> FileIndex;

class FileLibrary : public CustomLibrary
{
public:
    FileLibrary();
    virtual ~FileLibrary();
    virtual String toString() override;

	void setResponseType();
    
    FileIndexList file_list_;

protected:
	String getFileName();
	String getFullPath();
	String getFileExtension();
};

#endif // FILELIBRARY_H
