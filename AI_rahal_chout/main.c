#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "IA.h"

// ---------------------------------------------- Définitions des fonctions ---------------------------------------------- //

// fonction qui renvoie un nombre aléatoire entre a et b
double rand_double(double a, double b)
{
  return ((double)rand() * (b - a)) / (double)RAND_MAX + a;
}

// fonction sigmoide retourne valeur entre 0 et 1
double sigmoide(double output)
{
  return 1.0 / (1.0 + exp(-output));
}

// fonction de sauvegarde des images au format BMP
void saveBMP(unsigned char data[], int rows, int cols, const char* filename)
{
  int width = rows, height = cols;
  int pad = (4 - (3 * width) % 4) % 4;
  int filesize = 54 + (3 * width + pad) * height;
  unsigned char header[54] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0, 40,0,0,0,
                                       0,0,0,0, 0,0,0,0, 1,0,24,0 };
  char padding[3] = { 0,0,0 };
  unsigned char* img;
  FILE* file;
  int  i;
  for (i = 0; i < 4; i++)
  {
    header[2 + i] = (unsigned char)((filesize >> (8 * i)) & 255);
    header[18 + i] = (unsigned char)((width >> (8 * i)) & 255);
    header[22 + i] = (unsigned char)((height >> (8 * i)) & 255);
  }
  img = malloc(sizeof(char) * 3 * (size_t)width * (size_t)height);
  if (img == NULL)
    printf("Error could not allocate memory !\n");
  else
  {
    for (i = 0; i < width * height; i++)
    {
      int color = data[i];
      img[3 * i + 2] = img[3 * i + 1] = img[3 * i] = (unsigned char)(color & 0xFF);
    }
    file = fopen(filename, "wb");
    if (file == NULL)
      printf("Error: could not open file '%s' !\n", filename);
    else
    {
      fwrite(header, 54, 1, file);
      for (i = height - 1; i >= 0; i--)
      {
        fwrite((char*)(img + 3 * width * i), 3 * (size_t)width, 1, file);
        fwrite(padding, (unsigned int)pad, 1, file);
      }
      fclose(file);
    }
    free(img);
  }
}

// fonction qui extrait les données des images d'un fichier pour les transférer à une variable de type structuré "dataset_t"
dataset_t extractDataImg(const char* filename)
{
  dataset_t ds;
  FILE* file;
  char chaine[READ_MAX] = "";
  int i, j, nb_zeros;
  int k = 0;
  unsigned char pixel;
  file = fopen(filename, "r");

  if (file == NULL)
    printf("Error: could not open file '%s' !\n", filename);
  else
  {
    fgets(chaine, READ_MAX, file);
    char* extracted = strtok(chaine, ";");
    ds.nb_images = atoi(extracted);
 
    extracted = strtok(NULL, ";");
    ds.rows = atoi(extracted);

    extracted = strtok(NULL, ";");
    ds.cols = atoi(extracted);

    ds.size = ds.rows * ds.cols;
    ds.data = calloc(ds.nb_images, sizeof(subdata_t));

    while (fgets(chaine, READ_MAX, file) != NULL) 
    {
      ds.data[k].pixel = calloc(ds.size, sizeof(unsigned char));
      i = 0;
      char* extracted2 = strtok(chaine, ";");
      ds.data[k].number_expected = atoi(extracted2);
      extracted2 = strtok(NULL, ";");

      while (extracted2 != NULL)
      {                    
        if (strchr(extracted2, 44) != NULL)                     //  On cherche la présence d'une virgule
        {
          char* a = &extracted2[2];
          nb_zeros = atoi(a);
          for (j = 0; j < nb_zeros; j++)
          {
            ds.data[k].pixel[i] = 0;
            i++;
          }
        }
        else
        {
          pixel = (unsigned char)atoi(extracted2);
          ds.data[k].pixel[i] = pixel;
          i++;
        }
        extracted2 = strtok(NULL, ";");
      }
      k++;
    }
    fclose(file);
  }
  return ds;
}

