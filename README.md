# Projet UE COMPLEX - Vertex Cover

Projet de M1 sur le problème de couverture de sommets (Vertex Cover), implémenté C.

**Auteurs :** BARAHIMI Imane                 

---

## Description

Implémentation de plusieurs algorithmes pour résoudre le problème NP-complet de couverture de sommets :

- **Algorithme de couplage** (approximation 2-approx)
- **Algorithme glouton** (heuristique basée sur les degrés)
- **Branch and Bound** (solution exacte par branchement avec plusieurs versions améliorées)


---

## Structure du projet

```
complex-projet/
├── includes/
│   └── graph.h           # Déclarations des structures et fonctions
├── src/
│   ├── main.c            # Point d'entrée et fonctions de démonstration
│   ├── branch.c          # Algorithme de branchement (Branch)
│   ├── couverture.c      # Algorithmes d'approximation (couplage, glouton)
│   ├── init.c            # Création de graphes (manuel, aléatoire)
│   ├── utils.c           # Fonctions utilitaires (arêtes, degrés, etc.)
│   ├── voisins.c         # Gestion des listes de voisins
│   ├── suppression.c     # Suppression de sommets
│   ├── fichier.c         # Chargement de graphes depuis fichiers
│   ├── branch_ameliore.c # Branchement version finale avec toutes améliorations
│   ├── branch_common.c   # Fonctions communes aux trois versions du branchement
│   ├── bound.c           # Version du branchement avec bornes
│   └──free               # Libération mémoire
├── Makefile              # Compilation
└── README.md
```

---

## Compilation et exécution

### Compiler (sous Linux)
```bash
make
```

### Exécuter
```bash
./projet_complex 
```

Un menu s'affichera et montrera les choix possibles. 

### Nettoyer
```bash
make clean   # Supprime les .o
make fclean  # Supprime les .o et l'exécutable
make re      # Recompile tout
```

---

## Démonstrations incluses

Le `main` exécute plusieurs démonstrations :

1. **Chargement de fichier** : Lecture d'un graphe depuis un fichier
2. **Suppression de sommets** : Tests de suppression simple et multiple
3. **Calcul de degrés** : Affichage des degrés et sommet de degré max
4. **Graphe aléatoire** : Génération de graphes selon une probabilité
5. **Algorithmes d'approximation** : Comparaison couplage vs glouton
6. **Branchement (exact)** : Arbre de décision et solution optimale

---

## Algorithmes implémentés

### 1. Couverture par couplage 
### 2. Couverture gloutonne 
### 3. Branchement exact


---


### Exemple d'arbre

```
        -1 (racine fictive)
       /  \
      3    1
     /    / \
   (f)   3   6
        (f) / \
           4   2
          (f) (f)

(f) = feuille = couverture complète
```

## Format de fichier graphe

```
Nombre de sommets
n
Sommets
id_sommet_1
id_sommet_2
...
id_sommet_n
Nombre d aretes
m
Aretes
id_sommet_x id_sommet_y
id_sommet_w id_sommet_z
...

```

**Exemple :**
```
Nombre de sommets
5
Sommets
3
1
7
15
4
Nombre d aretes
6
Aretes
3 1
15 7
4 1
7 3
4 15
1 15
```

---


##  Ressources

[site utilisé pour vérifier le Vertex Cover optimal](https://visualgo.net/en/mvc)


**Dernière mise à jour :** novembre 2025
