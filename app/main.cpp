
// ICS53 Project1 Main
// Author: Byung Ho Lee

#include "FileSystem53.hpp"
#include "OFT.hpp"

int main()
{
    
    FileSystem53* fileSystem = new FileSystem53(64,64,"Testing");

    // char arr[] = {'q', 'w', 'e', 'r'};
    // fileSystem->write_descriptor(2, arr);

    // //std::cout << "Read_Descriptor : " << *fileSystem->read_descriptor(2) << std::endl;
    // for(int i = 0; i < 4 ; i ++) {
    //     std::cout << *(fileSystem->read_descriptor(2)+i) << std::endl;
    // }

    // std::cout << "Empty :" << fileSystem->find_empty_descriptor() << std::endl;

    ///// ROOT DIRECTORY


    fileSystem->format();
    int x = fileSystem->create("ANT");
    int x2 = fileSystem->create("dope");
    int x3 = fileSystem->create("coffee");
    // int xx = fileSystem->create("UCI");
    // int x4 = fileSystem->create("di");
    // cout << "DELETE EXPECTED -1 : "<<  fileSystem->deleteFile("UCI") << endl;
    // cout << "OPEN   EXPECTED -1 : " << fileSystem->open("ANT") << endl;
    cout << endl;
    cout << "----- D I R E C T O R Y INFO -----" << endl;
    fileSystem->directory();

    cout << "\n\n\n ------ FROM MAIN -----\n";
    //cout << fileSystem->create("boo");
    //cout << " find empty descriptor : " << fileSystem->find_empty_descriptor();
    //cout << "\nDirectory pointer : " << *fileSystem->read_descriptor(0) << endl;
    //fileSystem->versionInfo();

    fileSystem->checkFileNumber();
    fileSystem->save();

    //cout << fileSystem->create("hi");

    ///////////

    // //std::cout <<  fileSystem->find_empty_descriptor() << std::endl;
    // std::cout << "Read_Descriptor : " << *fileSystem->read_descriptor(2) << std::endl;
    // fileSystem->clear_descriptor(2);
    // std::cout << "Read_Descriptor : " << *fileSystem->read_descriptor(2) << std::endl;
    // for(int i = 0; i < 4 +1; i ++) {
    //     std::cout << *(fileSystem->read_descriptor(2)+i) << std::endl;
    // }
    // Below comments are for testing purpose
    //fileSystem->versionInfo();
    // fileSystem->save();
    // fileSystem->restore();

    // char* pTemp = new char[64];
    // for(int i = 0; i < 64; i++) {
    // 	pTemp[i] = 'Z';
    // }

    // // char x = 'X';
    // // char* pX = &x;

    // fileSystem->read_block(1, pTemp);
    // fileSystem->read_block(2, pTemp);

    // pTemp = new char[64];
    // for(int i = 0; i < 64; i++) {
    //     pTemp[i] = 'Z';
    // }
    // fileSystem->write_block(1, pTemp);
    // fileSystem->write_block(2, pTemp);

    // fileSystem->read_block(1, pTemp);
    // fileSystem->read_block(2, pTemp);

    // cout << pTemp[0];
    // cout << pTemp[1];
    // cout << pTemp[2];

    // string command;

    // do {
    // 	cout << "$ ";
    // 	getline(std::cin, command);
    // }while(command!="quit");

    delete fileSystem;

    return 0;
}

