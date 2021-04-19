#include "UselessPch.h"

#include "IFileStreamBuffer.h"

#ifdef _TEST_I_FILE_STREAM
#include "Useless/File/StdIFile.h"
int main()
{
    // Test std::getline
    {
        Useless::SPointer<Useless::IFile> file = new Useless::StdIFile("../../../../HowToBuild.txt");
        Useless::IFileStreamBuffer sbuf(file);
        std::istream st( &sbuf);
        std::string str;

        int i=0;
        while ( std::getline( st, str))
        {
            std::cout << "Line [" << i++ << "]: "<< str << endl;
        }
    }
    // Test operator >>
    {
        Useless::SPointer<Useless::IFile> file = new Useless::StdIFile("../../../../HowToBuild.txt");
        Useless::IFileStreamBuffer sbuf(file);
        std::istream st( &sbuf);
        std::string str;

        int i=0;
        while ( st >> str)
        {
            std::cout << "Word [" << i++ << "]: "<< str << endl;
        }
    }
    return 0;
}
#endif//_TEST_I_FILE_STREAM
