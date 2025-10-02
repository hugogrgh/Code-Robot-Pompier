#include "AutomForArduino.h"

/********************************************************
****  TEMPORISATION RETARD A LA MONTEE ******************
*  La sortie passe à 1 au bout de la tempo **************
*********************************************************/

// Contructeur qui prend la duree souhaitee de la temporisation
TemporisationRetardMontee :: TemporisationRetardMontee(unsigned long duree)
{
  this->duree = duree;
  sortie = false;
  captureTemps = false;
}

// Activation de la temporisation. Doit etre fait tout le temps de la duree de la tempo
void TemporisationRetardMontee :: activation()
{
  // Capture du temps de reference
  if(!captureTemps)
  {
	  debut = millis();
	  captureTemps = true;
  }
  // Calcul du temps ecoule depuis le temps de reference
  tempsEcoule = millis() - debut;
  // Mise a 1 de la fin de tempo
  if (tempsEcoule >= duree)
  {
    sortie = true;
	  captureTemps = false;
  }
  else
  {
    sortie = false;
  }
}

// Interrogration du bit de fin de tempo
bool TemporisationRetardMontee :: getSortie()
{
	return(sortie);
}

// Recuperation du temps ecoule depuis le debut si necessaireint
unsigned long TemporisationRetardMontee :: getTempsEcoule()
{
	return(tempsEcoule);
}

/********************************************************
****  TEMPORISATION RETARD A LA DESCENTE ******************
*  La sortie passe à 0 au bout de la tempo **************
*********************************************************/

// Contructeur qui prend la duree souhaitee de la temporisation
TemporisationRetardDescente :: TemporisationRetardDescente(unsigned long duree)
{
  this->duree = duree;
  sortie = false;
  captureTemps = false;
}

// Activation de la temporisation. Doit etre fait tout le temps de la duree de la tempo
void TemporisationRetardDescente :: activation()
{
  // Capture du temps de reference
  if(!captureTemps)
  {
    debut = millis();
    captureTemps = true;
    sortie = true;
  }
  // Calcul du temps ecoule depuis le temps de reference
  tempsEcoule = millis() - debut;
  // Mise a 1 de la fin de tempo
  if (tempsEcoule >= duree)
  {
    sortie = false;
    captureTemps = false;
  }
}

// Interrogration du bit de fin de tempo
bool TemporisationRetardDescente :: getSortie()
{
  return(sortie);
}

// Recuperation du temps ecoule depuis le debut si necessaireint
unsigned long TemporisationRetardDescente :: getTempsEcoule()
{
  return(tempsEcoule);
}

/********************************************************
****  CLIGNOTEUR *************************************
*********************************************************/

// Construteur qui prend en parametre le temps haut ou bas souhaitee
Clignoteur :: Clignoteur(int baseDeTemps)
{
  this->baseDeTemps = baseDeTemps;
}

// Fonction qui renvoie true si le clignoteur est à l'état haut et false s'il est à l'état bas
bool Clignoteur :: statut()
{
  return ((millis() / baseDeTemps) % 2 == 1);
}

/********************************************************
****  COMPTEUR *************************************
**** ATTENTION : Il faut gerer le front montant dans le programme
*********************************************************/
// Constructeur qui prend en parametre la valeur de preselection
Compteur ::	Compteur(int valeurPreselection)
{
	this->valeurPreselection = valeurPreselection;
	valeur = 0;
}

// Incrementation du compteur
void Compteur :: incrementer()
{
	valeur++;
}

// Decrementation du compteur
void Compteur :: decrementer()
{
	valeur--;
}

// remise a zero du compteur
void Compteur :: remettreAZero()
{
	valeur = 0;
}

// recuperation de la valeur courante
int Compteur :: getValeurCourante()
{
	return(valeur);
}

// est-ce que la preselection est atteinte (sortie Q compteur Siemens ou Schnieder)
bool Compteur :: getSortie()
{
	return(valeur == valeurPreselection);
}

/********************************************************
****  MISE A L'ECHELLE DE VALEUR ************************
*********************************************************/

// Constructeur qui ne prend en parametre la plage d'entree et la plage de sortie
MiseAEchelle :: MiseAEchelle(float minEntree,float maxEntree,float minSortie,float maxSortie)
{
	this->minEntree = minEntree;
	this->maxEntree = maxEntree;
	this->minSortie = minSortie;
	this->maxSortie = maxSortie;
}

// fonction de conversion qui prend la valeur a convertir et renvoie la valeur convertie
float MiseAEchelle :: convertir(float valeurAConvertir)
{
	if(valeurAConvertir >= minEntree && valeurAConvertir <= maxEntree)
	{
		float norm = (1 / (maxEntree - minEntree)) * (valeurAConvertir - minEntree);
		float scale = (maxSortie - minSortie) * norm + minSortie;
		return(scale);
	}
	return(-1000);
}
