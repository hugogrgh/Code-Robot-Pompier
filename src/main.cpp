#include <Arduino.h>
#include "AutomForArduino.h"

// Définition des broches
#define BUZZER_PIN 6
#define DUREE_FREQ 1000
#define FLAME_SENSOR_PIN A1
#define LED_PIN 11
#define LED_PIN3 8

#define FREQ1 435
#define FREQ2 488
#define PAS 10
#define DELAI 5
#define PAUSE 1000

// Moteur gauche
#define MG26 2
#define MG27 3
#define MLIG3 12
// Moteur droit
#define MD52 4
#define MD53 5
#define MLID5 13
// Actionneur moteur
#define ACTIONNEUR 9

Clignoteur buzz(1000);

int valeur = 0;          // Intensité actuelle (0-255)
bool monte = true;       // Sens du gradient (montée/descente)
bool enPause = false;    // Indique si la LED est en pause (cycle terminé)
unsigned long dernierTemps = 0;
unsigned long debutPause = 0;

float pourcentageFlamme;
unsigned long dernierChangement = 0;
bool etatFreq = false; // false = FREQ1, true = FREQ2
bool flammeDetectee=false;

// Mise à l'échelle : valeur brute (0-1023) vers pourcentage (0-100)
MiseAEchelle miseAEchelleFlamme(0, 1023, 0, 100);

// Temporisation à la montée de 2 secondes (2000 ms)
TemporisationRetardMontee tempoFlamme(10);
TemporisationRetardMontee demitour(2750);
TemporisationRetardMontee recul(2000);

unsigned long previousMillis = 0;
const unsigned long interval = 100; // période d'affichage en ms


// Variables d'état
int etatCourant, etatFutur;
bool frontEtape;
// Variables internes
bool fms0, fms1;
bool s0, s1;

void lectureEntrees(void);
void evolutionGrafcets(void);
void actions(void);

/***Différentes fonctions utilisées***/

void buzzer(void);
void GIRO(void);
void AVANCER(void);
void STOP(void);
void DEMIT(void);
void RECULER(void);
void flammeAc(void);

void setup() {
  pinMode(MG26, OUTPUT);
  pinMode(MG27, OUTPUT);
  pinMode(MLIG3, OUTPUT);
  pinMode(MD52, OUTPUT);
  pinMode(MD53, OUTPUT);
  pinMode(MLID5, OUTPUT);

  pinMode(53,INPUT);
  pinMode(51,INPUT);
  pinMode(49,INPUT_PULLUP);
  pinMode(A0,INPUT);
  pinMode(ACTIONNEUR,OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  flammeAc();
  STOP();
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_PIN3, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(ACTIONNEUR, LOW);
  etatCourant = etatFutur = 0;
  Serial.begin(9600);
  Serial.println("Fin initialisation");
}

void loop() {
  lectureEntrees();
  evolutionGrafcets();
  actions();
  flammeAc();
}

/***utilisations des fronts montant***/

void lectureEntrees()
{
  // Détection des fronts montants pour S0,
  fms0 = (digitalRead(53) && !s0);
  s0 = digitalRead(53); 
// TODO: Ajouter les autres fronts montants pour S1 et S2,
  fms1 = (digitalRead(51) && !s1);
  s1 = digitalRead(51);  

}

void evolutionGrafcets()
{
  // TODO: Implémenter les transitions du grafcet
  // Exemple: Si état 0 et S1 pressé, passer à état 1
  if (etatCourant==0 && fms1 == 1) etatFutur = 1;
  if (etatCourant==1 && flammeDetectee) etatFutur = 2;
  if (etatCourant==2 && (pourcentageFlamme>50) && (pourcentageFlamme<90)) etatFutur = 3;
  if (etatCourant==3 && (pourcentageFlamme>98)) etatFutur = 4; 
  if (etatCourant==4 && (pourcentageFlamme<10)) etatFutur = 5; 
  if (etatCourant==5 && (recul.getSortie()==1)) etatFutur = 6; 
  if (etatCourant==6 && (demitour.getSortie()==1)) etatFutur = 7; 

  if (fms0 == 1) etatFutur = 0;
  
  frontEtape = (etatCourant != etatFutur);
  if (frontEtape) {
    Serial.print("etat=");
    Serial.println(etatFutur);
  }
  etatCourant = etatFutur;
}

void actions()
{
  // Exemple: Allumer H1 à l'état 1
  if (etatCourant==0) {STOP();digitalWrite(BUZZER_PIN,LOW);}
  if (etatCourant==1) {GIRO();buzzer();AVANCER();}
  if (etatCourant==2) {GIRO();buzzer();AVANCER();}
  if (etatCourant==3) {GIRO();buzzer();AVANCER();}
  if (etatCourant==4) {GIRO();buzzer();STOP();digitalWrite(ACTIONNEUR,HIGH);}
  if (etatCourant==5) {RECULER();digitalWrite(ACTIONNEUR,LOW);recul.activation();digitalWrite(BUZZER_PIN,LOW);}
  if (etatCourant==6) {DEMIT();demitour.activation();}
  if (etatCourant==7) AVANCER();

}

/***Detection de la flamme***/

void flammeAc() {

  float flamme;
  // Lecture du capteur
  int valeurBrute = analogRead(FLAME_SENSOR_PIN);
  flamme = miseAEchelleFlamme.convertir(valeurBrute);
  pourcentageFlamme=100-flamme;

  // Affichage toutes les 100 ms
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("Valeur brute : ");
    Serial.print(valeurBrute);
    Serial.print(" | Pourcentage flamme : ");
    Serial.println(pourcentageFlamme);
  }

  // Seuil de détection (à ajuster selon calibration)
  flammeDetectee = (pourcentageFlamme > 30);

  // Activation de la temporisation à la montée

  // La LED s'allume seulement si la flamme est détectée de façon continue pendant 2 secondes
  if (tempoFlamme.getSortie()) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
 
}


