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
			n = n->dodaj_slovo(str[i],broj_reci,broj_cvorova,i);
		}
		n->dodaj_slovo('*',broj_reci,broj_cvorova,i);
		n->dodaj_u_list(str);
	}

	void print()const {
		cout <<"\nBroj reci u stablu: "<< broj_reci<<'\n';
		cout <<"Broj cvorova u stablu: "<< broj_cvorova<<'\n';
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
				char *word1 = const_cast<char*>(word);
				for (int j=0; j < strlen(word1); j++) {
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