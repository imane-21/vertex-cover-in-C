/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   branch.c                                      :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"

void	branchementRecursion(Arbre *arbre, Graphe *g)
{
	int			nbAretes;
	AreteGraphe	*aretes;
	Arbre		u;
	Arbre		v;
	int			idx_node_u;
	int			idx_node_v;
	int			nSommets;
	int			*ignore_u;
	int			*ignore_v;
	Graphe		*g_u;
	Graphe		*g_v;

	// les deux variables suivantes c'est pour la position des enfants u et v dans le tableau arbre->nodes
	// ignore_u et ignore_v nous permettent de savoir quels sommets ignorer pour créer les sous-graphes
	// sous graphes créés pour les deux branches
	int idx_u, idx_v; // ici c'est les indices INTERNES donc de 0 à n-1
	int id_u, id_v;
	// mais là c'est les IDs des sommets donc ca peut être n'importe quel entier
	aretes = obtenir_aretes(g, &nbAretes);
	if (nbAretes > 0 && aretes)
	// base de la recursion : tant qu'il y a des arêtes
	{
		idx_u = aretes[0].indice_u;
		idx_v = aretes[0].indice_v;
		id_u = id_sommet(g, idx_u);
		id_v = id_sommet(g, idx_v);
		u = new_arbre(id_u);
		v = new_arbre(id_v);
		ajouter_noeud(arbre, u);
		ajouter_noeud(arbre, v);
		idx_node_u = arbre->n_nodes - 2;
		idx_node_v = arbre->n_nodes - 1;
		nSommets = nombre_sommets(g);
		ignore_u = calloc(nSommets, sizeof(int));
		ignore_v = calloc(nSommets, sizeof(int));
		if (!ignore_u || !ignore_v)
		{
			perror("Erreur d'allocation indicateurs");
			exit(EXIT_FAILURE);
		}
		ignore_u[idx_u] = 1;
		ignore_v[idx_v] = 1;
		g_u = copie_nouveau_graphe(g, ignore_u);
		g_v = copie_nouveau_graphe(g, ignore_v);
		free(ignore_u);
		free(ignore_v);
		branchementRecursion(&arbre->nodes[idx_node_u], g_u);
		// on recurse sur u (avant dernière position des noeuds)
		branchementRecursion(&arbre->nodes[idx_node_v], g_v);
		// on recurse sur v (dernière position des noeuds)
		liberer_graphe(g_u);
		liberer_graphe(g_v);
	}
	free(aretes);
}

Arbre	branchement1(Graphe g)
{
	Arbre	arbre;

	arbre = new_arbre(-1);
	branchementRecursion(&arbre, &g);
	return (arbre);
}

/*
 Fonction partagée pour extraire la solution minimale de l'arbre
 */
void	rechercheMin(Arbre *a, int *sizeSolution, int niveau,
		int *couvertureTmp, int *solution)
{
	int	nouveau_niveau;
	int	i;
	int	est_conteneur_sommets;

	if (a == NULL)
		return ;
	nouveau_niveau = niveau;
	// Si c'est un nœud conteneur (-1) avec des enfants, ajouter tous les enfants
	if (a->id == -1 && a->n_nodes > 0)
	{
		// Vérifier si c'est un conteneur de sommets (tous les enfants ont id !=-1)
		est_conteneur_sommets = 1;
		for (i = 0; i < a->n_nodes; i++)
		{
			if (a->nodes[i].id == -1)
			{
				est_conteneur_sommets = 0;
				break ;
			}
		}
		// Si c'est un conteneur de sommets ET qu'on n'est pas à la racine
		if (est_conteneur_sommets && niveau > 0)
		{
			// Ajouter tous les sommets du conteneur à la solution
			for (i = 0; i < a->n_nodes; i++)
			{
				couvertureTmp[nouveau_niveau] = a->nodes[i].id;
				nouveau_niveau++;
			}
			// Pour trouver les prochains nœuds, on doit chercher parmi
			// les enfants des sommets du conteneur
			// On prend le premier sommet qui a des enfants
			for (i = 0; i < a->n_nodes; i++)
			{
				if (a->nodes[i].n_nodes > 0)
				{
					// Explorer les enfants de ce sommet
					for (int j = 0; j < a->nodes[i].n_nodes; j++)
					{
						rechercheMin(&a->nodes[i].nodes[j], sizeSolution,
							nouveau_niveau, couvertureTmp, solution);
					}
					return ; // On a exploré, on sort
				}
			}
			// Si aucun enfant n'a de descendants, c'est une feuille
			if (*sizeSolution > nouveau_niveau)
			{
				*sizeSolution = nouveau_niveau;
				for (i = 0; i < *sizeSolution; i++)
				{
					solution[i] = couvertureTmp[i];
				}
			}
			return ;
		}
		// Sinon c'est la racine : explorer les enfants sans les ajouter
		else if (niveau == 0)
		{
			for (i = 0; i < a->n_nodes; i++)
			{
				rechercheMin(&a->nodes[i], sizeSolution, niveau, couvertureTmp,
					solution);
			}
			return ;
		}
	}
	// Sinon, c'est un sommet normal (id != -1)
	else if (a->id != -1)
	{
		couvertureTmp[niveau] = a->id;
		nouveau_niveau = niveau + 1;
	}
	// Si c'est une feuille, vérifier si on a une meilleure solution
	if (a->n_nodes == 0)
	{
		if (*sizeSolution > nouveau_niveau)
		{
			*sizeSolution = nouveau_niveau;
			for (i = 0; i < *sizeSolution; i++)
			{
				solution[i] = couvertureTmp[i];
			}
		}
		return ;
	}
	// Explorer récursivement tous les enfants
	for (i = 0; i < a->n_nodes; i++)
	{
		rechercheMin(&a->nodes[i], sizeSolution, nouveau_niveau, couvertureTmp,
			solution);
	}
}
