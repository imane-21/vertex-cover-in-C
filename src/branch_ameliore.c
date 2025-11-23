/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   branch_ameliore.c                             :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

/*
 Amelioration branchement(4.3) :
 Branche 1 : on prend u dans la couverture
 Branche 2 : on ne prend PAS u, donc on prend TOUS ses voisins
 on choisit u de degré maximum
 */
static int	est_couverture_valide(Graphe *g, int *couverture, int taille)
{
	int			n;
	int			i;
	int			idx;
	int			nbAretes;
	char		*pris;
	AreteGraphe	*aretes_local;

	aretes_local = obtenir_aretes(g, &nbAretes);
	if (!aretes_local)
		return (0);
	if (nbAretes == 0)
	{
		free(aretes_local);
		return (1);
	}
	n = nombre_sommets(g);
	pris = calloc(n, sizeof(char));
	if (!pris)
	{
		perror("Erreur d'allocation validation couverture");
		free(aretes_local);
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < taille; i++)
	{
		idx = trouver_indice(g, couverture[i]);
		if (idx >= 0 && idx < n)
			pris[idx] = 1;
	}
	for (i = 0; i < nbAretes; i++)
	{
		if (!pris[aretes_local[i].indice_u] && !pris[aretes_local[i].indice_v])
		{
			free(pris);
			free(aretes_local);
			return (0);
		}
	}
	free(pris);
	free(aretes_local);
	return (1);
}

typedef struct s_branchement_contexte
{
	int		capacite;
	int		*courant;
	int		*meilleur;
	int		taille_meilleur;
}			BranchementContexte;

static void	explorer_branchement_ameliore(Graphe *g, BranchementContexte *ctx,
		int taille_courante)
{
	int			nbAretes;
	AreteGraphe	*aretes;
	int			borne_min;
	int			n;
	int			taille_couplage;
	int			*couplage;
	int			id_voisin;
	int			idx_voisin;
	int			*ignore;
	Graphe		*g2;
	int			idx_u;
	int			deg_max;
	int			deg;
	int			id_u;
	int			*ignore_u;
	Graphe		*g_u;
	int			*ignore_voisins;
	Graphe		*g_voisins;
	int			taille_suivante;

	// Variables pour traiter les sommets de degré 1
	// Variables pour le choix du sommet u de degré maximum
	// Variables pour la branche 1 : on prend u
	// Variables pour la branche 2 : on ne prend pas u,donc on prend tous ses voisins
	aretes = obtenir_aretes(g, &nbAretes);
	// Cas de base : plus d'arêtes, on a une solution
	if (nbAretes == 0)
	{
		if (aretes)
			free(aretes);
		if (taille_courante < ctx->taille_meilleur)
		{
			ctx->taille_meilleur = taille_courante;
			memcpy(ctx->meilleur, ctx->courant, taille_courante * sizeof(int));
		}
		return ;
	}
	// Élagage : si on a déjà dépassé la meilleure solution, on arrête
	if (taille_courante >= ctx->taille_meilleur)
	{
		if (aretes)
			free(aretes);
		return ;
	}
	// Calcul de la borne inférieure et élagage
	borne_min = calculer_borne_inferieure(g);
	if (taille_courante + borne_min >= ctx->taille_meilleur)
	{
		if (aretes)
			free(aretes);
		return ;
	}
	// Obtenir une solution réalisable via couplage pour améliorer la borne supérieure
	couplage = couverture_sommets_couplage(g, &taille_couplage);
	if (couplage)
	{
		if (est_couverture_valide(g, couplage, taille_couplage)
			&& taille_courante + taille_couplage < ctx->taille_meilleur)
		{
			ctx->taille_meilleur = taille_courante + taille_couplage;
			memcpy(ctx->meilleur, ctx->courant, taille_courante * sizeof(int));
			memcpy(ctx->meilleur + taille_courante, couplage, taille_couplage
				* sizeof(int));
		}
		free(couplage);
	}
	n = nombre_sommets(g);
	// (4.3.3) : Si un sommet est de degré 1,
	// on peut prendre son unique voisin dans la couverture sans brancher
	for (int i = 0; i < n; ++i)
	{
		if (g->sommets[i].nombre_voisins == 1)
		{
			// Sommet de degré 1 trouvé, on prend son voisin
			id_voisin = g->sommets[i].voisins[0];
			idx_voisin = trouver_indice(g, id_voisin);
			if (idx_voisin < 0)
				continue ;
			// Ajouter le voisin à la solution courante
			ctx->courant[taille_courante] = id_voisin;
			// Créer nouveau graphe sans ce voisin
			ignore = calloc(n, sizeof(int));
			if (!ignore)
			{
				perror("Erreur d'allocation branchement (deg1)");
				exit(EXIT_FAILURE);
			}
			ignore[idx_voisin] = 1;
			g2 = copie_nouveau_graphe(g, ignore);
			free(ignore);
			if (aretes)
				free(aretes);
			// Continuer la récursion
			explorer_branchement_ameliore(g2, ctx, taille_courante + 1);
			liberer_graphe(g2);
			return ;
		}
	}
	// (4.3.2) : Choisir le sommet u de degré maximum pour le branchement
	idx_u = -1;
	deg_max = -1;
	for (int i = 0; i < n; ++i)
	{
		deg = g->sommets[i].nombre_voisins;
		if (deg > deg_max)
		{
			deg_max = deg;
			idx_u = i;
		}
	}
	// Si aucun sommet valide n'a été trouvé, on arrête
	if (idx_u < 0 || deg_max <= 0)
	{
		if (aretes)
			free(aretes);
		return ;
	}
	id_u = id_sommet(g, idx_u);
	// Branche 1 : on prend u dans la couverture
	ignore_u = calloc(n, sizeof(int));
	if (!ignore_u)
	{
		perror("Erreur d'allocation branchement (u)");
		exit(EXIT_FAILURE);
	}
	ctx->courant[taille_courante] = id_u;
	ignore_u[idx_u] = 1; // suppression de u uniquement
	g_u = copie_nouveau_graphe(g, ignore_u);
	free(ignore_u);
	explorer_branchement_ameliore(g_u, ctx, taille_courante + 1);
	liberer_graphe(g_u);
	// Branche 2 : on ne prend pas u, donc on doit prendre tous ses voisins
	ignore_voisins = calloc(n, sizeof(int));
	taille_suivante = taille_courante;
	if (!ignore_voisins)
	{
		perror("Erreur d'allocation branchement (voisins)");
		exit(EXIT_FAILURE);
	}
	ignore_voisins[idx_u] = 1; // u n'est pas dans la couverture
	// Ajouter tous les voisins de u à la solution courante
	for (int j = 0; j < g->sommets[idx_u].nombre_voisins; ++j)
	{
		id_voisin = g->sommets[idx_u].voisins[j];
		idx_voisin = trouver_indice(g, id_voisin);
		if (idx_voisin >= 0)
		{
			ctx->courant[taille_suivante++] = id_voisin;
			ignore_voisins[idx_voisin] = 1;
		}
	}
	g_voisins = copie_nouveau_graphe(g, ignore_voisins);
	free(ignore_voisins);
	explorer_branchement_ameliore(g_voisins, ctx, taille_suivante);
	liberer_graphe(g_voisins);
	if (aretes)
		free(aretes);
}

