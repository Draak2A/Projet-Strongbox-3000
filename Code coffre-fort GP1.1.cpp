const int ledR1 = 2;    
const int ledR2 = 3;
const int ledR3 = 4;
const int ledR4 = 5;
const int ledV = 6;
const int btn = 7;

const int nombre_agent = 16;    // On defini le nombre d'agent
int etape = 0;  // Permet au programme d'exécuter seulement certaines actions dans la fonction loop()
String lettre_agent;    // Correspond à la lettre d'agent saisi dans la console
String liste_lettre[nombre_agent] = {"A", "B", "C", "D", "E", "F", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q"};   // Tableau contenant toutes les lettre d'agent
int modele;     // Représente le modèle de carte inséré
int essai;  // On défini une variable essai afin de pouvoir bloquer le coffre au bout d'un certains nombre d'erreur
int erreur; // Va servir afin de savoir si une erreur viens d'être faites pendant l'authentification

void setup() {
    pinMode(ledR1, OUTPUT); // On initialise les différentes entrées et sorties
    pinMode(ledR2, OUTPUT);
    pinMode(ledR3, OUTPUT);
    pinMode(ledR4, OUTPUT);
    pinMode(ledV, OUTPUT);
  	pinMode(btn, INPUT);

    Serial.begin(9600); // La communication série est initialisé
    randomSeed(analogRead(1));  // Le générateur de nombre pseudo-aléatoire est initialisé au port A1 (car A0 est utilisé pour mesurer des tensions)
}

void loop() {
    if (etape == 0) {
        Serial.println("Bienvenu cher utilisateur. Veuillez sélectionner votre combinaison d'interrupteurs et introduisez votre carte.");
        etape ++;   // Le programme passe à l'étape suivante
    }
    else if (etape == 1) {
        tensionModele();    // La fonction tensionModele est appelée pour mesurer la tension de sortie et identifié le modèle de carte
        Serial.print("Modèle de carte identifié : ");
        Serial.println(modele);
        Serial.println("Veuillez vous identifier.");
        etape ++;
    }
    else if (etape == 2 && Serial.available() > 0) {    // On vérifie si la console est disponible
        lettre_agent = Serial.readString();     // On saisi notre lettre d'agent
        lettreModele();     // La fonction lettreModele est appellé pour vérifier si le modèle de carte et le modèle de carte inséré concorde
        if (erreur == 1) {  // En cas d'erreur, on ne passe pas à l'étape suivante
            erreur = 0;
            return;
        }
        etape ++;
    }
    else if (etape == 3) {
        if (modele == 1) {  // On appelle les différents niveaux de sécurité
            niveauSecurite5();
            if (erreur == 1) {  
                erreur = 0;
                return;
            }
            etape ++;
        }
        else if (modele == 2) {
            niveauSecurite4();
            if (erreur == 1) {
                erreur = 0;
                return;
            }
            etape ++;
        }
        else if (modele == 3) {
            niveauSecurite1();
            if (erreur == 1) {
                erreur = 0;
                return;
            }
            etape ++;
        }
        else if (modele == 4) {
            niveauSecurite2();
            if (erreur == 1) {
                erreur = 0;
                return;
            }
            etape ++;
        }
        else if (modele == 5) {
            niveauSecurite3();
            if (erreur == 1) {
                erreur = 0;
                return;
            }
            etape ++;
        }
        else if (modele == 6) {
            niveauSecurite5();
            if (erreur == 1) {
                erreur = 0;
                return;
            }
            etape ++;
        }
        else if (modele == 7) {
            niveauSecurite3();
            if (erreur == 1) {
                erreur = 0;
                return;
            }
            etape ++;
        }
        else if (modele == 8) {
            niveauSecurite4();
            if (erreur == 1) {
                erreur = 0;
                return;
            }
            etape ++;
        }
    }
    else if (etape == 4) {
        Serial.println("Ouverture du coffre...");
        delay(1000);
        digitalWrite(ledV, HIGH);   // On allume la LED verte une fois le coffre ouvert
        Serial.println("Coffre ouvert. N'oubliez pas de fermer le coffre une fois terminé.");
        etape ++;
    }
    else if (etape == 5) {
        if (digitalRead(btn) == HIGH) { // Si le bouton poussoir est pressé le coffre est fermé
            Serial.println("Fermeture du coffre...");
            delay(1000);
            digitalWrite(ledV, LOW);       // la LED verte s'éteind une fois le coffre fermé
            Serial.println("Coffre fermé, à bientôt.");
            modele = 0; // On retourne au début du programme
            etape = 0;
        }
    }
}

void niveauSecurite1() {    // On associe à chaque niveau de sécurité des mécanismes d'authentifications
    MA1();
    if (erreur == 1) {  // En cas d'erreur, on ne passe pas au mécanisme suivant
        erreur = 0;
        return;
    }
    MA3();
}

void niveauSecurite2() {
    MA1();
    if (erreur == 1) {
        erreur = 0;
        return;
    }
    MA4();
}

void niveauSecurite3() {
    MA2();
    if (erreur == 1) {
        erreur = 0;
        return;
    }
    MA5();
}

void niveauSecurite4() {
    MA2();
    if (erreur == 1) {
        erreur = 0;
        return;
    }
    MA3();
    if (erreur == 1) {
        erreur = 0;
        return;
    }
    MA4();
}

void niveauSecurite5() {
    MA1();
    if (erreur == 1) {
        erreur = 0;
        return;
    }
    MA2();
    if (erreur == 1) {
        erreur = 0;
        return;
    }
    MA3();
    if (erreur == 1) {
        erreur = 0;
        return;
    }
    MA5();
}

void tensionModele() {
    while (modele == 0) {   // La boucle est répété à l'infinie tant qu'un modèle de carte n'est pas identifié
        int valeur = analogRead(A0);    // On mesure la tension sur le port analogique A0
        float tension = valeur * (5.0 / 1023.0);    // On donne la tension en Volt
        delay(500);
        if (0.15 <= tension && tension <= 0.17) {   // On défini les intervalles de tension pour chaque modèle de carte
            modele = 1;     // On identifie le modèle de carte correspondant
            return; // On sort de la fonction et le programme continu
        }
        else if (0.56 <= tension && tension <= 0.58) {
            modele = 2;
            return;
        }
        else if (0.91 <= tension && tension <= 0.93) {
            modele = 3;
            return;
        }
        else if (1.05 <= tension && tension <= 1.07) {
            modele = 4;
            return;
        }
        else if (1.73 <= tension && tension <= 1.75) {
            modele = 5;
            return;
        }
        else if (2.00 <= tension && tension <= 2.02) {
            modele = 6;
            return;
        }
        else if (2.46 <= tension && tension <= 2.48) {
            modele = 7;
            return;
        }
        else if (2.84 <= tension && tension <= 2.86) {
            modele = 8;
            return;
        }
    }
}

void lettreModele(){
    String liste_lettre_modele[8][3] = {{"A", "B"}, {"C"}, {"D", "E", "F"}, {"H", "I", "J"}, {"K"}, {"L", "M", "N"}, {"O", "P"}, {"Q"}};    // Double liste contenant les lettres des agents qui sont regroupées par modèle de carte
    int concordance = 0;
    for (int i = 0; i < 2; i ++) {
        if (liste_lettre_modele[modele - 1][i] == lettre_agent) { 
            Serial.print("Bonjour agent ");
            Serial.print(lettre_agent);
            Serial.println(". Veuillez finaliser votre authentification en validant les prochains mécanismes d'authentification.");
            concordance = 1;
        }
    }
    if (concordance == 0) {
        error();
    }
}

void MA1() {
    int reponse;
    int etape1 = 0;  
    while (true) { // Permet de répéter la boucle tant que le mécanisme d'authentification n'est pas terminé par l'agent
        if (etape1 == 0) {
            Serial.println("Quel est le nom de code du coffre ?");
            delay(200);
            Serial.println("1. Pipboy 2000");
            delay(200);
            Serial.println("2. Bigbox 3000");
            delay(200);
            Serial.println("3. Strongbox 3000");
            delay(200);
            Serial.println("4. Strongbox 360");
            etape1 ++;
        }
        else if (etape1 == 1 && Serial.available() > 0) {   // On attend la réponse de l'agent
            reponse = Serial.parseInt();    // Récupère la réponse de l'agent dans la console
            etape1 ++;
        }
        else if (etape1 == 2 && reponse == 3) { // Vérifie si la réponse entrée est correcte
            Serial.println("Réponse correcte");
            return; // On sort de la fonction pour passer à la suivante
        }
        else if (etape1 == 2 && reponse != 3) { // Vérifie si la réponse entrée est incorrecte
            error();
          	return;
        }
    }
}

void MA2() {
    int cle_publique, code_dechiffre;
    int code, code_chiffre;
    int etape1 = 0;
    while (true) { 
        if(etape1 == 0) {
            code = random(0,2881);  // code prend une valeur entre 0 et 2880
            cle_publique = cleAgent(lettre_agent);  // cle_publique prend comme valeur le résultat retourné par la fonction cleAgent()
            code_chiffre = chiffrer(code, cle_publique);
            Serial.print("Message à déchiffrer :");
            Serial.println(code_chiffre);
            etape1 ++;
        }
        else if (etape1 == 1 && Serial.available() > 0) {
            code_dechiffre = Serial.parseInt();
            if(code == code_dechiffre){
                Serial.println("Code correct");
                return;
            }
            else {
                error();
              	return;
            }
        }
    }
}

int cleAgent(String lettre_agent) {
    int cle_publique, i;
    int clePublic[nombre_agent] = {601, 619, 631, 641, 647, 653, 661, 673, 691, 701, 733, 739, 751, 797, 809, 811}; // Tableau contenant toute les clé publique
    for(i = 0; i < nombre_agent; i++) {
        if(liste_lettre[i] == lettre_agent) { // On vérifie si la lettre saisi est bien dans la liste des agents
            return clePublic[i];    // Si c'est le cas, on retourne le clé publique qui lui est associé
        }
    }
    return -1;
}

int chiffrer(int code, int cle_publique) {
    int n = 43 * 67;
    return modexp(code, cle_publique, n);
}

int modexp(int code, int cle_publique, int n) {
    long code_chiffre;
    if(cle_publique < 0) {
        Serial.println("unhandled case");
        return -1;
    }
    if(code == 0 || n == 1) {
        code_chiffre = 0;
    } 
    else {
        code_chiffre = 1;
        while(cle_publique > 0) {   // Tant que la clé publique est supérieure à 0
            code_chiffre = (code_chiffre * (code % n)) % n; // On chiffre notre code généré au début de la fonction MA2()
            cle_publique = cle_publique - 1;
        }
    }
    return code_chiffre;    // On retourne le code une fois chiffré
}

void MA3() {
    String oeil;
    int concordance = 0;
    String liste_yeux[nombre_agent] = {"bleu",	"vert",	"marron", "marron",	"bleu",	"marron", "vert", "marron",	"marron", "gris", "bleu", "marron",	"bleu",	"marron", "vert", "gris"};  // Le tableau contenant les informations sur les yeux des agents est initialisé
    int etape1 = 0;
    while (true) { 
        if (etape1 == 0) {
            Serial.println("Quelle est la couleur de vos yeux ?");
            etape1 ++;
        }
        else if (etape1 == 1 && Serial.available() > 0) {
            oeil = Serial.readString();
            etape1 ++;
        }
        else if (etape1 == 2) {
            for (int i = 0; i < nombre_agent; i ++) {   // On vérifie si la lettre saisi et l'oeil scanné correspond au même agent
                if (liste_lettre[i] == lettre_agent) {
                    if (liste_yeux[i] == oeil) {
                        concordance = 1;
                    }
                }
            }
            if (concordance == 1) {
              	Serial.println("Scan rétinien validé.");
                return;
            }
            else {
                error();
              	return;
            }
        }
    }
}

void MA4() {    // Même fonctionnement que pour MA3()
    String doigt;
    int concordance = 0;
    String liste_doigts[nombre_agent] = {"pouce", "index",	"index", "annulaire", "auriculaire", "pouce", "majeur",	"majeur", "majeur",	"annulaire", "index", "pouce", "auriculaire", "majeur",	"index", "pouce"};
    int etape1 = 0;
    while (true) { 
        if (etape1 == 0) {
            Serial.println("Quel doigt souhaitez-vous scanner ?");
            etape1 ++;
        }
        else if (etape1 == 1 && Serial.available() > 0) {
            doigt = Serial.readString();
            etape1 ++;
        }
        else if (etape1 == 2) {
            for (int i = 0; i < nombre_agent; i ++) {
                if (liste_lettre[i] == lettre_agent) {
                    if (liste_doigts[i] == doigt) {
                        concordance = 1;
                    }
                }
            }
            if (concordance == 1) {
                Serial.println("Scan digital validé.");
                return;
            }
            else {
                error();
              	return;
            }
        }
    }
}

void MA5() {    // Même fonctionnement que MA3() et MA4()
    int card_id;
    int concordance = 0;
    int liste_CardId[nombre_agent] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};   // Initialisation du tableau contenant le card_id de chaque agent
    int etape1 = 0;
    while (true) { 
        if (etape1 == 0) {
            Serial.println("Entrez votre cardId");
            etape1 ++;
        }
        else if (etape1 == 1 && Serial.available() > 0) {
            card_id = Serial.parseInt();
            etape1 ++;
        }
        else if (etape1 == 2) {
            for (int i = 0; i < nombre_agent; i ++) {
                if (liste_lettre[i] == lettre_agent) {
                    if (liste_CardId[i] == card_id) {
                        concordance = 1;
                    }
                }
            }
            if (concordance == 1) {
                Serial.println("CardId correct");
                return;
            }
            else {
                error();
                return;
            }
        }
    }
}

