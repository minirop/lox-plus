#include <iostream>
#include "Lox-plus.h"

int main(int argc, char** argv)
{
    if (argc > 2)
    {
        std::cout << "Usage: lox-plus [file.lox]\n";
        return 1;
    }
    else if (argc == 2)
    {
        return LoxPlus::runFile(argv[1]);
    }
    else
    {
        LoxPlus::runPrompt();
    }

    return 0;
}
