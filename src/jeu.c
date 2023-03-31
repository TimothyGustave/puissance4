/**
 * On considère le tableau a 2 dimensions suivant:
 * 
 *  |  0|  1|  2|  3|  4|  5|  6| <- colonne c0 
 *  |  7|  8|  9| 10| 11| 12| 13| <- colonne c1
 *           
 *  | 42| 43| 44| 45| 46| 47| 48| <- colonne c6
 *    ^   ^   ^   ^   ^   ^   ^
 *    l0  l1  l2  l3  l4  l5  l6
 * 
 * Ce tableau represente la grille de jeu.
 * 
 * De plus on prend dit que X est le jeton de l'IA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jeu.h"

#define COLUMN 80
#define DEBUG 0

int maxTab(int tab[], int tailleTab) {
    int i, max = tab[0];

    for ( i = 1 ; i < tailleTab ; i++ ) {
        if ( tab[i] > max ) {
            max = tab[i];
        }
    }

    return max;
}

void afficheScorePartie(int score[]) {
    int gagnant, perdant;

    gagnant = ( score[0] > score[1] ) ? 0 : 1;
    perdant = (gagnant + 1 ) % 2;

    if ( score[gagnant] != score[perdant] ) {
        printf("Le joueur %d a gagné avec %d points contre %d points\n", 
            gagnant, score[gagnant], score[perdant]);
    } else {
        printf("Match nul ! Chacun a %d points \n", score[0]);
    }
}

int estGrilleValide(char grille[][7]) {
    int ligne, colonne;
    char car;

    for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
        for ( ligne = 0 ; ligne < 7 ; ligne++ ) {
            car = grille[colonne][ligne];
            if ( car != ' ' && car != 'X' && car != 'O' ) {
                fprintf(stderr, "La grille n'est pas valide");
                return 0;
            }
        }
    }

    return 1; 
}

/** 
 * Renvoie une Grille vide. Tout les éléments du tableau 
 * bidimentionnelle passé en argument prennent la valeur ' '
 */
void initialiseGrille(char grille[][7]) {
    int ligne, colonne;

    for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
        for ( ligne = 0 ; ligne < 7 ; ligne++) {
            /* On mets le jeton vide sur toute les cases */
            grille[colonne][ligne] = ' ';
        }
    }
}

/** 
 * Affiche sur le terminal la grille de jeu.
 * 
 * Reste a trouver comment recuperer varEnv $COLUMNS pour aligner 
 * le plateau par rapport a a la fernetre de terminal 
 */
void afficheGrille(char grille[][7]) {
    int colonne, ligne, centre;

    /* ou '\033[2J\033[1;1H' */
    #if DEBUG == 0
    system("clear");
    #endif
    /* la grille prend 15 colonne dans le terminal */
    centre = (COLUMN / 2) - 7;
    printf("\n");

    for ( ligne = 0 ; ligne < 7 ; ligne++ ) {
        printf("%*c", centre, ' ');
        for ( colonne = 0 ; colonne < 7 ; colonne++ ) {   
            printf("\033[34m|\033[m%c", grille[colonne][ligne] );
        }
        printf("\033[34m|\033[m\n");
    }

    printf("%*c",centre, ' ');

    for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
        printf("\033[34m %d", colonne);
    }

    printf("\033[m\n\n");
}

int estGrillePleine(char grille[][7]) {
    int colonne;

    for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
        if ( grille[colonne][0] == ' ' ) {
            return 0;
        }
    }

    return 1;
}

int estGrilleVide(char grille[][7]) {
    int colonne;

    for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
        if ( grille[colonne][6] != ' ') {
            return 0;
        }
    }

    return 1;
}

/* OPÉRATION */

int derniereLigneLibre(char colonne[7]) {
    int ligne = 0;

    while ( ligne < 7 && colonne[++ligne] == ' ' );

    return ligne - 1;
}

/**
 * Fait moins de calculs que derniereLigneLibre 
 * quand les colonnes a moitié remplies. Et on a
 * besoin de cette definition pour les opérations de rotation
 */
int premiereLigneLibre(char colonne[7]) {
    int ligne = 7;

    while ( ligne >= 0 && colonne[--ligne] != ' ');

    return ligne;
}

/* retourne 1 si l'opération c'est bien passé et 0 sinon */
int ajouterJeton(char colonne[7], char jeton) {
    int ligne;

    ligne = premiereLigneLibre(colonne);

    if ( ligne < 7 && ligne >= 0 ) {
        colonne[ligne] = jeton;
        return 1;
    } else {
        return 0;
    }
}

char retirerJeton(char colonne[7]) {
    int ligne = premiereLigneLibre(colonne) + 1;
    char res;

    res = colonne[ligne];
    colonne[ligne] = ' ';

    return res;
}

