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
	
	cp = 0; // current position;
	pLdisk = new char*[B];
	
	// Initialize 2d array dynamically
	for(int i = 0; i < L; i++) {
		pLdisk[i] = new char[L];
		cout << endl;
	}

	for(int i = 0; i < L; i++) {
		for(int j = 0; j < B; j++) {
			pLdisk[i][j] = 0;
			// cout << pLdisk[i][j];
			// ch++;
		}
	}

	pDesc_table = &pLdisk[0];

	////////// FOR TESTING //////////
	pDesc_table[2][0] = 'A'; // 0st
	pDesc_table[2][4] = 'B'; // 1nd
	pDesc_table[2][8] = 'C'; // 2rd
	pDesc_table[2][12] = 'D'; // 3rd
	pDesc_table[2][16] = 'E'; // 4th
	pDesc_table[2][20] = 'F'; // 5th
	pDesc_table[2][24] = 'G'; // 6th
	pDesc_table[2][28] = 'H'; // 7th
	pDesc_table[2][32] = 'I'; // 8th

	// pDesc_table[3][0] = '1'; // 16th
	// pDesc_table[3][4] = '1'; // 17th
	// pDesc_table[3][8] = '0'; // 18th


	// cout << *pDesc_table << endl;
	// cout << pDesc_table[0][0] << endl;
	// char ch = *read_descriptor(6); // 24 = (1,8)
	// cout << "After function call: " << ch << endl;

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
			} 
			else {
				pDesc_table[i][j] = 0;
			}
		}
	}
	// ERR
	pLdisk[2][1] = 21;
	pLdisk[21][0] = 'A';
	pLdisk[21][1] = 'B';
	pLdisk[21][2] = 'C';
	pLdisk[21][3] = 'D';
	pLdisk[21][4] = 3;
	pLdisk[21][5] = 'E';
	pLdisk[21][6] = 'F';


	pLdisk[1][1] = 21;

	pDesc_table[2][0] = 'A'; // 0st
	pDesc_table[2][4] = 'B'; // 1nd
	pDesc_table[2][8] = 'C'; // 2rd
	pDesc_table[2][12] = 'D'; // 3rd
	pDesc_table[2][16] = 'E'; // 4th
	pDesc_table[2][20] = 'F'; // 5th
	pDesc_table[2][24] = 'G'; // 6th
	pDesc_table[2][28] = 'H'; // 7th
	pDesc_table[2][32] = 'I'; // 8th
	//cout << "pLdisk[2][1] : " <<pLdisk[2][1];
	//pLdisk[7][0] = '';

	
}

// SO FAR WORKS.
char* FileSystem53::read_descriptor(int no) {

	unsigned int blockNumber = ((no * DESCR_SIZE) / 16) + 2;
	unsigned int locationOnBlock = (no * DESCR_SIZE) % 16;

	cout << "RowNumber : " << blockNumber << endl;
	cout << "ColumnNumber : " << locationOnBlock << endl;
	
	return pDesc_table[blockNumber]+locationOnBlock;
}

// SO FAR WORKS. 
void FileSystem53::clear_descriptor(int no) {

	unsigned int blockNumber = ((no * DESCR_SIZE) / 16) + 2;
	unsigned int locationOnBlock = (no * DESCR_SIZE) % 16;
	// 1. Clear descriptor entry
	for(int i = 0 ; i < DESCR_SIZE; i++ ) {
		pDesc_table[blockNumber][locationOnBlock+i] = 0;
	}	
	
	// 2. Clear bitmap
	pDesc_table[0][no] = 0; // File Descriptor
	pDesc_table[1][no] = 0; // Data Region
	// 3.Write back to disk

}

// SO FAR WORKS
void FileSystem53::write_descriptor(int no, char* desc) {

	// 1.Upadte descriptor entry	
	unsigned int blockNumber = ((no * DESCR_SIZE) / 16) + 2;
	unsigned int locationOnBlock = (no * DESCR_SIZE) % 16;

	cout << "write-blockNumber " << blockNumber << endl;
	cout << "locationOnBlock " << locationOnBlock << endl;
	for(int i = 0 ; i < DESCR_SIZE; i++ ) {
		pDesc_table[blockNumber][locationOnBlock+i] = desc[i];
	}

	// 2. Mark bitmap 
	pLdisk[0][no] = 1; // File Descriptor
	pLdisk[1][no] = 1; // Data Region
	// 3. Write back to disk

}

// SO FAR WORKS
int FileSystem53::find_empty_descriptor(){
	
	int fileDescriptorNumber = 0;

	for(int i = 2; i < K; i++) {
		//cout << "Loop: " << i << endl;	
		for(int j = 0; j < B; j+=DESCR_SIZE) {
			//cout << "Inner Loop: " << j << endl;
			//cout << "pDesc_table " << i << " : " << j << " = " << pDesc_table[i][j] << endl; 	
			if(pDesc_table[i][j] == 0) {	
				fileDescriptorNumber += (j/DESCR_SIZE);
				return fileDescriptorNumber;
			}
		}
		fileDescriptorNumber += 16;
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

	int lastOccupiedPlace = -1;
	int firstEmptyDataBlock = -1;
	char* pBuffer = new char[64];
	// if iNode bitmap is not empty, search directory
	
	for(int i = 1; i < MAX_FILE_NO+1; i++) {
		if(pDesc_table[0][i] == 0) 
			break;
		if (MAX_FILE_NO == i)
			return -1;
	}

	char* pDirectory = pDesc_table[1]+0; // root
	for(int i = 1; i < DESCR_SIZE; i++) {
		if(pDirectory[i] != 0) {
			read_block(pDirectory[i], pBuffer);
			//cout << "string(buffer): " << string(pBuffer) << endl; 		
			string tempStr = string(pBuffer);
			//cout << "!!" << tempStr << "!!";
			int endOfFileName = tempStr.find(str.append("\3"));
			if(str.compare(tempStr.substr(0, endOfFileName))) {
				// Same name exist (DUPLICATION!)
				cout << "DULPLICATION";
				return -2;
			}
			else {
				cout << "Different";
				firstEmptyDataBlock = find_empty_data_block();
			}
		} else {
			firstEmptyDataBlock = find_empty_data_block();
			break; 
		}
	}

	int firstFreeDescriptor = find_empty_descriptor();
	
	// 1: creates empty file with file size zero
		


	// 2: makes/allocates descriptor
	// 3: updates directory file

	return firstFreeDescriptor;
}


void FileSystem53::read_block(int i, char* p) {
	
	// 블락에있는 모든것을 읽어야함. 단지 하나만은 아니고..
    *p = *pLdisk[i];
    // cout << *p << endl;
    
    // cout << endl;
    // cout << "READ BLOCK\n";

    for(int index = 0; index < B; index++) {
    	p[index] = pLdisk[i][index];
    	//cout << p[index];
    }

	
}

void FileSystem53::write_block(int i, char* p) {

	// cout << endl;
	// cout << "WRITE BLOCK\n";

	for(int index = 0; index < B; index++) {
		pLdisk[i][index] = p[index];
		cout << pLdisk[i][index];
	}


	//cout << *pLdisk[i] << endl;
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



void FileSystem53::versionInfo() {
	cout << "V0.1" << endl;

	for(int i = 0; i < L; i++) {
		for(int j = 0; j < B; j++) {
			cout << "[" << i << "," << j <<"]" << " = " << pLdisk[i][j] << "\t";
		} 
		cout << endl << endl;
	}
}