void buzzer() {

  if (buzz.statut()==1)
  {
    tone(BUZZER_PIN, 435);
  }
  
 if (buzz.statut()==0)
  {
    tone(BUZZER_PIN, 488);
  }
  
}

void GIRO() {
  unsigned long maintenant = millis();
  if (enPause) {
    if (maintenant - debutPause >= PAUSE) {
      enPause = false;
      valeur = 0;
      monte = true;
      dernierTemps = maintenant;
    } else {
      analogWrite(LED_PIN3, 0);
    }
    return;
  }
  if (maintenant - dernierTemps >= DELAI) {
    if (monte) {
      valeur += PAS;
      if (valeur >= 255) {
        valeur = 255; monte = false;
      }
    } else {
      valeur -= PAS;
      if (valeur <= 0) {
        valeur = 0; enPause = true; debutPause = maintenant;
      }
    }
    analogWrite(LED_PIN3, valeur);
    dernierTemps = maintenant;
  }
}

void AVANCER() {
  digitalWrite(MLIG3, HIGH);
  digitalWrite(MG26, HIGH);
  digitalWrite(MG27, LOW);

  digitalWrite(MLID5, HIGH);
  digitalWrite(MD52, HIGH);
  digitalWrite(MD53, LOW);
}

void RECULER() {
  digitalWrite(MLIG3, HIGH);
  digitalWrite(MG26, LOW);
  digitalWrite(MG27, HIGH);

  digitalWrite(MLID5, HIGH);
  digitalWrite(MD52, LOW);
  digitalWrite(MD53, HIGH);
}

void STOP() {
  digitalWrite(MLIG3, LOW);
  digitalWrite(MG26, LOW);
  digitalWrite(MG27, LOW);

  digitalWrite(MLID5, LOW);
  digitalWrite(MD52, LOW);
  digitalWrite(MD53, LOW);
}

void DEMIT() {
  digitalWrite(MLIG3, HIGH);
  digitalWrite(MG26, HIGH);
  digitalWrite(MG27, LOW);

  digitalWrite(MLID5, HIGH);
  digitalWrite(MD52, LOW);
  digitalWrite(MD53, HIGH);
}
