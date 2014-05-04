#include <fstream>
#include <string>
#include "FileSystem53.hpp"

/* Constructor of this File system.
 *   1. Initialize IO system.
 *   2. Format it if not done.
 */

FileSystem53::FileSystem53(int l, int b, string storage)
{
	B = 64; // Block length
	L = 64; // Total number of block
	K = 7; // Number of blocks for descriptor table
	
	number_of_files_in_ldisk = 0; // number of files in ldisk
	pLdisk = new char*[B];
	
	// Initialize 2d array dynamically
	for(int i = 0; i < L; i++) {
		pLdisk[i] = new char[L];
		
	}

	for(int i = 0; i < L; i++) {
		for(int j = 0; j < B; j++) {
			pLdisk[i][j] = 0;
			// cout << pLdisk[i][j];
			// ch++;
		}
	}



	pDesc_table = &pLdisk[0];

}

void FileSystem53::OpenFileTable(){

	oft = new class OpenFileTable[L];
	
	

}

int FileSystem53::find_oft() {
	for(int i = 0; i < L; i++) {
		if(oft[i].file_descriptor_index == -1)
			return i;
	}

	return -1;
}

void FileSystem53::deallocate_oft(int index) {
	for(int i = 0; i < L; i++) {
		oft[index].p_rw_buffer[i] = 0;	
	}
	oft[index].cp = 0;
	oft[index].file_descriptor_index = -1;
	
}

void FileSystem53::format() {

	// Initialize the first K blocks with zeros.
	// block 0 to 7 is 
	for(int i = 0 ; i < K; i++) {
		for(int j = 0; j < B; j++) {
			
			if((i==0 && j==0) || (i==1 && j ==0) ) {
				pDesc_table[i][j] = 1; // for root directory
			}else if (i == 2 && j ==0) {
				// First file Descriptor
				pDesc_table[i][j] = 0; 
			}else if (i == 2 && j == 1) {
				pDesc_table[i][j] = 7;
			}
			else {
				pDesc_table[i][j] = 0;
			}
		}
	}
	// TEST CASE 1
	/*pLdisk[2][0] = 1;
	pLdisk[2][1] = 7;
	pLdisk[2][2] = 21;
	pLdisk[2][3] = 22;


	pLdisk[7][0] = 'f';
	pLdisk[7][1] = 'o';
	pLdisk[7][2] = 'p';
	pLdisk[7][3] = 3;
	pLdisk[7][4] = 'f';
	pLdisk[7][5] = 'f';
	pLdisk[7][6] = 'f';
	pLdisk[7][7] = 3;

	pLdisk[21][0] = 'U';
	pLdisk[21][1] = 'C';
	pLdisk[21][2] = 'I';
	pLdisk[21][3] = 3;
	pLdisk[21][4] = 'U';
	pLdisk[21][5] = 'E';
	pLdisk[21][6] = 'F';
	// 
	pLdisk[22][0] = 'U';
	pLdisk[22][1] = 'C';
	pLdisk[22][2] = 'I';
	pLdisk[22][3] = 3;
	//
	pLdisk[23][0] = 'f';
	pLdisk[23][1] = 'o';
	pLdisk[23][2] = 'x';
	pLdisk[23][3] = 3;
	//
	pLdisk[24][0] = 'z';
	pLdisk[24][1] = 'o';
	pLdisk[24][2] = 'o';
	pLdisk[24][3] = 3;
	//
	pLdisk[25][0] = 'f';
	pLdisk[25][1] = 'o';
	pLdisk[25][2] = 'o';
	pLdisk[25][3] = 3;
	//
	pLdisk[26][0] = 'y';
	pLdisk[26][1] = 'y';
	pLdisk[26][2] = 'y';
	pLdisk[26][3] = 3;
		

	// TEST CASE 2
	pLdisk[2][4] = 1;
	pLdisk[2][5] = 24;
	pLdisk[2][6] = 25;
	pLdisk[2][7] = 26;
*/

	// pLdisk[1][1] = 21;
	// pDesc_table[2][0] = 'A'; // 0st
	// pDesc_table[2][4] = 'B'; // 1nd
	// pDesc_table[2][8] = 'C'; // 2rd
	// pDesc_table[2][12] = 'D'; // 3rd
	// pDesc_table[2][16] = 'E'; // 4th
	// pDesc_table[2][20] = 'F'; // 5th
	// pDesc_table[2][24] = 'G'; // 6th
	// pDesc_table[2][28] = 'H'; // 7th
	// pDesc_table[2][32] = 'I'; // 8th
	//cout << "pLdisk[2][1] : " <<pLdisk[2][1];
	//pLdisk[7][0] = '';

	
}

