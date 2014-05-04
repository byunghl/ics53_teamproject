#ifndef OFT_HPP
#define OFT_HPP

class OpenFileTable {
	
private:

public:
	char p_rw_buffer[64];
	int cp;
	int file_descriptor_index;

	OpenFileTable();

};


#endif