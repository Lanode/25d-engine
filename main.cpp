#include "engine.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    Engine engine(argc, argv);
    engine.Run();
    return 0;
}
