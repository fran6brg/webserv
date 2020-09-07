// ifstream constructor.
#include <iostream> // std::cout
#include <fstream>  // std::ifstream

int main()
{

    std::ifstream ifs("test1.txt", std::ifstream::in);

    if (ifs.good()) // _file already exists
    {
        std::cout << "good" << std::endl;
    }
    else // _file has been created
    {
        std::cout << "not good" << std::endl;
    }

    char c = ifs.get();

    while (ifs.good())
    {
        std::cout << c;
        c = ifs.get();
    }

    ifs.close();

    return 0;
}