// SO FAR WORKS.
char* FileSystem53::read_descriptor(int no) {

	unsigned int blockNumber = (no  / 16) + 2;
	unsigned int locationOnBlock = (no * DESCR_SIZE) % 64;

//	cout << "\nRowNumber : " << blockNumber << endl;
//	cout << "ColumnNumber : " << locationOnBlock << endl;
	
	return pDesc_table[blockNumber]+locationOnBlock;
}

// SO FAR WORKS. 
void FileSystem53::clear_descriptor(int no) {

	unsigned int blockNumber = (no  / 16) + 2;
	unsigned int locationOnBlock = (no * DESCR_SIZE) % 64;
	// 1. Clear descriptor entry
	for(int i = 0 ; i < DESCR_SIZE; i++ ) {

		// Clear data region if it is not empty
		if(pDesc_table[blockNumber][locationOnBlock+i] != 0)
			empty_data_block(pDesc_table[blockNumber][locationOnBlock+i]); // 3.Write back to disk
		pDesc_table[blockNumber][locationOnBlock+i] = 0;
	}	
	
	// 2. Clear bitmap
	pDesc_table[0][no] = 0; // File Descriptor
	pDesc_table[1][no] = 0; // Data Region
	

}

// SO FAR WORKS
void FileSystem53::write_descriptor(int no, char* desc) {

	// 1.Upadte descriptor entry	
	unsigned int blockNumber = (no / 16) + 2;
	unsigned int locationOnBlock = (no * DESCR_SIZE) % 64;

	//cout << "write-blockNumber :" << blockNumber << endl;
	//cout << "locationOnBlock :" << locationOnBlock << endl;

	for(int i = 0 ; i < DESCR_SIZE; i++ ) {
		pDesc_table[blockNumber][locationOnBlock+i] = desc[i];
	}

	// 2. Mark bitmap 
	pLdisk[0][no] = 1; // File Descriptor

	//pLdisk[1][no] = 1; // Data Region
	// 3. Write back to disk
	//int firstEmptyDataBlock = find_empty_data_block();
	// while(desc[i] != 3) {
	// 	pLdisk[]
	// }

}

// SO FAR WORKS
int FileSystem53::find_empty_descriptor(){
	
	int fileDescriptorNumber = 0;

	for(int i = 0; i < 14+1; i++) {
		if(pDesc_table[0][i] == 0) {
			return i;
		}
	}
	
	// CASE: if not found
	return -1;

}

int FileSystem53::find_empty_block() {
	int result = -1;

	for(int i = 0; i < B; i++) {
		if(pDesc_table[0][i] == 0)
			return i;
	}

	return -1;
}

