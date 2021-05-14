#define _CRT_SECURE_NO_WARNINGS

// constantes
#define N_NEURONES_ENTREE 784
#define N_NEURONE_SORTIE 10
#define N_NEURONE_CACHEE 16
#define READ_MAX 1000


// structure pour un neurone
typedef struct neurone_s
{
  int nb;			      // nombre d'entr�es
  double* poids;		// un tableau de poids
  double biais;		  // une valeur de biais
}neurone_t;


// structure pour un layer
typedef struct layer_s
{
  neurone_t* neurone;		// un tableau de neurones
  double* entree;		    // un tableau d'entr�e
  double* sortie;		    // un tableau de sorties
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


// structures pour le dataset
typedef struct subdata_s
{
  int number_expected;
  unsigned char* pixel;
}subdata_t;

typedef struct dataset_s
{
  int nb_images;
  int rows;
  int cols;
  int size;

  subdata_t* data;
}dataset_t;

// D�clarations des fonctions
double sigmoide(double output);
double aleat(int a, int b);
void saveBMP(unsigned char data[], int rows, int cols, const char* filename);
dataset_t extractDataImg(const char* filename);