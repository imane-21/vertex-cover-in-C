/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   suppression.c                                 :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"

/*
fonction principale question 2.1.1
*/
Graphe	*supprimer_sommet(const Graphe *graphe, int id_sommet)
{
	int		*indicateurs_ignorer;
	int		indice;
	Graphe	*resultat;

	if (!graphe)
	{
		return (NULL);
	}
	indicateurs_ignorer = calloc((size_t)graphe->nombre_sommets, sizeof(int));
	if (!indicateurs_ignorer)
	{
		return (NULL);
	}
	indice = trouver_indice(graphe, id_sommet);
	if (indice >= 0)
	{
		indicateurs_ignorer[indice] = 1;
	}
	resultat = copie_nouveau_graphe(graphe, indicateurs_ignorer);
	free(indicateurs_ignorer);
	return (resultat);
}

Graphe	*supprimer_liste_sommets(const Graphe *graphe, const int *ids_sommets,
		int nombre_a_supprimer)
{
	int *indicateurs_ignorer;
	int indice;
	Graphe *resultat;

	if (!graphe)
	{
		return (NULL);
	}
	indicateurs_ignorer = calloc((size_t)graphe->nombre_sommets, sizeof(int));
	if (!indicateurs_ignorer)
	{
		return (NULL);
	}
	for (int i = 0; i < nombre_a_supprimer; ++i)
	{
		indice = trouver_indice(graphe, ids_sommets[i]);
		if (indice >= 0)
		{
			indicateurs_ignorer[indice] = 1;
		}
	}
	resultat = copie_nouveau_graphe(graphe, indicateurs_ignorer);
	free(indicateurs_ignorer);
	return (resultat);
}