void inverserColonne(char colonne[7]) {
    char colonneTmp[7] = {' ',' ',' ',' ',' ',' ',' '};
    int nbElt = premiereLigneLibre(colonne);
    int ligne;

    for ( ligne = nbElt + 1 ; ligne < 7 ; ligne++ ) {
        ajouterJeton(colonneTmp, colonne[ligne]);
        colonne[ligne] = ' ';
    }

    for ( ligne = 6 ; ligne > nbElt ; ligne-- ) {
        ajouterJeton(colonne, colonneTmp[ligne]);
    }
}

void inverserGrille(char grille[][7]) {
    int nColonne; 

    for ( nColonne = 0 ; nColonne < 7 ; nColonne++ ) {
        inverserColonne(grille[nColonne]);
    }
}

void copierTab2Dimension(char tabSrc[][7], char tabDest[][7]) {
    int colonne, ligne;

    for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
        for ( ligne = 0 ; ligne < 7 ; ligne++ ) {
            tabDest[colonne][ligne] = tabSrc[colonne][ligne];
        }
    }
}

void tournerAntiHoraire(char grille[][7]) {
    int ligne, colonne;
    char grilleTmp[7][7];

    initialiseGrille(grilleTmp);

    for ( ligne = 0 ; ligne < 7 ; ligne++ ) {
        for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
            ajouterJeton(grilleTmp[ligne], grille[colonne][ligne]);
        }
    }

    copierTab2Dimension(grilleTmp, grille);
}

void tournerHoraire(char grille[][7]) {
    int ligne, colonne;
    char grilleTmp[7][7];

    initialiseGrille(grilleTmp);

    for ( ligne = 6 ; ligne >= 0 ; ligne-- ) {
        for ( colonne = 6 ; colonne >= 0 ; colonne-- ) {
            ajouterJeton(grilleTmp[6 - ligne], grille[colonne][ligne]);
        }
    }

    copierTab2Dimension(grilleTmp, grille);
}

/* EVALUATION */

/**
 * Retourne un entier représentant le nombres de Jetons
 * alignés verticalement
 */
int evalVertical(char grille[][7], int colonne, int ligne) {
    int compteur = 1, i = 1;
    char motifDJ = grille[colonne][ligne];
    
    while ( grille[colonne][ligne + i] == motifDJ ) {
        compteur += 1; 
        i++;
    }

    i = -1;

    while ( grille[colonne][ligne + i] == motifDJ ) {
        compteur += 1; 
        i--;
    }

    #if DEBUG == 1
    printf("compteur eval_vertical = %d \n", compteur);
    #endif

    return compteur;
}

/** 
 * Retourne le nombre de jeton aligné horizontalement 
 * par rapport au jeton present a la colonne et ligne fournie
 * en argument
 */
int evalHorizontal(char grille[][7], int colonne, int ligne) {
    int compteur = 1, i = 1; 
    char motifDJ = grille[colonne][ligne];

    while ( grille[colonne + i][ligne] == motifDJ ) {
        compteur++;
        i++;
    }

    i = -1; 

    while ( grille[colonne + i][ligne] == motifDJ ) {
        compteur++;
        i--;
    }

    #if DEBUG == 1
    printf("compteur eval_horizontal = %d \n", compteur);
    #endif

    return compteur;
}

int evalDiagonalGD(char grille[][7], int colonne, int ligne) {
    int compteur = 1, i = 1;
    char motifDJ = grille[colonne][ligne];

    while ( grille[colonne + i][ligne + i] == motifDJ ) {
        compteur++;
        i++;
    }

    i = -1;

    while ( grille[colonne + i][ligne + i] == motifDJ ) {
        compteur++;
        i--;
    }

    #if DEBUG == 1
    printf("compteur eval_diag ( sens gh -> bd ) = %d \n", compteur);
    #endif

    return compteur;
}

int evalDiagonalDG(char grille[][7], int colonne, int ligne) {
    int compteur = 1, i = 1; 
    char motifDJ = grille[colonne][ligne];

    while ( grille[colonne + i][ligne - i] == motifDJ ) {
        compteur++;
        i++;
    }

    i = -1;

    while ( grille[colonne + i][ligne - i] == motifDJ ) {
        compteur++;
        i--;
    }

    #if DEBUG == 1
    printf("compteur eval_diag ( sens dg ) = %d \n", compteur);
    #endif

    return compteur;
}

/**
 * On evalue en un certain point et on retourne le nombre de symbole
 * identique adjacent
 *  antecedent jeton != ' ' 
 */
int eval(char grille[][7], int colonne, int ligne) {
    int tab[4] = {0,0,0,0};

    tab[0] = evalVertical(grille, colonne, ligne);
    tab[1] = evalHorizontal(grille, colonne, ligne);
    tab[2] = evalDiagonalGD(grille, colonne, ligne);
    tab[3] = evalDiagonalDG(grille, colonne, ligne);

    return maxTab(tab, 4);
}

