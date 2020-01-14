#include <ncurses.h>
#include "ProjektObiektowe.hpp"
#include "nano.hpp"

int main()
{
    WorkersTool mytool;
    Nano nano;
    nano.tool = &mytool;
	mytool.backend = &nano;
    nano.bind("<EDITION>", [](){printw("k"); refresh();}, "help");
    nano.bind("<DEL>%New entry!Write in your entry${E}", [](){printw("J"); refresh();}, "help");
    mytool.initial();
    nano.start();
	return 0;
}          