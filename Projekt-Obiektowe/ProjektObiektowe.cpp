
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <ncurses.h>
#include "shared.hpp"
#include "ProjektObiektowe.hpp"

using namespace std;



void Pracownik::init(char name[20], char surname[20], int age)
{
    strcpy(imie, name);
    strcpy(nazwisko, surname);
    wiek = age;
}

void Pracownik::worker_groups(int i, char name[20])
{
        mvprintw(i+4, 30, "%d. %s", i, name);
}

char* Pracownik::getName()
{
    return imie;
}

char* Pracownik::getSurname()
{
    return nazwisko;
}

int Pracownik::getAge()
{
    return wiek;
}

void Grupa::init(char name[20])
{
    strcpy(this->name, name);
}

char* Grupa::getName()
{
    return name;
}

void Grupa::AddWorker(Pracownik worker)
{
    if (!InitialWorker)
    {
        InitialWorker = true;
        lista* tmp = new lista;
        tmp->member = worker;
        tmp->next = nullptr;
        first = tmp;
        current = tmp;
    }
    else
    {
        lista* tmp = new lista;
        if (!first->next)
            first->next = tmp;
        current->next = tmp;
        tmp->member = worker;
        tmp->next = nullptr;
        current = tmp;
    }
}

Pracownik Grupa::FindWorker(char name[20], char surname[20])
{
    lista* tmp = first;
    while (tmp)
    {
        if (strcmp(name, tmp->member.getName()) == 0 && strcmp(surname, tmp->member.getSurname()) == 0)
        {
            actual = tmp;
            return tmp->member;
            break;
        }
        previous = tmp;
        tmp = tmp->next;
    }
    return previous->member;
}

bool Grupa::IsIn(char name[20], char surname[20])
{
    lista* tmp = first;
    while (tmp)
    {
        if (strcmp(name, tmp->member.getName()) == 0 && strcmp(surname, tmp->member.getSurname()) == 0)
        {
            return true;
            break;
        }
        tmp = tmp->next;
    }
    return false;
}

void Grupa::DeleteWorker(Pracownik tmp)
{
    if (actual == first)
    {
        first = first->next;
    }
    else if (actual->next)
        previous->next = actual->next;
    else
        previous->next = nullptr;
}

void Grupa::ShowGroup(int number)
{
    int counter = 1;
    lista* tmp = first;
    mvprintw( 2, number*25, "Group %s", getName() );
    while (tmp)
    {
        mvprintw(  counter+2, number*25, "%d. %s %s",  counter ,tmp->member.getName(), tmp->member.getSurname() );
        counter++;
        tmp = tmp->next;
    }
}

void Grupa::Save(ofstream& plik1)
{
    lista* tmp = first;
    plik1 << "Group " << name << endl;
    while (tmp)
    {
        plik1 << tmp->member.getName() << " " << tmp->member.getSurname() << " " << tmp->member.getAge()<< endl;
        tmp = tmp->next;
    }
}


void WorkersTool::setEntry(string value, string field)
{
    this->field = field;
}

string WorkersTool::getEntry(string field)
{
    return field;
}

void WorkersTool::initial()
{
    if(backend)
    {
    backend->bind("#nano#<LARROW>%Init",[&](){init();}, "Init File and program");
    backend->bind("#nano#<UARROW>%Up",[&](){move_up();}, "Move cursor up");
    backend->bind("#nano#<RARROW>%Enter",[&](){enter();}, "Enter");
    backend->bind("#nano#<DARROW>%Down",[&](){move_down();}, "Move cursor down");
    backend->bind("#nano#<F1>%Create New Worker",[&](){creat_worker();}, "Create New Worker");
    backend->bind("#nano#<F2>%Show All Workers",[&](){show_workers();}, "Show All Workers");
    backend->bind("#nano#<F3>%Create New Group",[&](){creat_group();}, "Create New Group");
    backend->bind("#nano#<F4>%Add Worker To Group",[&](){add_to_group();}, "Add Worker To Group");
    backend->bind("#nano#<F5>%Delete worker",[&](){delete_worker();}, "Delete worker");
    backend->bind("#nano#<F6>%Show Group",[&](){show_group();}, "Show Group");
    backend->bind("#nano#<F7>%Show Worker Groups",[&](){show_worker_groups();}, "Show Worker Groups");
    backend->bind("#nano#<F8>%Show All Groups",[&](){show_all();}, "Show All Groups");
    backend->bind("#nano#<F9>%Save and Quit",[&](){save_quit();}, "Save and Quit");
    }
}