// fonction qui créer un layer vide  
layer_t createLayer(int nb_neurones, int nb_entrees)
{
  layer_t layer;
  layer.nb_entree = nb_entrees;
  layer.nb_neurone = nb_neurones;
  layer.nb_sortie = nb_neurones;

  layer.neurone = calloc(layer.nb_neurone, sizeof(neurone_t));
  if (layer.neurone == NULL)
  {
    printf("createLayer : Erreur allocation dynamique de mémoire pour le neurone !\n");
    exit(1);
  }

  layer.entree = calloc(nb_entrees, sizeof(double));
  if (layer.entree == NULL)
  {
    printf("createLayer : Erreur allocation dynamique de mémoire pour le tableau d'entrées du layer !\n");
    exit(1);
  }

  layer.sortie = calloc(nb_neurones, sizeof(double));
  if (layer.sortie == NULL)
  {
    printf("createLayer : Erreur allocation dynamique de mémoire pour le tableau de sorties du layer !\n");
    exit(1);
  }

  layer.neurone->nb = nb_entrees;

  int i;
  for (i = 0; i < nb_neurones; i++)
  {
    layer.neurone[i].poids = calloc(layer.neurone->nb, sizeof(double));
    if (layer.neurone[i].poids == NULL)
    {
      printf("createLayer : Erreur allocation dynamique de mémoire pour le tableau de poids !\n");
      exit(1);
    }
  }

  for (i = 0; i < nb_neurones; i++)      
  {
    layer.neurone[i].biais = 0;
    layer.neurone[i].delta_biais = 0;

    layer.neurone[i].delta_poids = calloc(layer.nb_entree, sizeof(double));
    if (layer.neurone[i].delta_poids == NULL)
    {
      printf("createLayer : Erreur allocation dynamique de mémoire pour le tableau de delta poids !\n");
      exit(1);
    }
  }

  return layer;
}

// fonction qui ajoute un layer au network
void addLayerToNetwork(reseau_t* network, layer_t layer)
{
  node_layer_t* node;
  node = calloc(1, sizeof(node_layer_t));

  if (node == NULL)
  {
    fprintf(stderr, " addLayerToNetwork : memory allocation failed !\n");
  }
  else
  {
    node->layer = layer;
    if (network->queue)
    {
      network->queue->suiv = node;
      node->prec = network->queue;
      network->queue = node;
    }
    else
    {
      network->tete = node;
      network->queue = node;
    }
  }
}

// fonction qui créer un réseau depuis un fichier
reseau_t createNetworkFromFile(int nb_layers, const char* filename)
{
  reseau_t network = { NULL, NULL };
  layer_t* layers = calloc(nb_layers, sizeof(layer_t));
  int i, j, k;

  int nb_entrees;
  char chaine[READ_MAX] = "";
  FILE* file;
  file = fopen(filename, "r");

  if (file == NULL)
    printf("createNetworkFromFile : could not open file '%s' !\n", filename);
  else
  {
    for (i = 0; i < nb_layers; i++)
    {
      fgets(chaine, READ_MAX, file);
      char* extracted = strtok(chaine, ";");
      nb_entrees = atoi(extracted);
      layers[i].nb_entree = nb_entrees;

      extracted = strtok(NULL, ";");
      layers[i].nb_neurone = atoi(extracted);
      layers[i].nb_sortie = layers[i].nb_neurone;
      
      layers[i] = createLayer(layers[i].nb_neurone, nb_entrees);

      for (j = 0; j < layers[i].nb_neurone; j++)
      {
        fgets(chaine, READ_MAX, file);
        char* extracted = strtok(chaine, ";");
        layers[i].neurone[j].biais = atof(extracted);
        
        for (k = 0; k < nb_entrees; k++)
        {
          char* extracted = strtok(NULL, ";");
          layers[i].neurone[j].poids[k] = atof(extracted);
        }
      }
      addLayerToNetwork(&network, layers[i]);
    }
  }
  return network;
}

// fonction qui créer un réseau initialisé avec des poids et biais aléatoires
reseau_t createNetwork(int nb_layers, int nb_entrees_1st_layer, int* nb_neurones_layer)
{
  reseau_t network = { NULL, NULL };
  layer_t* layers = calloc(nb_layers, sizeof(layer_t));
  int i, j, k;
  int nb_entrees = nb_entrees_1st_layer;

  for (i = 0; i < nb_layers; i++)
  {
    layers[i].nb_entree = nb_entrees;
    layers[i].nb_neurone = nb_neurones_layer[i];
    layers[i].nb_sortie = layers[i].nb_neurone;
    layers[i] = createLayer(layers[i].nb_neurone, layers[i].nb_entree);

    for (j = 0; j < layers[i].nb_neurone; j++)
    {
      layers[i].neurone[j].biais = rand_double(-10, 10);

      for (k = 0; k < nb_entrees; k++)
      {
        layers[i].neurone[j].poids[k] = rand_double(-10, 10);
      }
    }
    addLayerToNetwork(&network, layers[i]);
    nb_entrees = layers[i].nb_sortie;
  }
  return network;
}

// fonction qui affiche le contenu d'un layer
void printLayer(layer_t layer)
{
  printf("Ce layer contient %d neurones, %d entrees et %d sorties. \n", layer.nb_neurone, layer.nb_entree, layer.nb_sortie);
}

