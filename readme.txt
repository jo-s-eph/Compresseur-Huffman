Compiler le code : 
make 

executer le code : 
./main.o

Deux fichiers pour effectuer des tests :
    - test1.txt : 4595 caractères
    - test2.txt : 32185 caractères

Le code se divise en plusieurs parties :

- huffman.c : Contient une structure de tableau ainsi que les fonctions d'encodage et de decodage de huffman
- text.c : Contient les fonctions necessaires pour traiter du texte (lire, encoder, decoder) ainsi que des fonctions annexes
- tree.c : Contient la structure d'un arbre binaire modifié avec ses fonctions associées
- tas.c : Contient la structure d'un TAS min. modifié avec ses fonctions associées

Le fichier main.c apelle l'ensemble necessaire afin d'encoder ou de decoder en fonction du choix de l'utilisateur.
Une fois executé, suivez les indications du menu et passez toujours des fichiers existants.

Après compilation, le fichier qui en résulte prend le nom suivant : [NOM].b
Après décompilation, le fichier qui en résulteprend le nom suivant : r_[NOM].txt

Problèmes à fix :
- Memory leak en utilisant gcc pour compiler et non clang.
- Prend en charge que les 128 caractères ASCII donc pas d'accents, de lettres grecques etc..
- Optimisations possibles sur la gestion de mëmoire.
- Utiliser des args plutôt qu'un menu (Ex : "./main.o -C test1.txt" Pour compresser le fichier)