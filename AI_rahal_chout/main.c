#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "IA.h"

// Définitions des fonctions
// fonction sigmoide retourne valeur entre 0 et 1
double sigmoide(double output)
{
  return 1.0 / (1.0 + exp(-output));
}

// fonction nombre réel aleatoire
double aleat(int a, int b)
{
  return (double)((rand() / (double)RAND_MAX) * (b - a) + a);
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
      ds.data[k].number_expected = atoi(extracted);
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

int main(void)
{
  dataset_t ds;
  ds = extractDataImg("test_images.csv");

  saveBMP(ds.data[0].pixel, ds.rows, ds.cols, "1.bmp");      
  saveBMP(ds.data[1].pixel, ds.rows, ds.cols, "2.bmp");
  saveBMP(ds.data[2].pixel, ds.rows, ds.cols, "3.bmp");

  return EXIT_SUCCESS;
}