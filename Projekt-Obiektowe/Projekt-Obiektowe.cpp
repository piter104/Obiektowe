// Projekt-Obiektowe.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include "shared.hpp"

using namespace std;

class WorkersTool
{
public:
	string nazwa;
	string field, value;

	void init(string name)
	{
		this->nazwa = name;
	}

	void setEntry(string field, string value)
	{
		this->value = value;
		this->field = field;
	}

	string getEntry(string field)
	{

	}

	void redraw()
	{
		
	}
};


class Pracownik
{
public:
	string imie;
	string nazwisko;
	int wiek;
	string grupy[20];
	int counter = 1;

	void init(string name, string surname, int age)
	{
		imie = name;
		nazwisko = surname;
		wiek = age;
	}

	void worker_groups()
	{
		cout << "Worker is in groups: " << endl;
		for (int i = 0; i < counter; i++)
			cout << i << ". " << grupy[i] << endl;
	}
};

class lista
{
public:
	Pracownik member;
	lista* next=nullptr;
};

class Grupa
{
public:
	string name;
	lista* previous = new lista;
	lista* actual = new lista;
	lista* current = new lista;
	lista* first = new lista;
	bool InitialWorker=false;



	void init(string name)
	{
		this->name = name;
	}

	void AddWorker(Pracownik worker)
	{
		if (!InitialWorker)
		{
			InitialWorker = true;
			lista* tmp = new lista;
			tmp->member = worker;
			tmp->member.grupy[tmp->member.counter++] = this->name;
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
			tmp->member.grupy[tmp->member.counter++] = this->name;
			tmp->next = nullptr;
			current = tmp;
		}
	}

	Pracownik FindWorker(string name, string surname)
	{
		lista* tmp = first;
		while (tmp)
		{
			if (name.compare(tmp->member.imie) == 0 && surname.compare(tmp->member.nazwisko) == 0)
			{
				actual = tmp;
				return tmp->member;
				break;
			}
			previous = tmp;
			tmp = tmp->next;
		}
		//cout << "Nie znaleziono pracownika" << endl;
		return previous->member;
	}

	bool IsIn(string name, string surname)
	{
		lista* tmp = first;
		while (tmp)
		{
			if (name.compare(tmp->member.imie) == 0 && surname.compare(tmp->member.nazwisko) == 0)
			{
				return true;
				break;
			}
			tmp = tmp->next;
		}
		return false;
	}

	void DeleteWorker(Pracownik tmp)
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

	void ShowGroup()
	{
		int counter=0;
		lista* tmp = first;
		cout << "Group " << name << endl;
		while (tmp)
		{
			cout << 1+counter++<< ". " << tmp->member.imie << " "<< tmp->member.nazwisko << endl;
			tmp = tmp->next;
		}
	}

	void Save(ofstream& plik1)
	{
		lista* tmp = first;
		plik1 << "Group " << name << endl;
		while (tmp)
		{
			plik1<< tmp->member.imie << " " << tmp->member.nazwisko << " " << tmp->member.wiek << endl;
			tmp = tmp->next;
		}
	}
};




