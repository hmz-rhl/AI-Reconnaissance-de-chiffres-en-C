Nom du projet : Intelligence artificielle
Binôme : RAHAL Hamza / CHOUT Jawad
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Date de la séance : Vendredi 21 Mai 2021


Bonjour monsieur Dubessy, 

Voici nos réalisations de cette séance :

-> Tout d'abord, nous avons atteint l'objectif de la séance dernière qui était de "Créer et initialiser un réseau". 
-> Nous avons mis au point la fonction "layer_t createLayer(int nb_neurones, int nb_entrees)" qui va nous permettre de créer un layer vide.
-> Nous avons mis au point la fonction "void addLayerToNetwork(reseau_t* network, layer_t layer)" qui va nous permettre d'ajouter un layer dans le réseau.
-> Nous avons mis au point la fonction "reseau_t createNetworkFromFile(int nb_layers, const char* filename)" qui va nous permettre de créer et initialiser un réseau à partir d'un fichier .csv (ce fichier nous fournit les poids
et le biais pour chaque neurone de chaque layer). En initialisant ce réseau avec ces coefficients, on obtient un réseau déjà entrainé, capable de reconnaitre les chiffres de 0 à 9 avec une grande précision.
-> Enfin, on a définit la fonction "void testNetwork(reseau_t network, dataset_t ds)", qui va tester la performance du réseau entraîné (càd sa capacité à reconnaître le chiffre de l'image) à l'aide du dataset créé
à partir du fichier "images_data.csv". On obtient alors un très bon résultat : ce réseau entraîné reconnaît les chiffres avec un taux de reconnaissance de 97,658333% !


Objectif suivant : Débuter l'apprentissage d'un réseau non entrainé.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


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