int	*couverture_branchement_ameliore(Graphe *g, int *taille)
{
	BranchementContexte	ctx;
	int					capacite;
	int					*solution;

	if (!g)
	{
		if (taille)
			*taille = 0;
		return (NULL);
	}
	capacite = nombre_sommets(g);
	if (capacite <= 0)
	{
		if (taille)
			*taille = 0;
		return (NULL);
	}
	ctx.capacite = capacite;
	ctx.courant = malloc(sizeof(int) * capacite);
	ctx.meilleur = malloc(sizeof(int) * capacite);
	ctx.taille_meilleur = INT_MAX;
	if (!ctx.courant || !ctx.meilleur)
	{
		perror("Erreur d'allocation branchement amélioré");
		free(ctx.courant);
		free(ctx.meilleur);
		if (taille)
			*taille = 0;
		return (NULL);
	}
	explorer_branchement_ameliore(g, &ctx, 0);
	if (ctx.taille_meilleur == INT_MAX)
	{
		free(ctx.courant);
		free(ctx.meilleur);
		if (taille)
			*taille = 0;
		return (NULL);
	}
	solution = malloc(sizeof(int) * ctx.taille_meilleur);
	if (!solution)
	{
		perror("Erreur d'allocation solution branchement amélioré");
		free(ctx.courant);
		free(ctx.meilleur);
		exit(EXIT_FAILURE);
	}
	memcpy(solution, ctx.meilleur, sizeof(int) * ctx.taille_meilleur);
	if (taille)
		*taille = ctx.taille_meilleur;
	free(ctx.courant);
	free(ctx.meilleur);
	return (solution);
}
