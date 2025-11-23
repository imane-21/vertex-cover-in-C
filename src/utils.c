/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   utils.c                                       :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"

/* fonctions utilitaires pour les opérations de base */

int	ajouter_sommet(Graphe *graphe, SommetGraphe u)
{
	graphe->nombre_sommets++;
	graphe->sommets = realloc(graphe->sommets, sizeof(SommetGraphe)
			* graphe->nombre_sommets);
	graphe->sommets[(graphe->nombre_sommets) - 1] = u;
	return (0);
}

Arbre	new_arbre(int id)
{
	Arbre	arbre;

	arbre.id = id;
	arbre.nodes = malloc(sizeof(Arbre) * 2);
	arbre.size = 2;
	arbre.n_nodes = 0;
	return (arbre);
}

int	ajouter_noeud(Arbre *a, Arbre n)
{
	Arbre	*nouveaux_nodes;

	if (a->n_nodes >= a->size)
	{
		a->size *= 2;
		nouveaux_nodes = realloc(a->nodes, sizeof(Arbre) * a->size);
		if (!nouveaux_nodes)
		{
			perror("Memory allocation error");
			return (-1);
		}
		a->nodes = nouveaux_nodes;
	}
	a->nodes[a->n_nodes] = n;
	a->n_nodes++;
	return (0);
}

int	ajouter_arete(Graphe *graphe, int id_u, int id_v)
{
	int	indice_u;
	int	indice_v;

	if (!graphe || id_u == id_v)
	{
		return (-1);
	}
	indice_u = trouver_indice(graphe, id_u);
	indice_v = trouver_indice(graphe, id_v);
	if (indice_u < 0 || indice_v < 0)
	{
		return (-1);
	}
	if (ajouter_voisin(&graphe->sommets[indice_u], id_v) != 0)
	{
		return (-1);
	}
	if (ajouter_voisin(&graphe->sommets[indice_v], id_u) != 0)
	{
		supprimer_voisin(&graphe->sommets[indice_u], id_v);
			// si jamais l'ajout échoue on annule l'autre ajout
		return (-1);
	}
	return (0);
}

Graphe	*copie_nouveau_graphe(const Graphe *graphe,
		const int *indicateurs_ignorer)
{
	int		nouveau_nombre;
	int		*ids;
	int		curseur;
	Graphe	*resultat;
	int		id_voisin;
	int		indice_voisin;

	nouveau_nombre = 0;
	for (int i = 0; i < graphe->nombre_sommets; ++i)
	{
		if (!indicateurs_ignorer[i])
		{
			++nouveau_nombre;
		}
	}
	ids = malloc(sizeof(int) * nouveau_nombre);
	if (!ids)
	{
		return (NULL);
	}
	curseur = 0;
	for (int i = 0; i < graphe->nombre_sommets; ++i)
	{
		if (!indicateurs_ignorer[i])
		{
			ids[curseur++] = graphe->sommets[i].id;
		}
	}
	resultat = creer_graphe(nouveau_nombre, ids);
	free(ids);
	if (!resultat)
	{
		return (NULL);
	}
	for (int i = 0; i < graphe->nombre_sommets; ++i)
	{
		if (indicateurs_ignorer[i])
		{
			continue ;
		}
		for (int j = 0; j < graphe->sommets[i].nombre_voisins; ++j)
		{
			id_voisin = graphe->sommets[i].voisins[j];
			indice_voisin = trouver_indice(graphe, id_voisin);
			if (indice_voisin >= 0 && !indicateurs_ignorer[indice_voisin])
			{
				ajouter_arete(resultat, graphe->sommets[i].id, id_voisin);
			}
		}
	}
	return (resultat);
}

int	trouver_indice(const Graphe *graphe, int id_sommet)
{
	for (int i = 0; i < graphe->nombre_sommets; ++i)
	{
		if (graphe->sommets[i].id == id_sommet)
		{
			return (i);
		}
	}
	return (-1);
}

void	calculer_degres(const Graphe *graphe, int *degres)
{
	for (int i = 0; i < graphe->nombre_sommets; ++i)
	{
		degres[i] = graphe->sommets[i].nombre_voisins;
	}
}

int	sommet_degre_max(const Graphe *graphe, int *valeur_degre)
{
	int	indice_max;
	int	degre_max;

	if (graphe->nombre_sommets == 0)
	{
		if (valeur_degre)
		{
			*valeur_degre = 0;
		}
		return (-1);
	}
	indice_max = 0;
	degre_max = graphe->sommets[0].nombre_voisins;
	for (int i = 1; i < graphe->nombre_sommets; ++i)
	{
		if (graphe->sommets[i].nombre_voisins > degre_max)
		{
			degre_max = graphe->sommets[i].nombre_voisins;
			indice_max = i;
		}
	}
	if (valeur_degre)
	{
		*valeur_degre = degre_max;
	}
	return (graphe->sommets[indice_max].id);
}

AreteGraphe	*obtenir_aretes(const Graphe *graphe, int *nombre_aretes)
{
	int			capacite;
	int			nombre;
	AreteGraphe	*aretes;
	int			id_voisin;
	int			indice_voisin;
	AreteGraphe	*nouvelles_aretes;

	capacite = 4;
	nombre = 0;
	aretes = malloc(sizeof(AreteGraphe) * capacite);
	if (!aretes)
	{
		return (NULL);
	}
	for (int i = 0; i < graphe->nombre_sommets; ++i)
	{
		for (int j = 0; j < graphe->sommets[i].nombre_voisins; ++j)
		{
			id_voisin = graphe->sommets[i].voisins[j];
			indice_voisin = trouver_indice(graphe, id_voisin);
			if (indice_voisin > i)
			{
				if (nombre == capacite)
				{
					capacite *= 2;
					nouvelles_aretes = realloc(aretes, sizeof(AreteGraphe)
							* capacite);
					if (!nouvelles_aretes)
					{
						free(aretes);
						return (NULL);
					}
					aretes = nouvelles_aretes;
				}
				aretes[nombre].indice_u = i;
				aretes[nombre].indice_v = indice_voisin;
				++nombre;
			}
		}
	}
	if (nombre_aretes)
	{
		*nombre_aretes = nombre;
	}
	return (aretes);
}

int	nombre_sommets(const Graphe *graphe)
{
	return (graphe->nombre_sommets);
}

/*
fonction utilitaire lorsqu'on manipule des sommets par leur indice
*/
int	id_sommet(const Graphe *graphe, int indice)
{
	if (indice < 0 || indice >= graphe->nombre_sommets)
	{
		return (-1);
	}
	return (graphe->sommets[indice].id);
}