void error() {
  	int i = 0;
  	Serial.println("Erreur");
    while (i < 3) { // La boucle est répétée 3 fois
        digitalWrite(ledR1, HIGH);  // Les LEDs clignote
        digitalWrite(ledR2, HIGH);
        digitalWrite(ledR3, HIGH);
        digitalWrite(ledR4, HIGH);
        delay(750);
        digitalWrite(ledR1, LOW);
        digitalWrite(ledR2, LOW);
        digitalWrite(ledR3, LOW);
        digitalWrite(ledR4, LOW);
        delay(750);
        i ++;
    }
    erreur = 1;
    essai ++;
    i = 0;

    if (essai > 1) {    // Si l'agent se trompe 2 fois pendant son authentification le coffre est bloqué 30s
        Serial.println("Erreur. coffre bloqué pendant 30s");
        while (i < 30) {// La boucle est répétée 30 fois
            digitalWrite(ledR1, HIGH);  // Les LEDs clignote
            digitalWrite(ledR3, HIGH);
            delay(250);
            digitalWrite(ledR2, HIGH);
            digitalWrite(ledR4, HIGH);
            delay(250);
            digitalWrite(ledR1, LOW);
            digitalWrite(ledR3, LOW);
            delay(250);
            digitalWrite(ledR2, LOW);
            digitalWrite(ledR4, LOW);
            delay(250);
            i ++;
        }
        essai = 0;
      	etape = 0;  // On retourne au début de l'authentification
      	modele = 0; // Le modèle est remis à 0 pour demander de nouveaux par la suite de rescanner la carte
    }
}