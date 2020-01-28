#include "nice-like.hpp"

void nice::draw_main_menu(int h)
{

    move(LINES, 0);
    clrtoeol();
    move(0, 0);
    clrtoeol();
    int x = 0;
    for(int i = 0; i < menu[0].size(); i++)
    {
        if(h == i)
        {
            attron(A_REVERSE);
            mvprintw(0, x, menu[0][i].name.c_str());
            attroff(A_REVERSE);
            x += menu[0][i].name.size() + 1;
        }
        else
        {
            mvprintw(0, x, menu[0][i].name.c_str());
            x += menu[0][i].name.size() + 1;
        }
    }
}

void nice::draw_submenu(int h, int n)
{
    int y = 1, x = 0;
    for(int i = 0; i < n-1; i++)
    {
        x += menu[0][i].name.size() + 1;
    }
    for(int i = 0; i < menu[n].size(); i++)
    {
        if(h == i)
        {
            attron(A_REVERSE);
            mvprintw(y, x, menu[n][i].name.c_str());
            attroff(A_REVERSE);
            y++;
        }
        else
        {
            mvprintw(y, x, menu[n][i].name.c_str());
            y++;
        }
    }
    refresh();
}

nice::nice() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    special_keys[KEY_ENTER] = "<ENTER>";
    special_keys[KEY_BACKSPACE] = "<BS>";
    special_keys[KEY_DC] = "<DEL>";
    special_keys[KEY_UP] = "<UARROW>";
    special_keys[KEY_RIGHT] = "<RARROW>";
    special_keys[KEY_DOWN] = "<DARROW>";
    special_keys[KEY_LEFT] = "<LARROW>";
    for(int i = 1; i <= 12; i++)
    {
        string tmp = "<F" + to_string(i) + ">";
        special_keys[KEY_F(i)] = tmp;
    }
    bind(".File.Quit", [this]() { quit(); }, "Close program");
    bind(".File.Help", [this]() { help(); }, "Show functions descriptions");
}

void nice::start()
{
    
    draw_main_menu(-1);
    refresh();
    tool->redraw();
    
    while(1)
    {
        int c = getch();
        switch(c)
        {
            case KEY_ESC:
                draw_main_menu(0);
                refresh();
                main_menu_controler();
                break;
            default:
                if(special_keys.find(c) == special_keys.end())
                {
                    string p(1, c);
                    tool->setEntry("KEY", p);
                }
                else
                {
                    tool->setEntry("KEY", special_keys[c]);
                }
                if(edit_bind.func != NULL)
                edit_bind.func();
        }
    }
}

void nice::bind(std::string str, std::function<void()> func, std::string help)
{
    if(str == "<EDITION>")
    {
        edit_bind.name = str;
        edit_bind.func = func;
        edit_bind.help = help;
    }
    string buf = "";
    int dot;
    function<void()> bfunc;
    if(str[0] == '#')
    {
        if(str[1] != 'n' && str[2] != 'i' && str[3] != 'c' && str[4] != 'e' && str[5] != '#')
        {
            return;
        }
        else
        {
            str.erase(str.begin(), str.begin()+6);
        }
    }
    if(str[0] != '.') return;
    str.erase(str.begin());
    int dpos = str.find("${");
    if(dpos != string::npos)
    {
        int epos = str.find("|");
        bfunc = [&, dpos, epos, str, func](){tool->setEntry(str.substr(epos+1, str.size()-epos-2), this->draw_box(str.substr(dpos+2, epos-dpos-2))); func();};
        str.erase(str.begin()+dpos, str.end());
    }
    else bfunc = func;
    dot = str.find('.');
    buf = str.substr(0, dot);
    
    if(menu.size() == 0)
    {
        vector<BindingString> bsbuf = {{str.substr(dot+1, str.size()), help, bfunc}};
        vector<BindingString> bsvbuf = {{buf}};
        menu.emplace(menu.end(), bsvbuf);
        menu.emplace(menu.end(), bsbuf);
        return;
    }
    for(int i = 0; i < menu[0].size(); i++)
    {
        if(buf.compare(menu[0][i].name) == 0)
        {
            BindingString bsbuf = {str.substr(dot+1, str.size()-1), help, bfunc};
            menu[i+1].emplace(menu[i+1].end(), bsbuf);
            break;
        }
        else if(buf.compare(menu[0][i].name) != 0 && i == menu[0].size() - 1)
        {
            BindingString bsbuf = {buf};
            menu[0].emplace(menu[0].end(), bsbuf);
            vector<BindingString> bsvbuf = {{str.substr(dot+1, str.size()), help, bfunc}};
            menu.emplace(menu.end(), bsvbuf);
            break;
        }
    }   
}

