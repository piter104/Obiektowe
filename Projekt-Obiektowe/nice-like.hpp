#ifndef Nice_like_hpp
#define Nice_like_hpp

#include <ncurses.h>
#include <vector>
#include <string>
#include <map>
#include "shared.hpp"

#define KEY_ESC 27
#define KEY_ENT '\n'

using namespace std;

class nice: public Backend
{
    private:
        map<int, string> special_keys;
        BindingString edit_bind;
        vector<vector<BindingString>> menu;
        virtual void draw_main_menu(int h);
        virtual void draw_submenu(int h, int n);
        virtual string draw_box(string str);
        virtual void main_menu_controler();
        virtual void submenu_controler(int n);
        virtual void quit();
        virtual void help();
    public:
        nice();
        virtual void bind(std::string str, std::function<void()> func, std::string help);
        virtual void start();
};

#endif
