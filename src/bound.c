/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   bound.c                                       :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +#+     */
/*                                                +#+  +:+      +#+    +#+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"
#include <math.h>

// noeud "coupé" (pruned) pour l'affichage (main.c teste id == -2)
#define PRUNE_NODE_ID (-2)

/*
  b1 = val absolue nb arrete m / delta le degré max
 */
static int calculer_b1(const Graphe *graphe, int m)
{
	int valeur_degre;
	
	sommet_degre_max(graphe, &valeur_degre);
	if (valeur_degre == 0)
		return 0;

	return (m + valeur_degre - 1) / valeur_degre;
}

/*
 calcule b2 = |M| le couplage max
 */
static int calculer_b2(const Graphe *graphe)
{
	int taille_couverture;
	int *couverture;
	int b2;
	
	couverture = couverture_sommets_couplage(graphe, &taille_couverture);
	if (!couverture)
		return 0;

	b2 = taille_couverture / 2;

	free(couverture);
	return b2;
}

/*
 calcule b3 = (2n - 1 - sqrt((2n-1)^2 - 8m)) / 2
 */
static int calculer_b3(int n, int m)
{
	double terme1;
	double discriminant;
	double racine;
	double resultat;
	
	terme1 = 2.0 * n - 1.0;
	discriminant = terme1 * terme1 - 8.0 * m;
	
	if (discriminant < 0)
		return 0;
	
	racine = sqrt(discriminant);
	resultat = (terme1 - racine) / 2.0;

	return (int)ceil(resultat);
}

/*
 calcul de la borne inférieure pour la couverture minimale
 */
int calculer_borne_inferieure(const Graphe *graphe)
{
	int n;         // nombre de sommets
	int m;         // nombre d'arêtes
	int b1, b2, b3;
	int borne;
	AreteGraphe *aretes;
	
	if (!graphe || graphe->nombre_sommets == 0)
		return 0;
	
	n = graphe->nombre_sommets;
	aretes = obtenir_aretes(graphe, &m);
	
	if (!aretes || m == 0)
	{
		free(aretes);
		return 0;
	}

	b1 = calculer_b1(graphe, m);
	b2 = calculer_b2(graphe);
	b3 = calculer_b3(n, m);
	
	free(aretes);
	
	borne = b1;
	if (b2 > borne)
		borne = b2;
	if (b3 > borne)
		borne = b3;
	
	return borne;

}

static int	compter_aretes(const Graphe *g)
{
	int			nb;
	AreteGraphe	*as;

	nb = 0;
	as = obtenir_aretes(g, &nb); // nb = nombre d'arêtes
	if (as)
		free(as);
	return (nb); // retourne m
}

static int	ceil_div(int a, int b)
{
	return ((b <= 0) ? 0 : (a + b - 1) / b);
}

// LB = max{ ceil(m/Δ), |M|, (2n-1 - sqrt((2n-1)^2 - 8m))/2 }
static int	borne_inferieure(const Graphe *g, int matching_size)
{
	const int n = nombre_sommets(g); // n = nombre de sommets
	const int m = compter_aretes(g); // m = nombre d'arêtes de chaque sous graphe
	int degmax = 0; // Δ = degré maximum
	int				b1;
	const int		b2 = matching_size;
	const double	t = (double)(2 * n - 1);
	const double	disc = t * t - 8.0 * (double)m;
	const double	root = (disc > 0.0) ? sqrt(disc) : 0.0;
	int				b3;
	int				lb;
	const double	b3_real = (t - root) / 2.0;

	
	
	(void)sommet_degre_max(g, &degmax);
	// b1 = ⌈m/Δ⌉
	b1 = (degmax > 0) ? ceil_div(m, degmax) : 0;
	// on utilise floor pour éviter une surestimation due aux erreurs flottantes
	b3 = (int)floor(b3_real + 1e-12);
	// LB = max(b1, b2, b3)
	lb = b1;
	if (b2 > lb)
		lb = b2;
	if (b3 > lb)
		lb = b3;
	return (lb);
}

