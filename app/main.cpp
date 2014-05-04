
// ICS53 Project1 Main
// Author: Byung Ho Lee

#include "FileSystem53.hpp"
#include "OFT.hpp"

int main()
{
    
    FileSystem53* fileSystem = new FileSystem53(64,64,"Testing");

    ///// ROOT DIRECTORY


    fileSystem->format();
    int x = fileSystem->create("ANT");
    int x2 = fileSystem->create("dope");
    int x3 = fileSystem->create("coffee");
    // int xx = fileSystem->create("UCI");
    // int x4 = fileSystem->create("di");
    cout << "DELETE EXPECTED -1 : "<<  fileSystem->deleteFile("UCI") << endl;
    cout << "DELETE EXPECTED -1 : "<<  fileSystem->deleteFile("ANT") << endl;
    // cout << "OPEN   EXPECTED -1 : " << fileSystem->open("ANT") << endl;
    cout << endl;
    cout << "----- D I R E C T O R Y INFO -----" << endl;
    //fileSystem->open
    fileSystem->directory();

    cout << "\n\n\n ------ FROM MAIN -----\n";
    //cout << fileSystem->create("boo");
    //cout << " find empty descriptor : " << fileSystem->find_empty_descriptor();
    //cout << "\nDirectory pointer : " << *fileSystem->read_descriptor(0) << endl;
    //fileSystem->versionInfo();

    fileSystem->checkFileNumber();
    fileSystem->save();

 
    delete fileSystem;

    return 0;
}

