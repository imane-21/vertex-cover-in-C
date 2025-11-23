/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   voisins.c                                     :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"

/* fonctions pour gérer les voisins d'un sommet dans un graphe */

int	eviter_doublon(const SommetGraphe *sommet, int id_voisin)
{
	for (int i = 0; i < sommet->nombre_voisins; ++i)
	{
		if (sommet->voisins[i] == id_voisin)
		{
			return (1);
		}
	}
	return (0);
}

int	ajouter_voisin(SommetGraphe *sommet, int id_voisin)
{
	int	nouvelle_capacite;
	int	*nouveaux_voisins;

	if (eviter_doublon(sommet, id_voisin))
	{
		return (0);
	}
	if (sommet->nombre_voisins == sommet->capacite_voisins)
	{
		nouvelle_capacite = sommet->capacite_voisins * 2;
		nouveaux_voisins = realloc(sommet->voisins, sizeof(int)
				* nouvelle_capacite);
		if (!nouveaux_voisins)
		{
			return (-1);
		}
		sommet->voisins = nouveaux_voisins;
		sommet->capacite_voisins = nouvelle_capacite;
	}
	sommet->voisins[sommet->nombre_voisins++] = id_voisin;
	return (0);
}

void	supprimer_voisin(SommetGraphe *sommet, int id_voisin)
{
	for (int i = 0; i < sommet->nombre_voisins; ++i)
	{
		if (sommet->voisins[i] == id_voisin)
		{
			sommet->voisins[i] = sommet->voisins[sommet->nombre_voisins - 1];
			--sommet->nombre_voisins;
			return ;
		}
	}
}