static void	branchementRecursionBB(Arbre *arbre, Graphe *g, int *prefix,
		int prefix_len, int *best_size, int **best_solution)
{
	int			*sol;
	int			ub_local;
	int			matching_size;
	int			*cover;
	int			nbAretes;
	AreteGraphe	*as;
	int			idx_u;
	int			idx_v;
	int			id_u;
	int			id_v;
	Arbre		u;
	Arbre		v;
	int			pos_u;
	int			pos_v;
	const int	n = nombre_sommets(g);
	int			*ignore;
	Graphe		*g_u;
	Graphe		*g_v;
	int			lb;
	int			cand;

	// si plus d'arêtes -> solution complète
	if (compter_aretes(g) == 0)
	{
		if (prefix_len < *best_size)
		{
			sol = (int *)malloc(sizeof(int) * prefix_len);
			if (!sol)
			{
				arbre->n_nodes = 0;
				return ;
			}
			memcpy(sol, prefix, sizeof(int) * prefix_len);
			free(*best_solution);
			*best_solution = sol;
			*best_size = prefix_len;
		}
		arbre->n_nodes = 0;
		return ;
	}
	// 1) UB locale par couplage
	ub_local = 0;
	matching_size = 0;
	cover = couverture_sommets_couplage(g, &ub_local);
	if (cover && ub_local > 0)
	{
		cand = prefix_len + ub_local;
		if (cand < *best_size)
		{
			sol = (int *)malloc(sizeof(int) * cand);
			if (sol)
			{
				memcpy(sol, prefix, sizeof(int) * prefix_len);
				memcpy(sol + prefix_len, cover, sizeof(int) * ub_local);
				free(*best_solution);
				*best_solution = sol;
				*best_size = cand;
			}
		}
		matching_size = ub_local / 2;
		free(cover);
	}
	else
	{
		if (cover)
			free(cover);
	}
	lb = borne_inferieure(g, matching_size);
	if (prefix_len + lb >= *best_size)
	{
		arbre->id = PRUNE_NODE_ID;
		arbre->n_nodes = 0;
		return ;
	}
	// 2) Brancher sur une arête {u,v}
	nbAretes = 0;
	as = obtenir_aretes(g, &nbAretes);
	if (!as || nbAretes == 0)
	{
		if (as)
			free(as);
		arbre->n_nodes = 0;
		return ;
	}
	idx_u = as[0].indice_u;
	idx_v = as[0].indice_v;
	id_u = id_sommet(g, idx_u);
	id_v = id_sommet(g, idx_v);
	free(as);
	u = new_arbre(id_u);
	v = new_arbre(id_v);
	ajouter_noeud(arbre, u);
	ajouter_noeud(arbre, v);
	pos_u = arbre->n_nodes - 2;
	pos_v = arbre->n_nodes - 1;
	// créer sous-graphes G - {u} et G - {v}
	ignore = (int *)calloc((size_t)n, sizeof(int));
	if (!ignore)
		return ;
	ignore[idx_u] = 1;
	g_u = copie_nouveau_graphe(g, ignore);
	ignore[idx_u] = 0;
	ignore[idx_v] = 1;
	g_v = copie_nouveau_graphe(g, ignore);
	free(ignore);
	// descendre (push id_u / id_v)
	if (g_u)
	{
		prefix[prefix_len] = id_u;
		branchementRecursionBB(&arbre->nodes[pos_u], g_u, prefix, prefix_len
			+ 1, best_size, best_solution);
		liberer_graphe(g_u);
	}
	if (g_v)
	{
		prefix[prefix_len] = id_v;
		branchementRecursionBB(&arbre->nodes[pos_v], g_v, prefix, prefix_len
			+ 1, best_size, best_solution);
		liberer_graphe(g_v);
	}
}

// pour le menu du main
Arbre	branchement_avec_bornes(Graphe g, int *best_size, int **best_solution)
{
	Arbre		arbre;
	const int	n = nombre_sommets(&g);
	int			*prefix;

	arbre = new_arbre(-1);
	prefix = (int *)malloc(sizeof(int) * (n > 0 ? n : 1));
	if (!prefix)
		return (arbre);
	branchementRecursionBB(&arbre, &g, prefix, 0, best_size, best_solution);
	free(prefix);
	return (arbre);
}
