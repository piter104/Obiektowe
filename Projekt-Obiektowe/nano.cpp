#include "nano.hpp"
#include <ncurses.h>
#define ctrl(x) ((x)& 0x1f)

WINDOW *create_new_window(int height, int width, int startY, int startX)
{
    WINDOW *local_window;

    local_window = newwin(height, width, startY, startX);
    box(local_window, 0, 0);
    wrefresh(local_window);

    return local_window;
}

void destroy_window(WINDOW *local_window)
{
    wborder(local_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(local_window);
    delwin(local_window);
}

void Nano::quit()
{
    bool q = false;
    if(tool->getEntry("IS_SAVED") == "NO")
    // if(q == true)
    {
        int y, x;
        getmaxyx(stdscr, y, x);
        WINDOW *tmp = create_new_window(8, 50, y/2 - 4, x/2 - 25);
        mvwprintw(tmp, 3, 17, "File not saved");
        wrefresh(tmp);
        getch();
        wclear(tmp);
        wrefresh(tmp);
        destroy_window(tmp);
    }
    else
    {
        exit = true;
    }
    
}

void Nano::help()
{
    int x,y;
    getmaxyx(stdscr,y,x);
    WINDOW *help_win = create_new_window(y - 4, x/2, 1, x/4-1 );
    mvwprintw(help_win, 2, x/4 - 2, "HELP");
    for(int i = 0; i < bindings.size(); i++)
    {
        std::string name;
        if(bindings[i].name == "<EDITION>")
        {
            name = "EDITION-";
        }
        else
        {
            int t = bindings[i].name.find('-');
            name = bindings[i].name.substr(0, t+1);
        }
        name +=bindings[i].help;
        mvwprintw(help_win, 4 + i * 2, 2, name.c_str());
    }
    wrefresh(help_win);
    wgetch(help_win);
    wclear(help_win);
    wrefresh(help_win);
    destroy_window(help_win);
}


void Nano::bind(std::string str, std::function<void()> func, std::string help)
{
    BindingString new_bind;
    std::string id = "#nano", name, tmp;
    char c;
    if((str.find(id, 0) != -1 || str[0] != '#') && str != "<EDITION>")
    {
        int l_bracket = str.find('<', 0), r_bracket = str.find('>', 0);
        if(l_bracket != -1 && r_bracket != -1)
        {
            tmp.assign(str, l_bracket + 1, r_bracket - l_bracket -1);
        }
        c = str[r_bracket + 1];
        for(int i = str.find('%', 0) + 1; i < str.size(); i++)
        {
            name.push_back(str[i]);
        }
        
        new_bind.func = func;
        new_bind.name=tmp;
        if(c != '%')
        {
            new_bind.name+='+';
            new_bind.name+=c;
        }
        
        new_bind.name+='-';
        new_bind.name+=name;
        new_bind.name.shrink_to_fit();
        new_bind.help = help;
        this->bindings.push_back(new_bind);
    }
    else if(str == "<EDITION>")
    {
        new_bind.name = str;
        new_bind.help = help;
        new_bind.func = func;
        bindings.push_back(new_bind);
    }
}

std::vector<int> Nano::GetBindTab()
{
    std::vector<int> tab;
    char tmp;
    for(int i = 0; i < bindings.size(); i++)
    {
        if(bindings[i].name == "<EDITION>")
        {
            tab.push_back(-1);
            continue;
        }
        std::string key;
        int k = bindings[i].name.find('+');
        if(k == -1)
        {
            k = bindings[i].name.find('-');
        }
        key.assign(bindings[i].name, 0, k);
        if(key == "CTRL")
        {
            tab.push_back(ctrl(bindings[i].name[k+1]));
        }
        else if (key == "SHIFT")
        {
            const int x = 32;
            tab.push_back(bindings[i].name[k+1] & ~x);
        }
        else if (key[0] == 'F')
        {
            tab.push_back(KEY_F(atoi(&key[1])));
        }
        else if (key == "DEL")
        {
            tab.push_back(KEY_DC);
        }
        else if (key == "BS")
        {
            tab.push_back(KEY_BACKSPACE);
        }
        else if (key == "RARROW")
        {
            tab.push_back(KEY_RIGHT);
        }
        else if (key == "LARROW")
        {
            tab.push_back(KEY_LEFT);
        }
        else if (key == "UARROW")
        {
            tab.push_back(KEY_UP);
        }
        else if (key == "DARROW")
        {
            tab.push_back(KEY_DOWN);
        }
        
        
        
    }
    return tab;
}

Nano::Nano()
{
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
}



void Nano::start()
{
    int row, col;
    getmaxyx(stdscr, row, col);
    move(row-1, 1);
    refresh();

    bind("#nano#<CTRL>Q%Quit", [&](){this->quit();}, "Exit program");
    bind("#nano#<CTRL>H%Help", [&](){this->help();}, "Show every keyboard shortcut with its help");

    std::vector<int> bindTab = GetBindTab();

    for(int i = 0; i < bindings.size(); i++)
    {
        if(bindings[i].name != "<EDITION>")
        {
            int prompt; //zmienna przechowująca pozycję znaku "!" w nazwie 
            if((prompt = bindings[i].name.find('!')) != -1 )
            {
                std::string txt = bindings[i].name.substr(0,prompt);
                printw(txt.c_str());
                printw("\t");
            }
            else
            {
                printw(bindings[i].name.c_str());
                printw("\t");
            }
        }
    }
    refresh();
    int a = 0;
    while(!exit)
    {
        a = getch();
        int i;
        for(i = 0; i < bindTab.size(); i++)
        {
            if(a == bindTab[i])
            {
                int prompt; // ponownie zmienna przechowująca pozycję znaku "!"
                if((prompt = bindings[i].name.find('!')) != -1)
                {
                    int l = bindings[i].name.find('{'), r = bindings[i].name.find('}');
                    std::string text = bindings[i].name.substr(prompt+1, bindings[i].name.find('$') - prompt - 1);
                    std::string entry,
                    name = bindings[i].name.substr(l+1, r - l - 2);
                    char tmp[100];
                    int width = text.size(); 
                    WINDOW *message = create_new_window(8, width + 4, row/2 - 4, col/2 - width/2);
                    
                    mvwprintw(message, 3, 2, text.c_str());
                    wrefresh(message);
                    wmove(message, 5, width/2);
                    echo();
                    wrefresh(message);
                    mvwscanw(message, 5, width/2 - 2, "%s", tmp);
                    entry = tmp;
                    tool->setEntry(name, entry);
                    noecho();
                    wclear(message);
                    wrefresh(message);
                    destroy_window(message);
                }
                
                bindings[i].func();
                i = 0;
                break;
            }
        }
        if(i == bindTab.size())
        {
            for(int j = 0; j < bindings.size(); j++)
            {
                if(bindings[j].name == "<EDITION>")
                {
                    std::string tmp;
                    tmp.push_back(a);
                    tool->setEntry("KEY", tmp);
                    bindings[j].func();
                }
            }
        }

    }
    
    endwin();
}