int FileSystem53::create(string str) {

	//write_descriptor(firstFreeDescriptor);
	// 1: creates empty file with file size zero
	// 2: makes/allocates descriptor
	// 3: updates directory file*/

	int firstFreeDescriptorIndex = -1;
	int firstEmptyDataBlock = -1;
	int initialFileSize = 0;
	int currentRootDirectoryBlockNumber = K;
	int lastOccupiedPlace = -1;
	
	char* pDirectory = pDesc_table[2]+0; // root data bitmap pointer
	char* pBuffer = new char[64];
	
	// Capacity check
	if(number_of_files_in_ldisk >= 14) {
		return -1;
	} else if(str.length() > MAX_FILE_NAME_LEN) {
		return -1; // FILE NAME CANNOT BE BIGGER THAN MAX FILE LENGTH
	}else {
		// Duplication Check
		if(pDesc_table[0][1] == 1) {
			
			read_block(K, pBuffer);			
			string tempStr = string(pBuffer);
			int endOfFileName = tempStr.find(str.append("\0"));
			string keyword = str.append("\3");
			int targetLocation = tempStr.find(keyword);
			//cout << "Target Location : " << targetLocation << endl;
			string substring = tempStr.substr(0, endOfFileName);

			// cout << "tempStr: " << tempStr << endl;
			// cout << "keyword: " << keyword << endl;
			// cout << "substring: " << substring << endl;

			if(str.compare(substring) == 0 || targetLocation > -1) {
				// cout << "DULPLICATION TERMINTATE\n";
				return -2;
			}	 
		} else {
			cout << "STARTED WITH EMPTY BLOCK" << endl;
		}
	}

	firstFreeDescriptorIndex = find_empty_descriptor();
	firstEmptyDataBlock = find_empty_data_block();

	const char* pFileName = str.c_str();	
	
	char* newFileInformation = new char[DESCR_SIZE];
	newFileInformation[0] = initialFileSize;
	newFileInformation[1] = firstEmptyDataBlock;
	newFileInformation[2] = 0;
	newFileInformation[3] = 0;
	
	write_descriptor(firstFreeDescriptorIndex, newFileInformation );

	// UPDATE DIRECTORY FILE.
	int firstEmptyIndex = -1;
	for(int index = 0; index < B; index++) {
		if(pLdisk[currentRootDirectoryBlockNumber][index] == 0) {
			// cout << "\n\nEMPTY INDEX : " << index << endl;
			firstEmptyIndex = index;
			break;
		}
		if( (index==63 && (pLdisk[currentRootDirectoryBlockNumber][index]!= 0)) && (currentRootDirectoryBlockNumber < 64) ) {
			//currentRootDirectoryBlockNumber +=1;
			// check second block of root directoy exist or not
			if(currentRootDirectoryBlockNumber == 7 && pDesc_table[2][2] != 0) {
				currentRootDirectoryBlockNumber = pDesc_table[2][2];
			} else if(pDesc_table[2][3] != 0){
				currentRootDirectoryBlockNumber = pDesc_table[2][3];
			} else {
				// THERE IS NO EMPTY SPOT! I HAVNE"T THINK ABOUT THIS CASE
				cout << "CHECK POINT!!" << endl; 
			}
		}
	}
	if(firstEmptyIndex == 0) {
		for(int i = firstEmptyIndex; i < (firstEmptyIndex + str.length());i++) {
				
			pLdisk[currentRootDirectoryBlockNumber][i] = pFileName[i-firstEmptyIndex];

			//cout << "pFileName:" << pFileName[i-firstEmptyIndex] << endl;
			
			// At the last, 
			if(i+1 == firstEmptyIndex + str.length()) {
				pLdisk[K][i+1] = 3;
				pLdisk[K][i+2] = firstFreeDescriptorIndex;
				pLdisk[K][i+3] = 4;
				number_of_files_in_ldisk+=1;
				pDesc_table[2][0] = number_of_files_in_ldisk;			
			}

			if(str.length()-1 == i)
				break;
			}
	} else {
		// cout << "HERE! : " << firstEmptyIndex << endl;
		
		for(int i = firstEmptyIndex; i < (firstEmptyIndex + str.length()-1); i++)  {
			pLdisk[currentRootDirectoryBlockNumber][i] = pFileName[i-firstEmptyIndex];
			//cout << "pFileName:" << pFileName[i-firstEmptyIndex] << endl;
			if(i+1 == firstEmptyIndex + str.length()-1) {
				pLdisk[K][i+1] = 3;
				pLdisk[K][i+2] = firstFreeDescriptorIndex;
				pLdisk[K][i+3] = 4;
				number_of_files_in_ldisk+=1;
				pDesc_table[2][0] = number_of_files_in_ldisk;		
			}
		}
	}

	return 0;
}

int FileSystem53::open_desc(int desc_no) {

	char* pBuffer = new char[64];
	string targetFileName = "";
	bool found = false;

	if(pDesc_table[0][desc_no] == 0) {
		delete pBuffer;
		return -1;
	}
	else {
		read_block(K, pBuffer);
		string bufferToString = string(pBuffer);
		string* tempFileList = fileNameParser(bufferToString);
		int* tempDescriptorIndex = fileDescriptorIndexParser(bufferToString, pBuffer);

		for(int i = 0; i < number_of_files_in_ldisk; i++) {
			if(tempDescriptorIndex[i] == desc_no) {
				found = true;
				targetFileName = tempFileList[i];
			}
		}

		if(found) 
			return open(targetFileName);
		else
			return -1;
		
	}

	return -1;
	
}

