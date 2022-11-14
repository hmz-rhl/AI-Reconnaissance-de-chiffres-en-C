#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "IA.h"


// ---------------------------------------------- Main ---------------------------------------------- //
int main(void)
{
    srand((unsigned int)time(NULL));

    int i;
    int return_scanf;
    int reseau_charge = 0;
    int dataset_charge = 0;
    int nb_layers;
    int nb_neurones;
    int nb_entrees_1st_layer = 784;
    int* nb_neurones_layer;
    int nb_images_par_sous_groupe;
    int nb_epoch;
    double learning_rate;

    char menu = '0';
    char network_filename[30] = "";
    char data_filename[30] = "";

    reseau_t reseau = { NULL, NULL };
    dataset_t ds = { 0, 0, 0, 0, NULL };

    while (menu != '8')
    {
        printf("Projet Intelligence Artificielle - par CHOUT Jawad (ING1 Telecom) et RAHAL Hamza (ING1 Instrumentation)\n");
        putchar('\n');
        printMenu();
        printf("  -> Tapez au clavier le chiffre correspondant a l'action que vous voulez effectuer : ");

        return_scanf = scanf("%c", &menu);
        emptyBuffer();
        putchar('\n');

        if (menu == LOAD_DATA)
        {
            printf("  -> Entrez le nom du fichier contenant le dataset d'images (par exemple : images_data.csv) : ");
            return_scanf = scanf("%s", data_filename);
            emptyBuffer();

            ds = extractDataImg(data_filename);
            dataset_charge = 1;
            printf("Fichier charge !\n");
            putchar('\n');
            waitSeconds(3);
        }

        if (menu == LOAD_NETWK)
        {
            printf("  -> Entrez le nom du fichier contenant le reseau de neurones deja entraine (par exemple : network_30_10.csv) : ");
            return_scanf = scanf("%s", network_filename);
            emptyBuffer();
            reseau = createNetworkFromFile(2, network_filename);
            printf("Reseau de neurones charge a partir du fichier '%s' !\n", network_filename);
            reseau_charge = 1;
            putchar('\n');
            waitSeconds(4);
        }

        if (menu == CREATE_NETWK)
        {
            printf("  -> Entrez le nombre de layers du reseau : ");
            return_scanf = scanf("%d", &nb_layers);
            emptyBuffer();
            putchar('\n');
            nb_neurones_layer = calloc(nb_layers, sizeof(int));

            for (i = 0; i < nb_layers; i++)
            {
                printf("  -> Entrez le nombre de neurones du layer %d : ", i + 1);
                return_scanf = scanf("%d", &nb_neurones);
                emptyBuffer();
                putchar('\n');
                nb_neurones_layer[i] = nb_neurones;
            }

            reseau = createNetwork(nb_layers, nb_entrees_1st_layer, nb_neurones_layer);
            printf("Le reseau a ete cree !");
            reseau_charge = 1;
            waitSeconds(4);
        }

        if (menu == PRINT_NETWK)
        {
            if (reseau_charge)
            {
                printNetwork(reseau);
            }
            else
            {
                printf("Erreur : Aucun reseau n'a ete charge/cree\n");
            }
            waitSeconds(5);
        }

        if (menu == TRAIN)
        {
            if (reseau_charge && dataset_charge)
            {
                printf("  -> Entrez le nombre d'images par sous-groupes : ");
                return_scanf = scanf("%d", &nb_images_par_sous_groupe);
                emptyBuffer();
                putchar('\n');

                while ((nb_images_par_sous_groupe <= 0) || (nb_images_par_sous_groupe > ds.nb_images))
                {
                    printf("  -> Entrez un nombre d'images par sous-groupes VALIDE (entre 1 et 60000) : ");
                    return_scanf = scanf("%d", &nb_images_par_sous_groupe);
                    emptyBuffer();
                    putchar('\n');
                }

                printf("  -> Entrez le nombre d'epoques : ");
                return_scanf = scanf("%d", &nb_epoch);
                emptyBuffer();
                putchar('\n');

                while (nb_epoch <= 0)
                {
                  printf("  -> Entrez un nombre d'epoques VALIDE (> 0) : ");
                  return_scanf = scanf("%d", &nb_epoch);
                  emptyBuffer();
                  putchar('\n');
                }
                
                printf("  -> Entrez le learning rate : ");
                return_scanf = scanf("%lf", &learning_rate);
                emptyBuffer();
                putchar('\n');

                while (learning_rate <= 0)
                {
                    printf("  -> Entrez un learning rate VALIDE (> 0) : ");
                    return_scanf = scanf("%lf", &learning_rate);
                    emptyBuffer();
                    putchar('\n');
                }
                printf("Une notification sonore vous avertira de la fin de l'entrainement !\n\n");
                waitSeconds(5);
                printf("Debut de l'entrainement...\n");
                trainNetwork(reseau, ds, ds.nb_images, nb_images_par_sous_groupe, nb_epoch, learning_rate);
                printf("Entrainement du reseau termine !\a\n");
            }
            else
            {
                if (reseau_charge == 0)
                {
                    printf("Erreur : Aucun reseau n'a ete charge/cree\n");
                }
                if (dataset_charge == 0)
                {
                    printf("Erreur : Aucun dataset n'a ete charge/cree\n");
                }
            }
            waitSeconds(4);
        }

        if (menu == TEST)
        {
            if (reseau_charge && dataset_charge)
            {
                printf("Test sur < %s >\n", data_filename);
                waitSeconds(4);
                testNetwork(reseau, ds);
            }
            else
            {
                if (reseau_charge == 0)
                {
                    printf("Erreur : Aucun reseau n'a ete charge/cree\n");
                }
                if (dataset_charge == 0)
                {
                    printf("Erreur : Aucun dataset n'a ete charge/cree\n");
                }
            }
            waitSeconds(6);
        }

        if (menu == SAVE_NETWK)
        {
            if (reseau_charge)
            {
                printf("  -> Entrez le nom du fichier de sauvegarde : ");
                return_scanf = scanf("%s", network_filename);
                emptyBuffer();
                saveNetwork(reseau, network_filename);
                waitSeconds(3);
                printf("Reseau correctement sauvegarde dans le fichier '%s'\a\n", network_filename);
            }
            else
            {
                printf("Erreur : Aucun reseau n'a ete charge/cree\n");
            }
            waitSeconds(4);
        }
        system("@cls||clear");
    }
    return EXIT_SUCCESS;
}
