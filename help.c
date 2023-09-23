#include "help.h"
#include <stdio.h>

void showHelp()
{
    printf(HELP_SCREEN_TEXT);

    int option = getchar();

    if (option == 50)
    {
        printf(DUMP_TYPES_TEXT);
    }

}