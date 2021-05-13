#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <time.h>

// constante
#define N_NEURONES_ENTREE 784
#define N_NEURONE_SORTIE 10
#define N_NEURONE_CACHEE 16



// structure pour un neurone
typedef struct neurone_s
{
  int nb;			// nombre d'entr�es
  double* poids;		// un tableau de poids
  double* biais;		// un tableau de biais
  double* entree;		// un tableau d'entr�es, car un neurone poss�de plusieurs entr�es
  double output;		// une sortie calcul�e "entree*poids+biais", car un neurone poss�de UNE seule sortie
}neurone_t;

// structure pour un layer
typedef struct layer_s
{
  neurone_t* neurone;		// un tableau de neurones
  neurone_t* entree;		// un tableau d'entr�e
  neurone_t* sortie;		// un tableau de sorties
  int nb_neurone;
  int nb_entree;
  int nb_sortie;
}layer_t;


// structures pour le reseau / une liste cha�n�e de layers
typedef struct node_layer_s
{
  layer_t layer;
  struct node_layer_s* suiv;
  struct node_layer_s* prec;
}node_layer_t;

typedef struct reseau_s
{
  node_layer_t* tete;
  node_layer_t* queue;
}reseau_t;


// fonction d'initialisation du r�seau
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
  return (double)((rand() / (double)RAND_MAX) * (b - a) + a);
}

int main(void)
{
  // test de la fonction aleat
  while (fgetc(stdin) != '0')
  {
    printf("%f\n", aleat(-100, 100));
  }
  return EXIT_SUCCESS;
}