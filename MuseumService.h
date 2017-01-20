/* Fonctions de gestion des données liées aux oeuvres et variables global */

// Recupere toutes les oeuvres d'art d'un musée. Format du tableau : N, Nom, Desc, Path
void getMuseumArtWorkIdForCurrentMuseum(int & artWorkID, int museeID, char dataBasePath[]);
void getMuseumArtWorkByID(char ***& museumArtWorksTab, int museumID, char dataBasePath[]);

// Recuperer les données sur les musées
int getMuseumQty(char dataBasePath[]);
char * getMuseumName(int museumID, char dataBasePath[]);