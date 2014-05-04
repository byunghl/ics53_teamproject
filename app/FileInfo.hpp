#ifndef FILEINFO_HPP
#define FILEINFO_HPP

#include <string>
using namespace std;
class FileInfo {
	
private:
	string file_name;
	string file_size;
public:
	FileInfo();
	// FileInfo(string fileName, char fileSize);
	string getFileName();
	string getFileSize();
	void setFileName(string str);
	void setFileSize(string size);
};


#endif