// #define
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define READ_MAX 10000
#define WRITE_MAX 10000
#define LOAD_DATA '1'
#define LOAD_NETWK '2'
#define CREATE_NETWK '3'
#define PRINT_NETWK '4'
#define TRAIN '5'
#define TEST '6'
#define SAVE_NETWK '7'


// ---------------------------------------------- D?clarations et d?finitions des structures ---------------------------------------------- //

// structure pour un neurone
typedef struct neurone_s
{
	double* poids;			// un tableau de poids
	double biais;			// une valeur de biais
	double delta;			// un tableau de delta
	double delta_biais;		// un tableau de delta_biais
	double* delta_poids;	// un tableau de delta_poids
}neurone_t;


// structure pour un layer
typedef struct layer_s
{
	neurone_t* neurone;		// un tableau de neurones
	double* entree;		    // un tableau d'entr?e
	double* sortie;		    // un tableau de sorties
	int nb_neurone;			// le nombre de neurone du layer
	int nb_entree;			// le nombre d'entree du layer
	int nb_sortie;			// le nombre de sortie du layer
}layer_t;


// structures pour le reseau / une liste doublement cha?n?e de layers
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


// ---------------------------------------------- D?clarations des fonctions ---------------------------------------------- //
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
double produitScalaire(double* a, double* b, int taille_vect);
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
void waitSeconds(time_t secondes);