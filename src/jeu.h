typedef struct PositionJeton PositionJeton;
struct PositionJeton {
    int colonne;
    int ligne;
    int nbMaxJetonAlign;
};

#define RED "\033[31m"
#define GREEN "\033[32m"

#define BGYELLOW "\033[43m"

#define RESETCOLOR "\033[m"

void afficheScorePartie(int score[]);

/* GRILLE */
int estGrilleValide(char grille[][7]);
void initialiseGrille(char grille[][7]);
void afficheGrille(char grille[][7]);

/* OPERATION */
int derniereLigneLibre(char colonne[7]);
int premiereLigneLibre(char colonne[7]);
int ajouterJeton(char colonne[7], char jeton);
char retirerJeton(char colonne[7]);
void inverserColonne(char colonne[7]);
void inverserGrille(char grille[][7]);
void tournerAntiHoraire(char grille[][7]);
void tournerHoraire(char grille[][7]);

/* EVALUATION */
int eval(char grille[][7], int colonne, int ligne);

/* IA */
void randomDecision(char grille[][7], char choixIA[], int joueur);

/* SAUVEGARDE */
void sauvPartie(char grille[][7]);
int recupPartie(char grille[][7]);

/* UTILISATEUR */
int continuerDerniereGrille(void);
int choisirModePartie(void);
int jeu(int modeIA, char grille[][7]);
int jouerPlusieursParties(char grille[][7]);
