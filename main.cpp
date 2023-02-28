#include <iostream>
#include<string>
#include <direct.h>
#include <queue>
using namespace std;

#ifndef _node_h_
#define _node_h_

class Node {
	struct elem {
		char* word;
		char c;
		int frek;
		elem* next;
		Node* son;
	};

	elem* novi_elem(char c) {
		elem* e = new elem;
		e->c = c;
		e->frek = 1;
		e->word = nullptr;
		e->next = nullptr;
		e->son = nullptr;
		return e;
	}
	elem* first;
	int level;
	//-----------------------------------------------------------------------------------------------------------------------------------
public:
	friend class Stablo;

	Node() {
		first = nullptr;
	}

	Node* dodaj_slovo(char c, int& rec, int& cvor, int lv) {
		this->level = lv;
		if (!first) {
			first = novi_elem(c);
			first->son = new Node;
			if (c != '*') cvor = cvor + 1;
			if (c == '*') rec = rec + 1;
			return first->son;
		}
		else {
			elem* p = nullptr, * e = first;
			while (e) {
				if (e->c == c) {
					e->frek++;
					//elem* temp = first;
					//ZAMENA MESTA
					/*
					while (temp!=e && temp->frek > e->frek) temp = temp->next;
					if (temp->frek < e->frek) {
						int t1 = e->frek;
						char t2 = e->c;
						Node* t3 = e->son;
						e->frek = temp->frek;
						e->c = temp->c;
						e->son = temp->son;
						temp->frek = t1;
						temp->c = t2;
						temp->son = t3;
						return temp->son;
					}
					*/
					return e->son;
				}
				p = e;
				e = e->next;
			}
			p->next = novi_elem(c);
			p->next->son = new Node;
			if (c == '*') rec = rec + 1;
			return p->next->son;
		}
		return nullptr;
	}

	void dodaj_u_list(char* rec) {
		elem* n = this->first;
		while (n) {
			if (n->c == '*') {
				n->word = new char[strlen(rec)];
				for (int i = 0; i < strlen(rec); i++) {
					n->word[i] = rec[i];
				}
				n->word[strlen(rec)] = '\0';
				return;
			}
			n = n->next;
		}
	}
	bool is_close(char a, char b) {
		char matrix[26][6] = { {'a','q','w','s','z','a'},{'b','v','n','h','b','b'},{'c','v','f','x','c','c',},{'d','e','r','s','x','f'},
		{'e','w','r','s','d','e'},{'f','r','t','g','c','d'},{'g','t','y','h','v','f'},{'h','y','u','j','b','g'},{'i','o','k','j','u','i'},
		{'j','k','n','h','u','i'},{'k','l','m','j','i','o'},{'l','k','m','o','p','l'},{'m','n','k','m','m','m'},{'n','m','j','b','n','n'},
		{'o','p','i','k','l','o'},{'p','o','l','p','p','p'},{'q','w','a','q','q','q'},{'r','e','t','f','d','r'},{'s','w','e','d','a','z'},
		{'t','r','y','f','g','t'},{'u','i','y','h','j','u'},{'v','c','b','g','v','v'},{'w','q','e','s','w','w'},{'x','z','d','c','x','x'},
		{'y','t','u','g','h','y'},{'z','x','s','z','z','z'} };
		if (a == '\0') return true;
		for (int i = 0; i < 6; i++) {
			if (matrix[a - 'a'][i] == b) return true;
		}
		return false;
	}