/**
 * Si on utilises une opération qui change tout les jetons 
 * on evalue en chaque jeton en essayant de limiter les cas.
 * 
 * Si on a plusieurs fois un puissance 4, on renvoie le jeton qui
 * a la plus grande valeur a l'evaluation
 * 
 */
PositionJeton evalTout(char grille[][7]) {
    int colonne, ligne, valeurTmp; 
    char car;
    PositionJeton res;
    
    res.nbMaxJetonAlign = 1;

    for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
        /**
         * On commence par la ligne la plus en bas pour eviter de 
         * regarder les lignes qui nous le savons seront vides
         */
        for ( ligne = 6 ; ligne >= 0 ; ligne-- ) {
            car = grille[colonne][ligne];
            if ( car == ' ' ) {
                continue;
            } else {
                valeurTmp = eval(grille, colonne, ligne);
                if ( valeurTmp > res.nbMaxJetonAlign ) {
                    res.nbMaxJetonAlign = valeurTmp;
                    res.colonne = colonne;
                    res.ligne = ligne;
                }
            }
        }
    }

    return res;
}

int JetonToJoueur(char grille[][7], PositionJeton jeton) {
    if ( grille[jeton.colonne][jeton.ligne] == 'X' ) {
        return 1;
    } else if ( grille[jeton.colonne][jeton.ligne] == 'O' ) {
        return 0;
    } else {
        fprintf(stderr, "Le jeton n'existe pas");
        return 2;
    }
}

/**
 * Valeur de retour :
 * 0 : la partie continue
 * 1 : grillePleine
 * 2 : le joueur 0 a gagné
 * 3 : le joueur 1 a gagné
 */
int arreterManche(char grille[][7]) {
    PositionJeton jetonGagnant;

    jetonGagnant = evalTout(grille);

    if ( jetonGagnant.nbMaxJetonAlign >= 4) {
        return (JetonToJoueur(grille, jetonGagnant) + 2);
    } else if ( estGrillePleine(grille) ) {
        printf("La grille est pleine. C'est donc un match nul");
        return 1;
    } else {
        return 0;
    }
}

/* IA - JOUEUR AUTONOME */

/**
 * En attendant d'avoir mieux on utilise un mode aléatoire.
 */
void randomDecision(char grille[][7], char choixIA[], int joueur) {
    int choixOp = rand() % 4, choixColonne = 0;
    char jeton[2] = {'O','X'};

    switch (choixOp) {
    case 0:
        do {
            choixColonne = rand() % 7;
        } while ( !ajouterJeton(grille[choixColonne], jeton[joueur]) );
        strcpy(choixIA, "L'IA a ajouté un jeton");
        break;
    
    case 1:
        inverserGrille(grille);
        strcpy(choixIA, "L'IA a inversé la grille");
        break;

    case 2: 
        tournerAntiHoraire(grille);
        strcpy(choixIA, "L'IA a tourné la grille de 90° dans le sens anti-horaire");
        break;

    case 3:
        tournerHoraire(grille);
        strcpy(choixIA, "L'IA a tourné la grille de 90° dans le sens horaire");
        break;
    
    default:
        break;
    }
}

/* SAUVEGARDES */

void insererGrilleParCar(char grille[][7], FILE *fichier) {
    int colonne, ligne, car;

    for ( ligne = 0 ; ligne < 7 ; ligne++ ) {
        for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
            car = (int)grille[colonne][ligne];
            fputc(car, fichier);
        }
    }
}

void recupererGrille(FILE *fichier, char grille[][7]) {
    int colonne, ligne;
    char car;

    for ( ligne = 0 ; ligne < 7 ; ligne++ ) {
        for ( colonne = 0 ; colonne < 7 ; colonne++ ) {
            car = (char)fgetc(fichier);
            #if DEBUG == 1
            printf("car : %d", car);
            #endif
            grille[colonne][ligne] = car;
        }
    }
}

int estFichierVide(FILE *fichier) {
    if ( fgetc(fichier) == EOF ) {
        return 1;
    } else {
        return 0;
    }
}

/** 
 * Stocke la grille dans un fichier afin de pouvoir la reutilisé 
 * dans une partie ultérieur
 */
void sauvPartie(char grille[][7]) {
    FILE *fichier = NULL;
    
    fichier = fopen("../Parties/partieEnCours","w+");

    if ( fichier != NULL ) {
        printf("Sauvegarde..\n");

        insererGrilleParCar(grille, fichier);

        fclose(fichier);
    } else {
        fprintf(stderr, "L'ouverture du fichier a echoué. Lors de l'insertion\n");
    }
}

/**
 * Initialise la grille avec la grille de la derniere partie
 * non terminer 
 */