int FileSystem53::open(string symbolic_file_name) {
	// 1. check file is in directory or not
	// Duplication Check
	int oftIndex = -1;

	if(pDesc_table[0][1] == 1) {
		char* pBuffer = new char[64];
		read_block(K, pBuffer);		

		string tempBuffer = string(pBuffer);
		//cout << "TEMPBUFFER :" << tempBuffer << endl; 
		int endOfFileName = tempBuffer.find(symbolic_file_name.append("\0"));
		string keyword = symbolic_file_name.append("\3");

		int targetLocation = tempBuffer.find(keyword);

		if(targetLocation == -1) {
			return -1;
		}

		cout << "Target Location : " << targetLocation << endl;

		int tempTargetDescriptorIndex = targetLocation + symbolic_file_name.length()+1;
		
		int oftIndex = find_oft();
		oft[oftIndex].cp = 0;
		oft[oftIndex].file_descriptor_index = tempTargetDescriptorIndex;
		char* tempDescriptorBuffer = read_descriptor(tempTargetDescriptorIndex);
		read_block(tempDescriptorBuffer[1], pBuffer);
		for(int i = 0; i < 64; i++) {
			oft[oftIndex].p_rw_buffer[i] = pBuffer[i];
		}
	
	} else {
		// NO FILE IN ROOT.
		return -1; 
	}
	return 0;
}

int FileSystem53::write(int index, char value, int count) {

	return 0;
}




int FileSystem53::deleteFile(string fileName) {

	if(pDesc_table[0][1] == 1) {
		char* pBuffer = new char[64];
		read_block(K, pBuffer);		

		string tempBuffer = string(pBuffer);
		//cout << "TEMPBUFFER :" << tempBuffer << endl; 
		int endOfFileName = tempBuffer.find(fileName.append("\0"));
		string keyword = fileName.append("\3");

		int targetLocation = tempBuffer.find(keyword);

		if(targetLocation == -1) {
			return -1;
		}

		cout << "Target Location : " << targetLocation << endl;

		int tempTargetDescriptorIndex = targetLocation + fileName.length()+1;
		
		clear_descriptor(tempTargetDescriptorIndex);
		
		string temp = tempBuffer.erase(targetLocation, fileName.length()+2);
		

		for(int i = 0; i < temp.length(); i++) {
			pLdisk[K][i] = temp.at(i);
		}
		//string temp = tempBuffer.substr(targetLocation, fileName.length() + 2);

		cout << "TEMP :" << temp;

		// read_block(K, pBuffer);	

		// cout << "TEMPBUFFER :" << tempBuffer << endl;
		//string substring = tempBuffer.substr(0, endOfFileName);
		number_of_files_in_ldisk-=1;

		return 0;
	} else {
		// NO FILE IN ROOT.
		return -1; 
	}

}

// int FileSystem53::open_desc(int desc_no) {

// }

// int FileSystem53::write(int index, char value, int count) {


// }

void FileSystem53::directory() {
	char* pBuffer = new char[64];
	int startPosition = 0;
	int currentPosition = -1;
	int previousPosition = -1;
	string tempFileName = "";
	int tempDescriptorIndex = -1;
	int tempFileSize = -1;

	if((pDesc_table[2][0] == 0) && number_of_files_in_ldisk == 0) {
		cout << "EMPTY DIRECTORY";
		delete pBuffer;
	} else {
		read_block(K, pBuffer);	
		string bufferToString = string(pBuffer);
		string* fileNameList = fileNameParser(bufferToString);
		int* fileSizeList = fileSizeParser(bufferToString, pBuffer);

		for(int i = 0 ; i < number_of_files_in_ldisk; i++) {
			//printf("%s %d", fileNameList[i], fileSizeList[i]);
			cout << fileNameList[i] << " " << fileSizeList[i] << " bytes" << endl ;
		}
	}
}

int* FileSystem53::fileSizeParser(string str, char* buffer) {
	int* pFileSizeList = new int[number_of_files_in_ldisk];
	int startPosition = 0;
	int currentPosition = -1;
	int previousPosition = -1;
	string tempFileName = "";
	int tempTargetDescriptorIndex = -1;

	for(int i = 0; i < number_of_files_in_ldisk; i++ ) {
		
		currentPosition = str.find("\4", startPosition);
		
		if((currentPosition !=-1) && i==0 ) {
			tempFileName = str.substr(startPosition, currentPosition);
			tempTargetDescriptorIndex = (int)buffer[currentPosition-1];
			char* descriptor = read_descriptor(tempTargetDescriptorIndex);
			pFileSizeList[i] = descriptor[0];
			previousPosition = currentPosition;
			startPosition = currentPosition+1;
				
		} else {
			tempFileName = str.substr(startPosition, (currentPosition-previousPosition));
			tempTargetDescriptorIndex = (int)buffer[currentPosition-1];
			char* descriptor = read_descriptor(tempTargetDescriptorIndex);
			pFileSizeList[i] = descriptor[0];
			previousPosition = currentPosition;
			startPosition = currentPosition+1;
		}
	
	}
	return pFileSizeList;
}


