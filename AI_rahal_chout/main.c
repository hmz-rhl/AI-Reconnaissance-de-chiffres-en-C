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

};

typedef struct neurone_s neurone_t;

// structure pour un layer
struct layer_s
{

};

typedef struct layer_s layer_t;

// structure pour le reseau
struct reseau_s
{

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
    return (double)rand() / ((double)RAND_MAX / a);
}

int main(int argc, char** argv)
{


    return EXIT_SUCCESS;
}