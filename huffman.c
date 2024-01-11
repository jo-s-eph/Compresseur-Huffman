#include "huffman.h"

/*
Permet de créer un arbre de huffman à partir d'une forêt (ici un TAS min.)
Chaque noeuds comportent la valeur combiné de ses deux enfants et a pour caractères '\0'.
Chaque feuilles comportent un caractère unique ainsi que sa fréquence.

    CONDITION(S) : - Le TAS doit comporter des feuilles comportant un caractères
                     ainsi que sa fréquence. 
*/
noeud_t* huffman(tas_t *tas) {
    // Tant que la taille du tas est supérieure ou égale à 2
    while (tas->size >= 2){

        // Extraire les 2 minimums du tas et les stocker dans cell1 et cell2
        noeud_t* cell1 = extractMin(tas);
        noeud_t* cell2 = extractMin(tas);
        // Créer un nouveau noeud parent avec la somme des valeurs de cell1 et cell2 et le caractère '\0'
        noeud_t *parent = consA(cell1, cell2, (cell1->v + cell2->v), '\0');

        // Insérer le noeud parent dans le tas
        insererTAS(tas,parent);

        // Si la taille du tas est 1, retourner le parent
        if (tas->size == 1) return parent;
    }
    // Si le tas est vide, retourner NULL
    return NULL;
}

/*
Permet de créer une forêt, représenté dans TAS min., à partir d'un tableau

    CONDITION(S) : - Le tableau comporte 128 entiers où tab[i] correspond à la fréquence de (char)i. 
                        Ex : tab[81] = 5 -> La fréquence de 'Q' ( (char)81 ) est de 5.
*/
tas_t* creerforets(int ascii_o[128]){
    // On initialise une taille correspondant au nombre de caractères uniques
    int taille = nb_char_unique(ascii_o);
    // On initialise un tas vide qui sera notre résultat
    tas_t* res = creerTAS(taille);
    // On parcours le tableau,
    for (int i = 0; i < 128; i++)
        // Si sa fréquence est non-nul, on ajoute une feuille comportant le caractère et que sa fréquence.
        if (ascii_o[i] > 0) insererTAS(res,node_occurence(i,ascii_o));
    return res;
}

/*
Permet de parcourir un arbre de Huffman jusqu'à la racine correspondant au caractère recherché (i.e char target)

  CONDITION(S) : - "huff" est un arbre de Huffman où tab[i] correspond à la fréquence de (char)i 
                 - le caractère recherché est dans l'arbre
                 - La fonction parcours l'arbre donc les accumulateurs code et longueur sont au préalable initialisés.
*/
void encodeChar(noeud_t* huff, int arr[], int top, char target, int* found, char* res) {
    // Si l'arbre est vide ou le caractère est déjà trouvé, on retourne
    if (!huff || *found) return;
    
    // Si il y a un sous-arbre gauche, on ajoute 0 à la séquence et on continue la recherche
    if (huff->g){
        arr[top] = 0;
        encodeChar(huff->g, arr, top + 1, target, found, res);
    }
    // Si il y a un sous-arbre droit, on ajoute 1 à la séquence et on continue la recherche
    if (huff->d){
        arr[top] = 1;
        encodeChar(huff->d, arr, top + 1, target, found, res);
    }

    // Si on est sur une feuille et que le caractère correspond au caractère recherché
    if ((!huff->g && !huff->d) && huff->c == target){
        // On marque le caractère comme trouvé et on copie la séquence dans le résultat
        *found = 1;  
        for (int i = 0; i < top; i++) res[i] = '0' + arr[i]; 
        res[top] = '\0';
    }
}