string* FileSystem53::fileNameParser(string str) {
	string* pFileList = new string[number_of_files_in_ldisk];
	int startPosition = 0;
	int currentPosition = -1;
	int previousPosition = -1;
	string tempFileName = "";

	for(int i = 0; i < number_of_files_in_ldisk; i++ ) {
		
		currentPosition = str.find("\3", startPosition);
		
		if((currentPosition !=-1) && i==0 ) {
			tempFileName = str.substr(startPosition, currentPosition);
			pFileList[i] = tempFileName;
			previousPosition = currentPosition;
			startPosition = currentPosition+1;
				
		} else {
			tempFileName = str.substr(startPosition, (currentPosition-previousPosition));
			pFileList[i] = tempFileName;
			previousPosition = currentPosition;
			startPosition = currentPosition+1;
		}
	}

	return pFileList;
}



void FileSystem53::read_block(int i, char* p) {
	
	// 블락에있는 모든것을 읽어야함. 단지 하나만은 아니고..
    // *p = *pLdisk[i];
    // cout << *p << endl;
    
    for(int index = 0; index < B; index++) {
    	p[index] = pLdisk[i][index];
    }

	
}

void FileSystem53::write_block(int i, char* p) {

	for(int index = 0; index < B; index++) {
		pLdisk[i][index] = p[index];
	}

}


/*------------------------------------------------------------------
	  Disk management functions.
	  These functions are not really a part of file system.
	  They are provided for convenience in this emulated file system
--------------------------------------------------------------------*/

// Restores the saved disk image in a file to the array
void FileSystem53::restore() {

	/*
	// Step1 : read a single char from the file
	// Step2 : input it into block array
	*/

	ifstream targetFile;
	char singleCharacter;
	string line;

	
	targetFile.open("output.txt");
	if(targetFile.is_open()) {
		// NOTE: eof() reads line by line. So, not appropriate in this case.
		while(getline (targetFile,line)) {
			cout << line;
		}

		targetFile.close();

	}else {
		cout << "Unable to open a file.\n";
	}

	cout << endl;

}

// Saves the array to a file as a disk image.
void FileSystem53::save() {
	
	ofstream newFile;
	
	newFile.open("output.txt");
	for(int i = 0; i < B; i++) {
		for(int j = 0; j < L; j++) {
			newFile << pLdisk[i][j];
		}
	} 
	
	newFile.close();
	cout << endl;
}

// Find first empty data block from the lDisk.
int FileSystem53::find_empty_data_block() {
	for(int i = K; i < L; i++) {
		if(pLdisk[i][0] == 0)
			return i;
	}

	return -1;
}

void FileSystem53::empty_data_block(int index) {
	for(int i = index; i < B; i++) {
		pLdisk[index][i] = 0;
	}
}

void FileSystem53::checkFileNumber() {
	cout << "Number of Files in lDisk: " << number_of_files_in_ldisk << endl;
}

int* FileSystem53::fileDescriptorIndexParser(string str, char* buffer) {
	int* pFileSizeList = new int[number_of_files_in_ldisk];
	int startPosition = 0;
	int currentPosition = -1;
	int previousPosition = -1;
	string tempFileName = "";
	int tempTargetDescriptorIndex = -1;

	for(int i = 0; i < number_of_files_in_ldisk; i++ ) {
		
		currentPosition = str.find("\4", startPosition);
		
		if((currentPosition !=-1) && i==0 ) {
			tempFileName = str.substr(startPosition, currentPosition);
			tempTargetDescriptorIndex = (int)buffer[currentPosition-1];
			pFileSizeList[i] = tempTargetDescriptorIndex;
			previousPosition = currentPosition;
			startPosition = currentPosition+1;
				
		} else {
			tempFileName = str.substr(startPosition, (currentPosition-previousPosition));
			tempTargetDescriptorIndex = (int)buffer[currentPosition-1];
			pFileSizeList[i] = tempTargetDescriptorIndex;
			previousPosition = currentPosition;
			startPosition = currentPosition+1;
		}
	
	}
	

	return pFileSizeList;
}


void FileSystem53::versionInfo() {
	cout << "V0.1" << endl;

	for(int i = 0; i < L; i++) {
		for(int j = 0; j < B; j++) {
			cout << "[" << i << "," << j <<"]" << " = " << pLdisk[i][j] << "\t";
		} 
		cout << endl << endl;
	}
}