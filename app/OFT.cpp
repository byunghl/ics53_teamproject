#include "OFT.hpp"

OpenFileTable::OpenFileTable() {
	char p_rw_buffer[64];
	for(int i = 0; i < 64; i++) {
		p_rw_buffer[i] = 0;
	}
	int cp = 0;
	int file_descriptor_index =-1;
}