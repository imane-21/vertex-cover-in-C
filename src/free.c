/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   free.c                                        :+:    :+:      :+:    :+:   */
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
fonctions pour libérer les allocations dynamiques
*/

void	liberer_sommet(SommetGraphe *sommet)
{
	free(sommet->voisins);
	sommet->voisins = NULL;
	sommet->capacite_voisins = 0;
	sommet->nombre_voisins = 0;
}


void	liberer_graphe(Graphe *graphe)
{
	if (!graphe)
	{
		return ;
	}
	for (int i = 0; i < graphe->nombre_sommets; ++i)
	{
		liberer_sommet(&graphe->sommets[i]);
	}
	free(graphe->sommets);
	free(graphe);
}