#include "shared.hpp"
#include "nano.hpp"
#include <ncurses.h>

int main()
{
    
    Nano nano;
    nano.bind("<EDITION>", [](){printw("k"); refresh();}, "help");
    nano.bind("<DEL>%New entry!Write in your entry${E}", [](){printw("J"); refresh();}, "help");
    nano.start();
    return 0;
}