// fonction qui affiche le contenu d'un réseau
void printNetwork(reseau_t network)
{
  int i = 1;
  if ((network.tete == NULL) && (network.queue == NULL))
  {
    fprintf(stderr, "Empty network!");
    printf("\n");
  }
  else
  {
    node_layer_t* node;
    node = network.tete;
    while (node != network.queue)
    {
      printf("Layer n#%d\n", i);
      printLayer(node->layer);
      node = node->suiv;
      i++;
    }
    printf("Layer n#%d:\n", i);
    printLayer(network.queue->layer);
  }
  printf("\n");
}

// fonction qui calcule le produit scalaire entre 2 vecteurs
double produit_scalaire(double* a, double* b, int taille_vect)
{
  double res = 0;
  for (int i = 0; i < taille_vect; i++)
  {
    res += a[i] * b[i];
  }
  return res;
}

// fonction qui renvoie l'indice de la valeur maximale du tableau
int maxOutput(double* tab, int taille_tab)
{
  int max = 0;
  int i;

  for (i = 1; i < taille_tab; i++)
  {
    max = (tab[i]>tab[max]) ? i : max;
  }
  return max;
}

// fonction qui teste un réseau opérationnel, et renvoie le chiffre qui a été lu sur l'image par ce réseau de neurones
void testNetwork(reseau_t network, dataset_t ds)
{
  int i,j;
  double succes = 0;
  double taux_de_reussite;
  node_layer_t* node = network.tete;

  for (j = 0; j < 50000 ; j++)     // pas oublier de modif -> j = 0      //ds.nb_images
  {
    for (i = 0; i < node->layer.nb_entree; i++)
    {
      node->layer.entree[i] = ((double)ds.data[j].pixel[i]) / 255;                
    }

    for (i = 0; i < node->layer.nb_sortie; i++)
    {
      node->layer.sortie[i] = sigmoide(produit_scalaire(node->layer.entree, node->layer.neurone[i].poids, node->layer.nb_entree) + node->layer.neurone[i].biais);
    }

    while (node->suiv != NULL)
    {
      node = node->suiv;

      for (i = 0; i < node->layer.nb_entree; i++)
      {
        node->layer.entree[i] = node->prec->layer.sortie[i];
      }

      for (i = 0; i < node->layer.nb_sortie; i++)
      {
        node->layer.sortie[i] = sigmoide(produit_scalaire(node->layer.entree, node->layer.neurone[i].poids, node->layer.nb_entree) + node->layer.neurone[i].biais);
      }
    }

    int chiffre_lu = maxOutput(node->layer.sortie, node->layer.nb_sortie);
    printf("Chiffre attendu : %d\n", ds.data[j].number_expected);
    printf("Chiffre predit par le reseau de neuronnes : %d\n\n", chiffre_lu);

    succes = (ds.data[j].number_expected == chiffre_lu) ? succes + 1 : succes;
    node = network.tete;
  }
  taux_de_reussite = (succes / ds.nb_images) * 100;
  printf("Le taux de reconnaissance de ce reseau de neuronnes est de %f %% !\n", taux_de_reussite);
}

// fonction qui effectue le feedForward
void feedForward(reseau_t network, unsigned char* data_image)
{
  int i;
  node_layer_t* node = network.tete;

  for (i = 0; i < node->layer.nb_entree; i++)
  {
    node->layer.entree[i] = ((double)data_image[i]) / 255;
  }

  for (i = 0; i < node->layer.nb_sortie; i++)
  {
    node->layer.sortie[i] = sigmoide(produit_scalaire(node->layer.entree, node->layer.neurone[i].poids, node->layer.nb_entree) + node->layer.neurone[i].biais);
  }

  while (node->suiv != NULL)
  {
    node = node->suiv;

    for (i = 0; i < node->layer.nb_entree; i++)
    {
      node->layer.entree[i] = node->prec->layer.sortie[i];
    }

    for (i = 0; i < node->layer.nb_sortie; i++)
    {
      node->layer.sortie[i] = sigmoide(produit_scalaire(node->layer.entree, node->layer.neurone[i].poids, node->layer.nb_entree) + node->layer.neurone[i].biais);
    }
  }
  node = network.tete;
}

// fonction qui implémente delta_L
void delta_L(reseau_t network, int number_expected)
{
  int i, j;

  node_layer_t* node;
  node = network.queue;

  for (i = 0; i < node->layer.nb_neurone; i++)
  {
    node->layer.neurone[i].delta = (node->layer.sortie[i] - (i == number_expected) ) * (node->layer.sortie[i] * (1 - node->layer.sortie[i]) );
    node->layer.neurone[i].delta_biais += node->layer.neurone[i].delta;
   
    for (j = 0; j < node->layer.nb_entree; j++)
    {
      node->layer.neurone[i].delta_poids[j] += node->layer.entree[j] * node->layer.neurone[i].delta;
    }
  }
}

