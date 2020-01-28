#include <ncurses.h>
#include "ProjektObiektowe.hpp"
#include "nano.hpp"
#include "nice-like.hpp"

int main()
{
    WorkersTool mytool;
    //Nano mybackend;
    nice mybackend;
    mybackend.tool = &mytool;
	mytool.backend = &mybackend;
    mytool.initial(1);
    mybackend.start();
	return 0;
}          