int recupPartie(char grille[][7]) {
    FILE *fichier = NULL;

    fichier = fopen("../Parties/partieEnCours","r");

    if ( fichier != NULL ) {
        if ( estFichierVide(fichier) ) {
            return 0;
        }

        recupererGrille(fichier, grille);

        fclose(fichier);

        return 1;
    } else {
        fprintf(stderr, "L'ouverture du fichier a echoué. Lors de la récup\n");
        return 0;
    }
}

/* JEU ET UTILISATEUR */

int continuerDerniereGrille(void) {
    int choix = 2;

    do {
        printf("Vous n'avez pas finie votre derniere partie. ");
        printf("Voulez-vous la finir ? (1/0) ");
        scanf("%d", &choix);
    } while ( choix < 0 || choix > 1 );
    
    return choix;
}

/**
 *  Renvoie le nombre de joueur autonome qui joueront
 *  dans la partie
 */
int choisirModePartie(void) {
    int choix; 

    do {
        choix = 2;
        printf("Combien de joueur autonome ? (0/1/2) ");
        scanf("%d", &choix);
    } while ( choix < 0 || choix > 2 );

    return choix;
}

/**
 * Choix de la colonne dans laquelle ajouter un jeton
 */
int choisirColonne(void) {
    int colonne; 

    printf("Dans quelle colonne ?\n\n");

    do {
        printf(GREEN "Votre choix : ");
        scanf("%d", &colonne);
        printf("\a");
    } while ( colonne < 0 || colonne > 6 );

    printf(RESETCOLOR "\n");

    return colonne;
}

/**
 * Demande aux joueur humain quelle opération effectuer 
 */
int choisirOp(void) {
    int choix;

    printf("Quelle opération voulez-vous effectuer ?\n");
    printf("1 : Ajouter un jeton\n");
    printf("2 : Inverser la grille\n");
    printf("3 : Tourner la grille dans sens anti-horaire\n");
    printf("4 : Tourner la grille dans le sens horaire\n");
    printf("5 : " RED "Sauver et quitter" RESETCOLOR "\n\n");

    do {
        printf(GREEN "Votre choix : ");

        scanf("%d", &choix);
        printf(RESETCOLOR "\n");
    } while ( choix < 1 || choix > 5 );

    return choix;
}

/**
 * Demande et effectue l'opération choisi par le joueur humain 
 */
int effectuerOp(char grille[][7], int joueur) {
    int choix = 0;
    char jeton[] = {'O','X'};

    choix = choisirOp();

    switch (choix) {
    case 1:
        choix = ajouterJeton(grille[choisirColonne()], jeton[joueur]);
        break;
    
    case 2:
        inverserGrille(grille);
        break;

    case 3:
        tournerAntiHoraire(grille);
        break;

    case 4:
        tournerHoraire(grille);
        break;
    
    case 5:
        system("clear");
        sauvPartie(grille);
        break;
        
    default:
        break;
    }

    return choix;
}

/**
 * @modeIA = 0, 1 ou 2 
 */
int jeu(int modeIA, char grille[][7]) {
    int joueur = 0, arreter = 0, choixOp = 0;
    char choixIA[60] = {0};

    do {
        if ( modeIA == 2 ) {
            randomDecision(grille, choixIA, joueur);
        } else if ( modeIA == 1 && joueur == 1 ) {
            randomDecision(grille, choixIA, joueur);
        } else {
            afficheGrille(grille);

            printf(BGYELLOW "Au tour de joueur%d." RESETCOLOR " ", joueur);
            if ( modeIA == 1 && joueur == 0 ) {
                printf("%s", choixIA);
            } 
            printf("\n\n");

            do {
                choixOp = effectuerOp(grille, joueur);
            } while ( choixOp == 0 );
        }

        arreter = arreterManche(grille);

        joueur = ( joueur + 1 ) % 2;
    } while ( !arreter && choixOp != 5 );

    /**
     * Si un joueur a gagné, on renvoie le numéro du joueur 
     * Sinon on envoie un nombre négatif et on sait qu'on a quitté 
     * la partie pour une autre raison.
     */
    return arreter - 2;
}

int jouerPlusieursParties(char grille[][7]) {
    int modePartie, gagnant, score[2] = {0,0}, continuer;

    do {
        afficheGrille(grille);

        modePartie = choisirModePartie();
        gagnant = jeu(modePartie, grille);

        if ( gagnant >= 0 ) {
            score[gagnant]++;
        } else {
            printf("Vous avez quitté la manche. La partie a été sauvé");
        }
        
        afficheGrille(grille);
        afficheScorePartie(score);
        printf("Voulez-vous rejouer ? (1/0)\n");
        scanf("%d", &continuer);

        initialiseGrille(grille);
    } while ( continuer );

    return 0;
}