void WorkersTool::init()
{
    file = this->field;
    int x=0;
    plik.open(file, ios::in);
    if (plik.good() == true)
    {
        mvprintw(0, 0, "Uzyskano dostep do pliku!");
        //cout << "Uzyskano dostep do pliku!" << endl;
        while (!plik.eof())
        {
            plik >> temp;
            
            if (strcmp(temp, napis.c_str()) == 0)
            {
                plik >> temp;
                strcpy(SpisGrup[++counter1].getName(), temp);
               // cout << SpisGrup[counter1].name << endl;

            }
            else
            {
                plik >> surname;
                plik >> age;
                if (SpisGrup[0].IsIn(temp, surname) == false)
                {
                SpisPracownikow[counter].init(temp, surname, age);
                SpisGrup[counter1].AddWorker(SpisPracownikow[counter]);
                SpisPracownikow[counter].grupy[SpisPracownikow[counter].counter++] = counter1;
                counter++;
                }
                else
                {
                    for(int i=0; i<counter; i++)
                    {
                        if (strcmp(SpisPracownikow[i].getName(), temp) == 0 && strcmp(SpisPracownikow[i].getSurname(), surname) == 0)
                            {
                                SpisGrup[counter1].AddWorker(SpisPracownikow[i]);
                                SpisPracownikow[i].grupy[SpisPracownikow[i].counter++] = counter1;
                            }
                    }
                }
            }
        }
    }
    else
         mvprintw(0, 0, "Nie uzyskano dostepu do pliku!");
    //cout << endl << endl;
}

void WorkersTool::redraw()
{
    for(int i=0; i<12; i++)
    {
        move(i, 0);
        clrtoeol(); 
    }
    if(possible1==true)
    {
        mvprintw( 1, 0, "Wybierz pracownika");
        if(groupint>counter)
        {
            groupint=counter;
        }
        if(groupint<0)
        {
            groupint=0;
        }
        for(int i = 0; i < counter; i++)
        {
            if( groupint == i)
            {
                reminder=i;
            }
            else
            {
                mvprintw( i+2, 0, "%s %s", SpisPracownikow[i].getName(), SpisPracownikow[i].getSurname());
            }
        }
        attron( A_REVERSE );
        mvprintw(reminder + 2, 0, "%s %s", SpisPracownikow[reminder].getName(), SpisPracownikow[reminder].getSurname());
        attroff( A_REVERSE );
        workerint = reminder;
    }

    if(possible2==true)
    {
        mvprintw( 1, 30, "Wybierz grupe");
        if(groupint>counter1)
        {
            groupint=counter1;
        }
        if(groupint<0)
        {
            groupint=0;
        }
        for(int i = 0; i < counter1+1; i++)
        {
            if(groupint == i)
            {
                reminder = i;
            }
            else
            {
                mvprintw( i+2, 30, "%s", SpisGrup[i].getName());
            }
        }
        attron( A_REVERSE );
        mvprintw(reminder + 2, 30, "%s", SpisGrup[reminder].getName());
        attroff( A_REVERSE );
    }
}

