/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   couverture.c                                  :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"

/* ici questions 3.2 c'est juste l'algo donné qui a été refait en C */

int	*couverture_sommets_couplage(const Graphe *graphe, int *taille_couverture)
{
	int			nombre_aretes;
	AreteGraphe	*aretes;
	int			*indicateurs_couverture;
	int			nombre_selectionnes;
	int			u;
	int			v;
	int			*couverture;
	int			curseur;

	nombre_aretes = 0;
	aretes = obtenir_aretes(graphe, &nombre_aretes);
	if (!aretes)
	{
		return (NULL);
	}
	indicateurs_couverture = calloc((size_t)graphe->nombre_sommets,
			sizeof(int));
	nombre_selectionnes = 0;
	for (int i = 0; i < nombre_aretes; ++i)
	{
		u = aretes[i].indice_u;
		v = aretes[i].indice_v;
		if (!indicateurs_couverture[u] && !indicateurs_couverture[v])
		{
			indicateurs_couverture[u] = 1;
			indicateurs_couverture[v] = 1;
			nombre_selectionnes += 2;
		}
	}
	free(aretes);
	couverture = malloc(sizeof(int) * nombre_selectionnes);
	if (!couverture)
	{
		free(indicateurs_couverture);
		return (NULL);
	}
	curseur = 0;
	for (int i = 0; i < graphe->nombre_sommets; ++i)
	{
		if (indicateurs_couverture[i])
		{
			couverture[curseur++] = graphe->sommets[i].id;
		}
	}
	free(indicateurs_couverture);
	if (taille_couverture)
	{
		*taille_couverture = nombre_selectionnes;
	}
	return (couverture);
}

int	*couverture_sommets_glouton(const Graphe *graphe, int *taille_couverture)
{
	int			aretes_restantes;
	AreteGraphe	*aretes;
	int			*degres;
	int			*aretes_couvertes;
	int			*dans_couverture;
	int			nombre_couvertes;
	int			*couverture;
	int			longueur_couverture;
	int			valeur_degre_max;
	int			indice_choisi;
	int			autre;

	aretes_restantes = 0;
	aretes = obtenir_aretes(graphe, &aretes_restantes);
	if (!aretes)
	{
		return (NULL);
	}
	degres = malloc(sizeof(int) * graphe->nombre_sommets);
	aretes_couvertes = calloc((size_t)aretes_restantes, sizeof(int));
	dans_couverture = calloc((size_t)graphe->nombre_sommets, sizeof(int));
	if (!degres || !aretes_couvertes || !dans_couverture)
	{
		free(aretes);
		free(degres);
		free(aretes_couvertes);
		free(dans_couverture);
		return (NULL);
	}
	calculer_degres(graphe, degres);
	nombre_couvertes = 0;
	couverture = malloc(sizeof(int) * graphe->nombre_sommets);
	if (!couverture)
	{
		free(aretes);
		free(degres);
		free(aretes_couvertes);
		free(dans_couverture);
		return (NULL);
	}
	longueur_couverture = 0;
	while (nombre_couvertes < aretes_restantes)
	{
		valeur_degre_max = -1;
		indice_choisi = -1;
		for (int i = 0; i < graphe->nombre_sommets; ++i)
		{
			if (!dans_couverture[i] && degres[i] > valeur_degre_max)
			{
				valeur_degre_max = degres[i];
				indice_choisi = i;
			}
		}
		if (indice_choisi < 0)
		{
			break ;
		}
		dans_couverture[indice_choisi] = 1;
		couverture[longueur_couverture++] = graphe->sommets[indice_choisi].id;
		for (int i = 0; i < aretes_restantes; ++i)
		{
			if (aretes_couvertes[i])
			{
				continue ;
			}
			if (aretes[i].indice_u == indice_choisi
				|| aretes[i].indice_v == indice_choisi)
			{
				aretes_couvertes[i] = 1;
				++nombre_couvertes;
				autre = aretes[i].indice_u == indice_choisi ? aretes[i].indice_v : aretes[i].indice_u;
				if (degres[autre] > 0)
				{
					--degres[autre];
				}
			}
		}
		degres[indice_choisi] = 0;
	}
	free(aretes);
	free(degres);
	free(aretes_couvertes);
	free(dans_couverture);
	if (taille_couverture)
	{
		*taille_couverture = longueur_couverture;
	}
	return (couverture);
}
