/*************************************************

  Fonctions pour l'automatismes sous Arduino

*/

#ifndef AutomForArduino_h
#define AutomForArduino_h

#include <arduino.h>


class TemporisationRetardMontee
{

    // methodes
  public :
    TemporisationRetardMontee(unsigned long);
    void activation();
    bool getSortie();
	  unsigned long getTempsEcoule();

    // Attributs
  private:
    unsigned long duree;
    unsigned long debut;
    unsigned long tempsEcoule;
	  bool captureTemps;
    bool sortie;

};

class TemporisationRetardDescente
{

    // methodes
  public :
    TemporisationRetardDescente(unsigned long);
    void activation();
    bool getSortie();
    unsigned long getTempsEcoule();

    // Attributs
  private:
    unsigned long duree;
    unsigned long debut;
    unsigned long tempsEcoule;
    bool captureTemps;
    bool sortie;

};

class Clignoteur
{

    // methodes
  public :
    Clignoteur(int);
    bool statut();

    // Attributs
  private:
    int baseDeTemps;

};

class Compteur
{

    // methodes
  public :
	  Compteur(int);
    void incrementer();
    void decrementer();
	  void remettreAZero();
	  int getValeurCourante();
	  bool getSortie();

    // Attributs
  private:
    int valeur;
    int valeurPreselection;
    
};

class MiseAEchelle
{

    // methodes
  public :
    MiseAEchelle(float,float,float,float);
	float convertir(float);

    // Attributs
  private:
    float minEntree;
    float minSortie;
    float maxEntree;
	float maxSortie;

};

#endif
