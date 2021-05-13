#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <time.h>

// constante
#define N_NEURONES_ENTREE 784
#define N_NEURONE_SORTIE 10
#define N_NEURONE_CACHEE 16



// structure pour un neurone
struct neurone_s
{
    double poids;
    double biais;
    int nb_entree;
    int nb_sortie;
    double output;
    struct neurone_s** entrees;
    struct neurone_s** sorties;
};

typedef struct neurone_s neurone_t;

// structure pour un layer
struct layer_s
{
    neurone_t* neurone;
};

typedef struct layer_s layer_t;

// structure pour le reseau
struct reseau_s
{
    layer_t* couche;
};

typedef struct reseau_s reseau_t;

// fonction d'initialisation du réseau
void init(reseau_t* r);

// fonction loss
double perte(reseau_t* s);

// fonction sigmoide retourne valeur entre 0 et 1
double sigmoide(double output)
{
    return 1.0 / (1.0 + exp(-output));
}
// fonction gradient

// fonction nombre aleatoire
double aleat(int a, int b)
{
    return (double)((rand()/(double)RAND_MAX)*(b-a) + a);
}

int main(void)
{
    // test de la fonction aleat
    while (fgetc(stdin) != '0')
    {
        printf("%f\n",aleat(-100, 100));
    }
    return EXIT_SUCCESS;
}