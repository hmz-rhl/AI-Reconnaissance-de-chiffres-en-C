Nom du projet : Intelligence artificielle
Binôme : RAHAL Hamza / CHOUT Jawad

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Date de la séance : Vendredi 14 Mai 2021


Bonjour monsieur Dubessy, 

Voici nos réalisations de cette séance :

-> Premièrement, nous avons, suite à vos recommandations, modifié nos structures "neurone_s" et "layer_s" : elles sont désormais opérationnelles.

-> Ensuite, nous avons implémenté la fonction dont le prototype est "dataset_t extractDataImg(const char* filename);" : cette fonction permet 
d'extraire, d'un fichier, les caractéristiques (càd valeur de 0 à 255 des pixels) de l'image afin de les stocker dans une variable de type structuré 
"dataset_t".

-> Le type "dataset_t" contient un champ correspondant au nombre d'images, un champ à la largeur de l'image, un champ à la hauteur de l'image, 
un champ à la taille (hauteur * largeur) de l'image, et un tableau data de type structuré "subdata_t".

-> Le type "subdata_t" contient un champ correspondant au chiffre que l'on peut apercevoir sur l'image et un tableau pixel de
type "unsigned char" (ce tableau contient la valeur des pixels, de 0 à 255).

-> Enfin, dans le main, nous avons pu générer, à l'aide de la fonction "saveBMP" fournie, 3 images dont les données ont été extraite du 
fichier "test_images.csv" (la 1re ligne de ce fichier contenant le nombre, et la taille des images, et les 3 dernières lignes (une ligne = une image) contiennent la valeur des
pixels de chaque image. Ces images seront stockés dans le même dossier source que le .c, le .h, etc.


Objectif suivant : Créer et initialiser un réseau.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
