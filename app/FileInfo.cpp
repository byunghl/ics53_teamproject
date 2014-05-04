#include "FileInfo.hpp"
FileInfo::FileInfo() {

}
// FileInfo::FileInfo(string fileName, char fileSize) {
// 	file_name = fileName;
// 	file_size = fileSize;
// }

string FileInfo::getFileName() {
	return file_name;
}

string FileInfo::getFileSize() {
	return file_size;
}

void FileInfo::setFileName(string str) {
	file_name = str;
}

void FileInfo::setFileSize(string size) {
	file_size = size;
}