int main()
{

	/*int choice;
    cout << "Choose Backend [1-4]: ";
	while (1)
	{
		cin >> choice;
		if (choice <= 4 && choice >= 1)
			break;
	}
	switch (choice)
	{
	case 1:
		//Backend mybackend;
		break;
	case 2:
		//Backend mybackend;
		break;
	case 3:
		//Backend mybackend;
		break;
	case 4:
		//Backend mybackend;
		break;
	}*/
	WorkersTool mytool;
	//mybackend.tool = &mytool;
	//mytool.backend = &mybackend;
	//mybackend.start();
	mytool.init("nazwa");

	Pracownik SpisPracownikow[50];
	Grupa SpisGrup[10];

	int counter = 0;
	int counter1 = 0;
	int counter2 = 0;

	bool looper = true;
	string imie, nazwisko;
	Pracownik tmp;



	string name, surname;
	int age, workerint, groupint;

	fstream plik;
	fstream plik1;
	string temp,  napis = "Group";

	SpisGrup[0].init("All_Workers");
	plik.open("dane123.txt", ios::in);
	if (plik.good() == true)
	{
		cout << "Uzyskano dostep do pliku!" << endl;
		while (!plik.eof())
		{
			plik >> temp;
			if (temp == napis)
			{
				plik >> temp;
				SpisGrup[++counter1].name = temp;
				cout << temp << endl;

			}
			else
			{
				plik >> nazwisko;
				plik >> age;
				cout << temp << " " << nazwisko << " " << age <<endl;
				SpisPracownikow[counter].init(temp, nazwisko, age);
				//if(!SpisGrup[0].IsIn(temp, nazwisko))
					//SpisGrup[0].AddWorker(SpisPracownikow[counter]);
				SpisGrup[counter1].AddWorker(SpisPracownikow[counter]);
				counter++;
			}
		}
	}
	else
		cout << "Dostep do pliku zostal zabroniony!" << endl;


	cout << endl << endl;
	int decision = 0;
	cout << "Choose what to do [1-4]" << endl << "1. Create New Worker" << endl << "2. Show All Workers" << endl << "3. Create New Group" << endl << "4. Add Worker To Group" << endl << "5. Delete worker" << endl << "6. Show Group" << endl << "7. Show Worker Groups" << "8. Show All Groups" << endl << "9. Save and Quit" << endl;
	while (looper)
	{
		cin >> decision;
		switch (decision)
		{
		case 1:
			if (counter < 50)
			{
				cout << "Podaj imie: " << endl;
				cin >> name;
				cout << "Podaj nazwisko: " << endl;
				cin >> surname;
				cout << "Podaj wiek: " << endl;
				cin >> age;
				if (SpisGrup[0].IsIn(name, surname))
				{
					cout << "Pracownik juz istnieje" << endl;
					break;
				}
				SpisPracownikow[counter].init(name, surname, age);
				SpisGrup[0].AddWorker(SpisPracownikow[counter]);
				cout << "Pracownik dodany: " << SpisPracownikow[counter].imie << " " << SpisPracownikow[counter].nazwisko << " " << SpisPracownikow[counter].wiek << endl;
				counter++;
			}
			break;
		case 2:
			SpisGrup[0].ShowGroup();
			break;
		case 3:
			cout << "Podaj nazwe grupy: " << endl;
			cin >> SpisGrup[++counter1].name;
			cout << "Grupa utworzona: " << SpisGrup[counter1].name << endl;
			counter1++;
			break;
		case 4:
			cout << "Podaj numer pracownika: " << endl;
			cin >> workerint;
			cout << "Podaj numer grupy: " << endl;
			cin >> groupint;
			if (SpisGrup[groupint].IsIn(SpisPracownikow[workerint-1].imie, SpisPracownikow[workerint-1].nazwisko))
			{
				cout << "Pracownik juz istnieje w tej grupie" << endl;
				break;
			}
			SpisGrup[groupint].AddWorker(SpisPracownikow[workerint - 1]);
			SpisGrup[groupint].ShowGroup();
			break;
		case 5:
			cout << "Podaj imie pracownika: " << endl;
			cin >> imie;
			cout << "Podaj nazwisko pracownika: " << endl;
			cin >> nazwisko;
			for (int i = 0; i < counter1 + 1; i++)
			{
				tmp = SpisGrup[i].FindWorker(imie, nazwisko);
				if (SpisGrup[i].IsIn(imie, nazwisko) == false)
					continue;
				SpisGrup[i].DeleteWorker(tmp);
			}
			break;
		case 6:
			cout << "Podaj numer grupy: " << endl;
			cin >> groupint;
			SpisGrup[groupint].ShowGroup();
			break;
		case 7:
			cout << "Podaj numer pracownika: " << endl;
			cin >> groupint;
			SpisPracownikow[groupint].worker_groups();
			break;
		case 8:
			for (int i = 0; i < counter1 + 1; i++)
			{
				SpisGrup[i].ShowGroup();
			}
			break;
		case 9:
			plik.close();
			ofstream plik1("dane123.txt");
			for (int i = 0; i < counter1 + 1; i++)
				SpisGrup[i].Save(plik1);
			plik1.close();
			cout << "All Saved" << endl;
			looper = false;
			break;
		}	
	}
	return 0;
}