// fonction qui implémente delta_l
void delta_l(reseau_t network)
{
  int l, i, j, k;

  node_layer_t* node;
  node = network.queue->prec;

  while (node != NULL)
  {
    for (i = 0; i < node->layer.nb_neurone; i++)
    {
      node->layer.neurone[i].delta = 0;
      for (k = 0; k < node->suiv->layer.nb_neurone; k++)
      {
        node->layer.neurone[i].delta += node->suiv->layer.neurone[k].delta * node->suiv->layer.neurone[k].poids[i];
      }
      node->layer.neurone[i].delta = node->layer.neurone[i].delta * (node->layer.sortie[i] * (1 - node->layer.sortie[i]));
      node->layer.neurone[i].delta_biais += node->layer.neurone[i].delta;

      for (j = 0; j < node->layer.nb_entree; j++)
      {
        node->layer.neurone[i].delta_poids[j] += node->layer.entree[j] * node->layer.neurone[i].delta;
      }
    }
    node = node->prec;
  }
}

// fonction qui implémente l'algorithme de backpropagation
void backPropagation(reseau_t network, int number_expected)
{
  delta_L(network, number_expected);
  delta_l(network);
}

// fonction qui effectue la descente de gradient
void gradientDescent(reseau_t network, double l_rate, int nb_training_exemples)      
{
  int l, i, j;
  node_layer_t* node;
  node = network.queue;

  while (node != NULL)   
  {
    for (i = 0; i < node->layer.nb_neurone; i++)
    {
      node->layer.neurone[i].biais -= (l_rate / nb_training_exemples) * node->layer.neurone[i].delta_biais;
      for (j = 0; j < node->layer.nb_entree; j++)
      {
        node->layer.neurone[i].poids[j] -= (l_rate / nb_training_exemples) * node->layer.neurone[i].delta_poids[j];
      }
    }
    node = node->prec;
  }

  // remise à 0 des deltas pour les couche L, L-1, L-2, etc.
  node = network.queue;
  while (node != NULL)
  {
    for (i = 0; i < node->layer.nb_neurone; i++)
    {
      node->layer.neurone[i].delta_biais = 0;

      for (j = 0; j < node->layer.nb_entree; j++)
      {
        node->layer.neurone[i].delta_poids[j] = 0;
      }
    }
    node = node->prec;
  }
}

// fonction qui entraine le réseau avec un set d'images
void trainNetwork(reseau_t network, dataset_t ds, int ind_first_image_dataset, int ind_last_image_dataset, double learning_rate)
{
  int i,j,k;
  int nb_training_exemples = (ind_last_image_dataset - ind_first_image_dataset) + 1;  
  int nb_sous_groupes = 50;
  int nb_images_par_sous_groupe = nb_training_exemples / nb_sous_groupes;

  for (j = 0; j < nb_sous_groupes; j++)
  {
    printf("----- Debut de l'epoque %d / %d -----\n", j, nb_sous_groupes);

    for (k = 0; k < 10; k++)
    {
      for (i = 0; i <= nb_images_par_sous_groupe; i++)
      {
        feedForward(network, ds.data[ind_last_image_dataset + (nb_sous_groupes * j + i)].pixel);
        backPropagation(network, ds.data[i].number_expected);
      }
      printf("gradientDescent : correction...\n");
      gradientDescent(network, learning_rate, nb_images_par_sous_groupe);
    }
    printf("----- Fin de l'epoque %d / %d -----\n", j, nb_sous_groupes);
  }
}


// ---------------------------------------------- Main ---------------------------------------------- //
int main(void)
{
  srand(time(NULL));

  //reseau_t reseau = createNetworkFromFile(2, "network_30_10.csv");
  //printNetwork(reseau);

  //dataset_t ds;
  //ds = extractDataImg("images_data.csv");
  
  //testNetwork(reseau, ds);

  int nb_layers = 2;
  int nb_entrees_1st_layer = 784;
  int nb_neurones_layer[2] = {30 , 10};

  dataset_t ds;
  ds = extractDataImg("images_data.csv");
  
  reseau_t reseau = createNetwork(nb_layers, nb_entrees_1st_layer, nb_neurones_layer);

  printNetwork(reseau);
  trainNetwork(reseau, ds, 0, 30000, 1);
  testNetwork(reseau, ds);
  return EXIT_SUCCESS;
}