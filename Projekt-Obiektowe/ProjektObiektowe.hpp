#ifndef ProjektObiektowe_HPP_INCLUDED_
#define ProjektObiektowe_HPP_INCLUDED_

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <ncurses.h>
#include "shared.hpp"

using namespace std;

class Pracownik
{
public:
	char imie[20];
	char nazwisko[20];
	int wiek;
	int grupy[20];
	int counter=0;

	void init(char name[20], char surname[20], int age);
	void worker_groups(int i, char name[20]);
};

class lista
{
public:
	Pracownik member;
	lista* next = nullptr;
};

class Grupa
{
public:
	char name[20];
	lista* previous = new lista;
	lista* actual = new lista;
	lista* current = new lista;
	lista* first = new lista;
	bool InitialWorker = false;



	void init(char name[20]);

	void AddWorker(Pracownik worker);

	Pracownik FindWorker(char name[20], char surname[20]);

	bool IsIn(char name[20], char surname[20]);

	void DeleteWorker(Pracownik tmp);

	void ShowGroup(int number);

	void Save(ofstream& plik1);
};

class WorkersTool : public Tool
{
    public:
    	Pracownik SpisPracownikow[50];
	    Grupa SpisGrup[10];

	    int decision=0, counter=0, counter1=-1, counter2=0, age=0, workerint=0, groupint=0, reminder, key2;

	    bool looper = true, looper2 = true, possible=false, possible1=false, possible2=false, possible3=false, possible4=false, possible5=false, possible6=false;
	    char name[20], surname[20], temp[20];
	    string imie, nazwisko, napis="Group", field;
	    Pracownik tmp;
		string file;
	    fstream plik;
	    fstream plik1;
        Backend *backend=nullptr;
        void setEntry(string field, string value);
        string getEntry(string field);
		void initial();
        void init();
        void redraw();
		void enter();
		void move_up();
		void move_down();
		void creat_worker();
		void show_workers();
		void creat_group();
		void add_to_group();
		void delete_worker();
		void show_group();
		void show_worker_groups();
		void show_all();
		void save_quit();
};

#endif
