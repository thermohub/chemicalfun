#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    string test_dir = ".";

    if( argc > 1)
        test_dir = argv[1];

    try{

      return 0;
    }
    catch( std::exception& e )
    {
        std::cout << "std::exception: " << e.what() <<  std::endl;
    }
    catch(...)
    {
        std::cout <<  "unknown exception" <<  std::endl;
    }

    return 1;
}