void nice::main_menu_controler()
{
    int c, h = 0;
    while(1)
    {
        c = getch();
        switch (c)
        {
        case KEY_RIGHT:
            if(h < menu[0].size()-1) h++;
            else h = 0;
            break;
        case KEY_LEFT:
            if(h > 0) h--;
            else h = menu[0].size()-1;
            break;
        case KEY_ENT:
            tool->redraw();
            draw_submenu(0, h+1);
            refresh();
            submenu_controler(h+1);
            return;
        case KEY_ESC:
            draw_main_menu(-1);
            refresh();
            return;
        }
        draw_main_menu(h);
        refresh();
    }
}

void nice::submenu_controler(int n)
{
    int c, h = 0;
    while(1)
    {
        c = getch();
        switch (c)
        {
        case KEY_DOWN:
            if(h < menu[n].size()-1) h++;
            else h = 0;
            break;
        case KEY_UP:
            if(h > 0) h--;
            else h = menu[n].size()-1;
            break;
        case KEY_ENT:
                menu[n][h].func();
                //tool->redraw();
                draw_main_menu(-1);
                refresh();
                return;
        case KEY_ESC:
            draw_main_menu(-1);
            tool->redraw();
            refresh();
            return;
        }
        draw_submenu(h, n);
        refresh();
    }
}

void nice::quit()
{
    if(tool->getEntry("IS_SAVED") == "YES")
    {
            endwin();
            exit(0);
    }
    else
    {
        WINDOW *tmp;
        tmp = newwin(3, 84, LINES/2 - 1, COLS/2 - 42);
        box(tmp, 0 ,0);
        mvwaddstr(tmp, 1, 1, "File is not saved. Press any key to continue. Press ENTER if you still want to quit.");
        wrefresh(tmp);
        char a = getch();
        if(a == KEY_ENT)
        {
            endwin();
            exit(0);            
        }
        wclear(tmp);
        delwin(tmp);
        return;
        }

}
string nice::draw_box(string str)
{
    WINDOW *tmp;
    string ret;
    int x = str.size();
    char t[33];
    if(x < 32) tmp = newwin(4, 35, LINES/2 - 2, COLS/2 - x/2);
    else tmp = newwin(4, x+2, LINES/2 - 2, COLS/2 - x/2);
    mvwaddstr(tmp, 1, 1, str.c_str());
    mvwaddstr(tmp, 2, 1, "Entry:");
    box(tmp, 0, 0);
    wrefresh(tmp);
    echo();
    mvwgetstr(tmp, 2, 7, t);
    noecho();
    ret = t;
    wclear(tmp);
    delwin(tmp);
    tool->redraw();
    return ret;

}

void nice::help()
{
    WINDOW *tmp = newwin(LINES, COLS, 0, 0);
    int y = 0;
    for(int i = 1; i < menu.size(); i++)
    {
        for(int j = 0; j < menu[i].size(); j++)
        {
            mvwprintw(tmp, y++, 0, "%s: ", menu[i][j].name.c_str());
            wprintw(tmp, "%s", menu[i][j].help.c_str());
        }
    }
    mvwaddstr(tmp, LINES-1, 0, "Press any key to leave.");
    wrefresh(tmp);
    getch();
    wclear(tmp);
    wrefresh(tmp);
    delwin(tmp);
    return;
}