	void pronadji(char* rec) {
		Node* n = this;
		char r1[30] = " ", r2[30] = " ", r3[30] = " ";
		int f1 = 0, f2 = 0, f3 = 0;
		int raz1 = strlen(rec), raz2 = strlen(rec), raz3 = strlen(rec);
		queue<Node*> red;
		red.push(n);
		while (!red.empty()) {
			n = red.front();
			red.pop();
			elem* e = n->first;
			while (e) {
				if (e->son && (is_close(e->c, rec[n->level]) || n->level >= strlen(rec))) red.push(e->son);	// && is_close(e->c,rec[n->level])
//ako sam dosla do kraja reci
				if (e->c == '*') {
					if (e->word) {
						if (strlen(e->word) >= strlen(rec)) {
							int a = 0;
							for (int i = 0; i < strlen(rec); i++) {
								if (rec[i] != e->word[i]) a++;
							}
							if (a > 3) break;
							if (a < raz1 || (a == raz1 && e->frek > f1)) {
								strcpy(r3, r2); f3 = f2; raz3 = raz2;
								strcpy(r2, r1);	f2 = f1; raz2 = raz1;
								strcpy(r1, e->word); f1 = e->frek; raz1 = a;
								break;
							}
							if (a < raz2 || (a == raz2 && e->frek > f2)) {
								strcpy(r3, r2); f3 = f2; raz3 = raz2;
								strcpy(r2, e->word); f2 = e->frek; raz2 = a;
								break;
							}
							if (a < raz3 || (a == raz3 && e->frek > f3)) {
								strcpy(r3, e->word); f3 = e->frek; raz3 = a;
								break;
							}
						}
					}
				}
				e = e->next;
			}

		}
		if (r1[0] == ' ' || r2[0] == ' ' || r3[0] == ' ') cout << "\nNe postoje reci koje se uklapaju\n";
		printf("%s %s %s", r1, r2, r3);
	}
};

#endif 

//****************************************************************************************************************************************************
#include "Node.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <ctype.h>

using namespace std;
namespace fs = std::filesystem;

#ifndef _stablo_h_
#define _stablo_h_

class Stablo {
	int broj_reci;			//broj razlicitih reci koje se nalaze u stablu
	int broj_cvorova;		//ukupan broj cvorova koje sadrzi jedno stablo
	Node* root;				//pokazivac na koren stabla

public:
	Stablo() {
		broj_cvorova = 0;
		broj_reci = 0;
		root = new Node;
	}
	void dodaj_rec(char* str) {
		Node* n = root;
		int i;
		for (i = 0; i < strlen(str); i++) {
			n = n->dodaj_slovo(str[i], broj_reci, broj_cvorova, i);
		}
		n->dodaj_slovo('*', broj_reci, broj_cvorova, i);
		n->dodaj_u_list(str);
	}

	void print()const {
		cout << "\nBroj reci u stablu: " << broj_reci << '\n';
		cout << "Broj cvorova u stablu: " << broj_cvorova << '\n';
	}

	void ucitaj1() {
		cout << "Zapocinje ucitavanje reci u stablo!\n";
		ifstream file;
		const char* word; string s;
		bool f;
		string path = "text";
		for (const auto& entry : fs::directory_iterator(path)) {
			file.open(entry.path());
			//cout << entry.path();
			if (!file.is_open()) return;
			while (file >> s) {
				f = true;
				word = s.c_str();
				char* word1 = const_cast<char*>(word);
				for (int j = 0; j < strlen(word1); j++) {
					if (!isalpha(word1[j])) {
						f = false;
						break;
					}
					word1[j] = word[j];
					if (word1[j] <= 'Z' && word1[j] >= 'A') word1[j] = word1[j] - ('Z' - 'z');
				}
				if (f) this->dodaj_rec(word1);
			}
			file.close();
		}
		cout << "Reci uspesno ucitane!\n";
	}

	void pronadji_rec(char* rec) {
		root->pronadji(rec);
	}

};

#endif // !_stablo_h_

//******************************************************************************************************************************************************
#include"Stablo.h"
#include "Node.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "dirent.h"

using namespace std;

int main() {
	Stablo s;
	s.ucitaj1();
	s.print();
	while (true) {
		cout << "\nOdaberite jednu od narednih opcija:\n"
			<< "1.Pretrazi rec\n"
			<< "2.Kraj!\n";
		int n;
		cin >> n;
		if (n == 1) {
			char rec[30];
			cout << "Unesite rec:  ";
			cin >> rec;
			s.pronadji_rec(rec);
		}
		if (n == 2) {
			break;
		}
	}
	cout << "\nKRAJ PROGRAMA, DOVIDJENJA!\n";
}