/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   graph.h                                       :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#ifndef PROJET_H
# define PROJET_H
# include <limits.h>
# include <math.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

# define BUFFER_LIGNE 256

/*
cette structure représente le sommet d'un graphe
l'id nous sert à identifier le sommet car les numéros de sommets ne correspondent pas aux indices
le nombre de voisins est particulièrement utile pour le calcul de degrés
le tableau de voisins contient les ids des sommets voisins
*/

typedef struct
{
	int				id;
	int				nombre_voisins;
	int				capacite_voisins;
	// cela nous sert dans les fonctions de création de graphe pour allouer de
	// l'espace supplémentaire (*2) si jamais on dépasse la capacité actuelle
	int				*voisins;
}					SommetGraphe;

/*
cette structure représente un graphe non orienté
*/

typedef struct
{
	int				nombre_sommets;
	SommetGraphe	*sommets;
}					Graphe;

/*
structure temporaire pour représenter une arête dans un graphe
utilisée notamment pour les questions sur la couverture de sommets
les indices font référence aux indices des sommets dans le tableau de sommets du graphe
*/

typedef struct
{
	int				indice_u;
	int				indice_v;
}					AreteGraphe;

typedef struct Arbre
{
	int				id;
	int				size;
	int				n_nodes;
	struct Arbre	*nodes;
}					Arbre;

/* init.c */

SommetGraphe		creer_sommet(int id);
Graphe				*creer_graphe(int nombre_sommets, const int *ids_sommets);
Graphe				*graphe_aleatoire(int nombre_sommets,
						double probabilite_arete, unsigned int graine);

/* free.c */

void				liberer_sommet(SommetGraphe *sommet);
void				liberer_graphe(Graphe *graphe);

/* voisins.c */

int					eviter_doublon(const SommetGraphe *sommet, int id_voisin);
int					ajouter_voisin(SommetGraphe *sommet, int id_voisin);
void				supprimer_voisin(SommetGraphe *sommet, int id_voisin);

/* utils.c */

Graphe				*copie_nouveau_graphe(const Graphe *graphe,
						const int *indicateurs_ignorer);
int					trouver_indice(const Graphe *graphe, int id_sommet);
AreteGraphe			*obtenir_aretes(const Graphe *graphe, int *nombre_aretes);
int					ajouter_arete(Graphe *graphe, int id_u, int id_v);
void				calculer_degres(const Graphe *graphe, int *degres);
int					sommet_degre_max(const Graphe *graphe, int *valeur_degre);
AreteGraphe			*obtenir_aretes(const Graphe *graphe, int *nombre_aretes);
int					nombre_sommets(const Graphe *graphe);
int					id_sommet(const Graphe *graphe, int indice);

Arbre				new_arbre(int id);
int					ajouter_noeud(Arbre *a, Arbre n);

/* couverture.c */

int					*couverture_sommets_couplage(const Graphe *graphe,
						int *taille_couverture);
int					*couverture_sommets_glouton(const Graphe *graphe,
						int *taille_couverture);

/* fichier.c */

int					parser_fichier(FILE *f, char *ligne);
Graphe				*charger_fichier(const char *chemin);

/* suppression.c */

Graphe				*supprimer_sommet(const Graphe *graphe, int id_sommet);
Graphe				*supprimer_liste_sommets(const Graphe *graphe,
						const int *ids_sommets, int nombre_a_supprimer);

/* branch.c */

void    branchementRecursion(Arbre *arbre, Graphe *g);
Arbre				branchement1(Graphe g);
void				rechercheMin(Arbre *a, int *sizeSolution, int niveau,
						int *couvertureTmp, int *solution);

/* bound.c */

int					calculer_borne_inferieure(const Graphe *graphe);
Arbre				branchement_avec_bornes(Graphe g, int *best_size,
						int **best_solution);

/* branch_ameliore.c */

int	*couverture_branchement_ameliore(Graphe *g, int *taille);

/* branch_common.c */

int	*calculer_couverture_branchement_generique(Graphe *graphe, int *taille,
		int version_algo); 	

#endif
