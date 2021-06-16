#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

// constantes
#define N_NEURONES_ENTREE 784
#define N_NEURONE_SORTIE 10
#define N_NEURONE_CACHEE 16
#define READ_MAX 10000
#define WRITE_MAX 10000


// ---------------------------------------------- Déclarations et définitions des structures ---------------------------------------------- //

// structure pour un neurone
typedef struct neurone_s
{
	int nb;			      // nombre d'entrées
	double* poids;		// un tableau de poids
	double biais;		  // une valeur de biais
	double delta;
	double delta_biais;
	double* delta_poids;
}neurone_t;


// structure pour un layer
typedef struct layer_s
{
	neurone_t* neurone;		// un tableau de neurones
	double* entree;		    // un tableau d'entrée
	double* sortie;		    // un tableau de sorties
	int nb_neurone;
	int nb_entree;
	int nb_sortie;
}layer_t;


// structures pour le reseau / une liste chaînée de layers
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


// ---------------------------------------------- Déclarations des fonctions ---------------------------------------------- //
double rand_double(double a, double b);
int rand_int(int a, int b);
double sigmoide(double output);
void saveBMP(unsigned char data[], int rows, int cols, const char* filename);
dataset_t extractDataImg(const char* filename);
layer_t createLayer(int nb_neurones, int nb_entrees);
void addLayerToNetwork(reseau_t* network, layer_t layer);
reseau_t createNetworkFromFile(int nb_layers, const char* filename);
reseau_t createNetwork(int nb_layers, int nb_entrees_1st_layer, int* nb_neurones_layer);
void printLayer(layer_t layer);
void printNetwork(reseau_t network);
double produit_scalaire(double* a, double* b, int taille_vect);
int maxOutput(double* tab, int taille_tab);
void feedForward(reseau_t network, unsigned char* data_image);
void testNetwork(reseau_t network, dataset_t ds);
void delta_L(reseau_t network, int number_expected);
void delta_l(reseau_t network);
void backPropagation(reseau_t network, int number_expected);
void gradientDescent(reseau_t network, double l_rate, int nb_training_exemples);
void shuffleDataset(dataset_t ds);
void trainNetwork(reseau_t network, dataset_t ds, int nb_images, int nb_images_par_sous_groupe, int nb_epoch, double learning_rate);
void saveNetwork(reseau_t network, const char* filename);
void emptyBuffer(void);
void printMenu(void);
void waitSeconds(unsigned int secondes);