/* Fonctions de gestion des donn�es li�es aux oeuvres et variables global */

// Recupere toutes les oeuvres d'art d'un mus�e. Format du tableau : N, Nom, Desc, Path
void getMuseumArtWorkIdForCurrentMuseum(int & artWorkID, int museeID, char dataBasePath[]);
void getMuseumArtWorkByID(char ***& museumArtWorksTab, int museumID, char dataBasePath[]);

// Recuperer les donn�es sur les mus�es
int getMuseumQty(char dataBasePath[]);
char * getMuseumName(int museumID, char dataBasePath[]);