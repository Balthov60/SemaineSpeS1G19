#include <iostream>
#include <sstream>
#include "fonctionsBD.h"
#include "sqlite3.h"

#include "MuseumService.h"

using namespace std;

// Renvoi un tableau d'oeuvre d'art : id, nom, descriptif, path
void getMuseumArtWorkByID(char ***& artWorkInformations, int workArtID, char dataBasePath[]) {
	ostringstream req;
	int line, column;

	req.str("");
	req << "select * from tableaux where idTableau = " << workArtID;

	sqlite3 * dataBase;
	dataBase = bd_ouvrir(dataBasePath);
	bd_requeteSelect(dataBase, req.str().c_str(), artWorkInformations, line, column);
	bd_fermer(dataBase);
	
	cout << "DEBUG : SQL Request. line = " << line << " column = " << column << endl;
	for (int i = 0; i < line; i++) {
		for (int j = 0; j < column; j++)
			cout << artWorkInformations[i][j] << " | ";
		cout << endl;
	}

	bd_fermer(dataBase);
	req.str("");
}

void getMuseumArtWorkIdForCurrentMuseum(int & artWorkID, int museumID, char dataBasePath[]) {
	ostringstream req;
	int line, column;
	char *** resultTab;

	req.str("");
	req << "select idTableau from association where numero = " << artWorkID << " and idMusee = " << museumID;

	sqlite3 * dataBase;
	dataBase = bd_ouvrir(dataBasePath);
	bd_requeteSelect(dataBase, req.str().c_str(), resultTab, line, column);
	bd_fermer(dataBase);

	cout << "DEBUG : SQL Request. line = " << line << " column = " << column << endl;
	for (int i = 0; i < line; i++) {
		for (int j = 0; j < column; j++)
			cout << resultTab[i][j] << " | ";
		cout << endl;
	}

	bd_fermer(dataBase);
	req.str("");

	artWorkID = resultTab[0][0][0] - 48;
}

int getMuseumQty(char dataBasePath[]) {
	ostringstream req;
	int line, column;
	char *** resultTab;

	req.str("");
	req << "SELECT count(id) from musees";

	sqlite3 * dataBase;
	dataBase = bd_ouvrir(dataBasePath);
	bd_requeteSelect(dataBase, req.str().c_str(), resultTab, line, column);
	bd_fermer(dataBase);

	cout << "DEBUG : SQL Request. line = " << line << " column = " << column << endl;
	for (int i = 0; i < line; i++) {
		for (int j = 0; j < column; j++)
			cout << resultTab[i][j] << " | ";
		cout << endl;
	}

	bd_fermer(dataBase);
	req.str("");

	return (int)resultTab[0][0][0] - 48;
}

char * getMuseumName(int museumID, char dataBasePath[]) {
	ostringstream req;
	int line, column;
	char *** resultTab;

	req.str("");
	req << "SELECT nom from musees where id = " << museumID;

	sqlite3 * dataBase;
	dataBase = bd_ouvrir(dataBasePath);
	bd_requeteSelect(dataBase, req.str().c_str(), resultTab, line, column);
	bd_fermer(dataBase);

	cout << "DEBUG : SQL Request. line = " << line << " column = " << column << endl;
	for (int i = 0; i < line; i++) {
		for (int j = 0; j < column; j++)
			cout << resultTab[i][j] << " | ";
		cout << endl;
	}

	bd_fermer(dataBase);
	req.str("");

	return resultTab[0][0];
}