void WorkersTool::enter()
{
    //possible - zdolnosc do ruszania strzalkami
    //possible1 - menu pracownikow
    //possible2 - menu grup
    //possible3 - zezwolenie dodania do grupy
    //possible4 - zezwolenie usuniecia z grupy
    //possible5 - zezwolenie pokazania grupy
    //possible6 - zezwolenie pokazania grup pracownika
    if(possible2==true)
    {
        possible2=false;

        if(possible3==true)
        {
            if (SpisGrup[reminder].IsIn(SpisPracownikow[workerint].getName(), SpisPracownikow[workerint].getSurname()))
            {
                mvprintw( counter+5, 0, "Pracownik istnieje juz w tej grupie");
                return;
            }
            SpisGrup[groupint].AddWorker(SpisPracownikow[workerint]);
            SpisPracownikow[counter].grupy[SpisPracownikow[counter].counter++] = counter1;
            mvprintw( counter+5, 0, "Dodano pracownika");
            possible3=false;
        }

        if(possible5==true)
            {
                SpisGrup[groupint].ShowGroup(groupint);
                possible5=false;
            }

        possible=false;  
    }

    if(possible1==true)
    {
        possible1=false;
        if(possible3==true)
        {
            possible2=true;
            groupint=0;
            mvprintw( 1, 30, "Wybierz grupe");
            for(int i = 0; i < counter1+1; i++)
            {
                if(groupint == i)
                {
                    reminder = i;
                }
                else
                {
                    mvprintw( i+2, 30, "%s", SpisGrup[i].getName());
                }
                
            }
            attron( A_REVERSE );
            mvprintw(reminder + 2, 30, "%s", SpisGrup[reminder].getName());
            attroff( A_REVERSE );
        }

        if(possible4==true)
        {
            for (int i = 0; i < counter1 + 1; i++)
            {
                tmp = SpisGrup[i].FindWorker(SpisPracownikow[i].getName(), SpisPracownikow[i].getSurname());
                if (SpisGrup[i].IsIn(SpisPracownikow[i].getName(), SpisPracownikow[i].getSurname()) == false)
                    continue;
                SpisGrup[i].DeleteWorker(tmp);
                SpisPracownikow[workerint]=SpisPracownikow[counter--];
            }
            mvprintw(counter+2, 0, "Usunieto pracownika");
            possible4=false;
            possible=false;
        }

        if(possible6==true)
        {
            mvprintw(counter1+5, 0, "Worker is in groups:");
            for(int i = 0; i < SpisPracownikow[groupint].counter; i++)
            SpisPracownikow[groupint].worker_groups(i+1, SpisGrup[SpisPracownikow[groupint].grupy[i]].getName());
            possible6=false;
            possible=false;
        }
    }
}

void WorkersTool::move_up()
{
    if(possible)
    {
        groupint--;
        redraw();
    }
}

void WorkersTool::move_down()
{
    if(possible)
    {
        groupint++;
        redraw();
    }
}

void WorkersTool::creat_worker()
{
    redraw();
    mvprintw( 0, 0, "Dodawanie pracownika" );
    mvprintw( 1, 0, "Podaj imie: " );
    echo();
    scanw("%s", &name);
    noecho();
    mvprintw( 2, 0, "Podaj nazwisko: " );
    echo();
    scanw("%s", &surname);
    noecho();
    mvprintw( 3, 0, "Podaj wiek: : " );
    echo();
    scanw("%d", &age);
    noecho();
    if (SpisGrup[0].IsIn(name, surname))
    {
        mvprintw( 5, 0, "Pracownik juz istnieje!" );
        return;
    }
    SpisPracownikow[counter].init(name, surname, age);
    SpisGrup[0].AddWorker(SpisPracownikow[counter]);
    mvprintw( 5, 0, "Pracownik dodany: %s %s %d", SpisPracownikow[counter].getName(), SpisPracownikow[counter].getSurname(), SpisPracownikow[counter].getAge() );
    counter++;

}

void WorkersTool::show_workers()
{
    redraw();
    mvprintw( 0, 0, "Wszyscy pracownicy" );
    SpisGrup[0].ShowGroup(0);
}

void WorkersTool::creat_group()
{
        redraw();
    mvprintw( 0, 0, "Tworzenie grupy" );
    mvprintw( 1, 0, "Podaj nazwe grupy: " );
    echo();
    scanw("%s", SpisGrup[++counter1].getName());
    noecho();
    mvprintw( 2, 0, "Grupa %s utworzona", SpisGrup[counter1].getName());
}