/*
Permet de remplir une table de codage à partir de son arbre de Huffman dernier.

  CONDITION(S) : - La fonction parcours l'arbre donc les accumulateurs code et longueur sont au préalable initialisés.
*/
void creerTable(noeud_t* huff, unsigned long code, int longueur, table_hm* tableHM) {
    if (huff == NULL) return;

    // Si c'est une feuille, ajoutez le code à la table
    if (huff->g == NULL && huff->d == NULL){
        tableHM->t[(int)huff->c].caractere = huff->c;
        tableHM->t[(int)huff->c].code = code;
        tableHM->t[(int)huff->c].longueurCode = longueur;
        tableHM->taille++;
    
    } 
    else{
        // Parcours des sous-arbres gauche et droit
        creerTable(huff->g, code << 1, longueur + 1, tableHM);
        creerTable(huff->d, (code << 1) | 1, longueur + 1, tableHM);
    }
}

/*
Permet d'encoder un texte à partir de son arbre de Huffman et de sa table de codage.

  CONDITION(S) : - La fonction alloue dynamiquement de la mémoire pour le texte encodé, il faut donc la libérer après utilisation.
*/
char* encoderTexte(noeud_t* abr, table_hm* tbl, char* text) {

    /* le code HM le plus long possible est la hauteur de l'arbre, donc par 
    précaution on alloue cette taille pour chaque caractère. */
    char* texteCode = (char*)malloc(hauteur(abr) * ((int)strlen(text)) + 1);
    
    if (!texteCode) {
        perror("Erreur alloc");
        return NULL;
    }
    texteCode[0] = '\0';

    // Parcours du texte caractère par caractère
    for (int i = 0; text[i] != '\0'; ++i) {    
        // Conversion du code binaire en caractère
        char* code = binary2Char(&(tbl->t[(int)text[i]]));     
        // Ajout du code au texte encodé
        for (int k = 0; code[k] != '\0'; k++) {
            strcat(texteCode, &code[k]);
            break;
        }
    }
    return texteCode;
}

/*
Convertit une chaîne de caractères en binaire.

  CONDITION(S) : - La fonction alloue dynamiquement de la mémoire pour le tableau binaire, il faut donc la libérer après utilisation.
*/
unsigned char* ChainetoBinaire(const char* chaine, size_t* taille) {
    size_t longueurChaine = strlen(chaine);
    size_t tailleBytes = (longueurChaine + 7) / 8; // Calculer le nombre de bytes nécessaires
    unsigned char* binaire = malloc(tailleBytes);
    if (binaire == NULL) return NULL;

    memset(binaire, 0, tailleBytes); // Initialiser le tableau à zéro
    *taille = tailleBytes;

    // Parcours de la chaîne caractère par caractère
    for (size_t i = 0; i < longueurChaine; ++i) {
        // Calculer l'index du byte en divisant l'index du caractère par 8
        size_t idxByte = i / 8;
        // Calculer l'index du bit en prenant le reste de la division de l'index du caractère par 8
        size_t idxBit = i % 8; 
        // Si le caractère est '1', on ajoute 1 au bit correspondant dans le tableau binaire
        if (chaine[i] == '1') {
            binaire[idxByte] |= (1 << (7 - idxBit));
        }
    }

    return binaire;
}

void exportTexte(noeud_t* abr, table_hm* tbl, char* texte, const char* nomFichier) {
    // 1. Encoder le texte
    char* texteEncode = encoderTexte(abr, tbl, texte);

    // 2. Convertir la chaîne encodée en binaire réel
    size_t tailleDonnees;
    unsigned char* donneesBinaire = ChainetoBinaire(texteEncode, &tailleDonnees);
    if (donneesBinaire == NULL) {
        fprintf(stderr, "Erreur lors de la conversion en binaire.\n");
        free(texteEncode);
        return;
    }

    // 3. Ouvrir le fichier en mode append binaire
    FILE* fichier = fopen(nomFichier, "ab");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        free(texteEncode);
        free(donneesBinaire);
        return;
    }

    // 4. Écrire le texte binaire encodé dans le fichier
    fwrite(donneesBinaire, sizeof(unsigned char), tailleDonnees, fichier);

    // 5. Fermer le fichier et libérer la mémoire
    fclose(fichier);
    free(texteEncode);
    free(donneesBinaire);
}