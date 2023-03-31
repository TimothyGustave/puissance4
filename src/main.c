#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "jeu.h"

#define TEST 0
#define TESTIA 0

void test(void){
    char grille[7][7];

    printf("On initialise la grille\n");
    initialiseGrille(grille);
    afficheGrille(grille);

    printf("On ajoute des jetons\n");
    ajouterJeton(grille[0], 'X');
    ajouterJeton(grille[0], 'O');
    ajouterJeton(grille[0], 'X');
    ajouterJeton(grille[6], 'X');
    ajouterJeton(grille[6], 'O');
    afficheGrille(grille);

    printf("Inverser\n");
    inverserGrille(grille);
    afficheGrille(grille);

    printf("Rotation Antihoraire\n");
    tournerAntiHoraire(grille);
    afficheGrille(grille);

    printf("Rotation horaire\n");
    tournerHoraire(grille);
    afficheGrille(grille);

    printf("On veut gagner maintenant.");
    printf(" On complete et on evalue en [1][0]\n");
    ajouterJeton(grille[2], 'O');
    ajouterJeton(grille[3], 'O');
    ajouterJeton(grille[4], 'O');
    afficheGrille(grille);
    eval(grille, 1, 6);

    /* On sauvegarde la grille */
    sauvPartie(grille);
    recupPartie(grille);
    afficheGrille(grille);
}

/* Verifions que la probabilité de gagner pour chaque IA est de 50% */
void testIA(char grille[][7]) {
    int i, score[2] = {0,0}, gagnant;

    for ( i = 0 ; i < 100 ; i++ ) {
        initialiseGrille(grille);
        gagnant = jeu(2, grille);

        if ( gagnant >= 0 ) {
            score[gagnant]++;
        } else {
            printf("Vous avez quitté la manche. La partie a été sauvé");
        }
    }

    afficheGrille(grille);
    afficheScorePartie(score);  
}

int main(int argc, char *argv[]) {
    char grille[7][7];
    int continuer = 1;
    time_t t;

    /* On initialise une valeur aléatoire pour le joueur autonome */
    srand((unsigned) time(&t));

    #if TEST == 1 
    test();
    #endif

    #if TESTIA == 1
    testIA(grille);
    #endif

    #if TESTIA == 0 && TEST == 0
    printf("Attention on va effacer le terminal. On continue ? (1/0) ");
    scanf("%d", &continuer);
    if ( !continuer ) {
        exit(0);
    } else {
        system("clear");
    }

    /* On initialise la grille */
    if ( recupPartie(grille) && estGrilleValide(grille) && continuerDerniereGrille() ) {
        printf("Très bien, finissons en !\n");
    } else {
        initialiseGrille(grille);
    }

    jouerPlusieursParties(grille);

    /* On nettoie avant de quitter le programme */
    system("clear");
    #endif

    return 0;
}