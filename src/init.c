/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   init.c                                        :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"

/* fonctions pour créer des sommets et des graphes */

SommetGraphe	creer_sommet(int id)
{
	SommetGraphe	v;

	v.id = id;
	v.nombre_voisins = 0;
	v.capacite_voisins = 4; // on commence avec une capacité de 4 voisins et on double si besoin
	v.voisins = malloc(sizeof(int) * v.capacite_voisins);
	if (!v.voisins)
	{
		v.capacite_voisins = 0;
	}
	return (v);
}

Graphe	*creer_graphe(int nombre_sommets, const int *ids_sommets)
{
	Graphe	*graphe;

	graphe = malloc(sizeof(Graphe));
	if (!graphe)
	{
		return (NULL);
	}
	graphe->nombre_sommets = nombre_sommets;
	graphe->sommets = malloc(sizeof(SommetGraphe) * nombre_sommets);
	if (!graphe->sommets)
	{
		free(graphe);
		return (NULL);
	}
	for (int i = 0; i < nombre_sommets; ++i)
	{
		graphe->sommets[i] = creer_sommet(ids_sommets[i]);
		if (!graphe->sommets[i].voisins)
		{
			while (i > 0)
			{
				--i;
				liberer_sommet(&graphe->sommets[i]);
			}
			free(graphe->sommets);
			free(graphe);
			return (NULL);
		}
	}
	return (graphe);
}


/*
besoin d'une graine à cause de la fonction random en C
*/
Graphe	*graphe_aleatoire(int nombre_sommets, double probabilite_arete,
		unsigned int graine)
{
	int		*ids;
	Graphe	*graphe;
	double	r;

	if (nombre_sommets <= 0 || probabilite_arete <= 0.0
		|| probabilite_arete >= 1.0)
	{
		return (NULL);
	}
	ids = malloc(sizeof(int) * nombre_sommets);
	if (!ids)
	{
		return (NULL);
	}
	for (int i = 0; i < nombre_sommets; ++i)
	{
		ids[i] = i;
	}
	graphe = creer_graphe(nombre_sommets, ids);
	free(ids);
	if (!graphe)
	{
		return (NULL);
	}
	srand(graine);
	for (int i = 0; i < nombre_sommets; ++i)
	{
		for (int j = i + 1; j < nombre_sommets; ++j)
		{
			r = (double)rand() / (double)RAND_MAX;
			if (r < probabilite_arete)
			{
				ajouter_arete(graphe, graphe->sommets[i].id,
					graphe->sommets[j].id);
			}
		}
	}
	return (graphe);
}