void WorkersTool::add_to_group()
{
    redraw();
    mvprintw( 0, 0, "Dodawanie do grupy" );
    possible1=true;
    possible=true;
    possible2=false;
    possible3=true;
    workerint = 0;
    groupint = 0;
    mvprintw( 1, 0, "Wybierz pracownika");
    for(int i = 0; i < counter; i++)
    {
        if( groupint == i)
        {
            reminder=i;
        }
        else
        {
            mvprintw( i+2, 0, "%s %s", SpisPracownikow[i].getName(), SpisPracownikow[i].getSurname());
        }
    }
    attron( A_REVERSE );
    mvprintw(reminder + 2, 0, "%s %s", SpisPracownikow[reminder].getName(), SpisPracownikow[reminder].getSurname());
    attroff( A_REVERSE );
    workerint = reminder;
        
}

void WorkersTool::delete_worker()
{   
    redraw();
    mvprintw( 0, 0, "Usuwanie pracownika"); 
    possible4=true;
    possible1=true;
    possible=true;
    groupint = 0;
    mvprintw( 1, 0, "Wybierz pracownika");
    for(int i = 0; i < counter; i++)
    {
        if( groupint == i)
        {
            reminder=i;
        }
        else
        {
            mvprintw( i+2, 0, "%s %s", SpisPracownikow[i].getName(), SpisPracownikow[i].getSurname());
        }
    }
    attron( A_REVERSE );
    mvprintw(reminder + 2, 0, "%s %s", SpisPracownikow[reminder].getName(), SpisPracownikow[reminder].getSurname());
    attroff( A_REVERSE );
    workerint = reminder;
}

void WorkersTool::show_group()
{
    redraw();
     mvprintw( 0, 30, "Podejrzyj grupe"); 
    groupint = 0;
    possible=true;
    possible2=true;
    possible5=true;
    mvprintw( 1, 30, "Wybierz grupe");
    for(int i = 0; i < counter1+1; i++)
    {
        if(groupint == i)
        {
            reminder = i;
        }
        else
        {
            mvprintw( i+1, 30, "%s", SpisGrup[i].getName());
        }
        
    }
    attron( A_REVERSE );
    mvprintw(reminder + 1, 30, "%s", SpisGrup[reminder].getName());
    attroff( A_REVERSE );

}

void WorkersTool::show_worker_groups()
{
    mvprintw(0, 0, "Podejrzyj grupy pracownika");
    groupint = 0;
    possible=true;
    possible1=true;
    possible6=true;
    mvprintw( 1, 0, "Wybierz pracownika");
    for(int i = 0; i < counter; i++)
    {
        if( groupint == i)
        {
            reminder=i;
        }
        else
        {
            mvprintw( i+1, 0, "%s %s", SpisPracownikow[i].getName(), SpisPracownikow[i].getSurname());
        }
    }
    attron( A_REVERSE );
    mvprintw(reminder + 1, 0, "%s %s", SpisPracownikow[reminder].getName(), SpisPracownikow[reminder].getSurname());
    attroff( A_REVERSE );
    workerint = reminder;
}

void WorkersTool::show_all()
{
        redraw();
    mvprintw(0, 30, "Podejrzyj wszystkie grupy");
    for (int i = 0; i < counter1 + 1; i++)
    {
        SpisGrup[i].ShowGroup(i);
    }
}

void WorkersTool::save_quit()
{
    redraw();
    mvprintw(0, 0, "Wyszedles z programu, aby powrocic nacisnij <-");
    plik.close();

    ofstream plik1(field, ofstream::trunc);
    if(!plik1.good())
        ofstream plik1(file, ofstream::trunc);
    for (int i = 0; i < counter1 + 1; i++)
        SpisGrup[i].Save(plik1);
    plik1.close();
    groupint=0;
    workerint=0;
    counter=0; 
    counter1=-1;
    mvprintw( 1, 0, "All saved");
}