#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <time.h>

// constante
#define N_NEURONES_ENTREE 784
#define N_NEURONE_SORTIE 10
#define N_NEURONE_CACHEE 16
#define READ_MAX 1000



// structure pour un neurone
typedef struct neurone_s
{
  int nb;			      // nombre d'entrées
  double* poids;		// un tableau de poids
  double biais;		  // une valeur de biais
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
  return (double)((rand() / (double)RAND_MAX) * (b - a) + a);
}


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

void extractDataImg(const char* filename)
{
  FILE* file;
  char chaine[READ_MAX] = "";
  unsigned char data[784] = { 0 };
  int i, j, nb_images, rows, cols, number_expected, nb_zeros;
  unsigned char pixel;
  file = fopen(filename, "r");
  if (file == NULL)
    printf("Error: could not open file '%s' !\n", filename);
  else
  {
    fgets(chaine, READ_MAX, file);
    char* extracted = strtok(chaine, ";");
    nb_images = atoi(extracted);

    extracted = strtok(NULL, ";");
    rows = atoi(extracted);

    extracted = strtok(NULL, ";");
    cols = atoi(extracted);

    while (fgets(chaine, READ_MAX, file) != NULL) 
    {
      i = 0;
      char* extracted2 = strtok(chaine, ";");
      number_expected = atoi(extracted);
      extracted2 = strtok(NULL, ";");

      while (extracted2 != NULL)
      {                    
        if (strchr(extracted2, 44) != NULL)                     //  On cherche la présence d'une virgule
        {
          char* a = &extracted2[2];
          nb_zeros = atoi(a);
          for (j = 0; j < nb_zeros; j++)
          {
            data[i] = 0;
            i++;
          }
        }
        else
        {
          pixel = (unsigned char)atoi(extracted2);
          data[i] = pixel;
          i++;
        }
        extracted2 = strtok(NULL, ";");
      }
      saveBMP(data, rows, cols, "1.bmp");
    }
    fclose(file);
  }
}


int main(void)
{
  extractDataImg("test_images.txt");
  return EXIT_SUCCESS;
}