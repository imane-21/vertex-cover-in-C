/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   branch_common.c                               :+:    :+:      :+:    :+:   */
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

	* Fonction générique pour calculer une couverture avec une méthode de branchement
 * version_algo:
 *   1 = branchement naïf (sans bornes)
 *   2 = branchement avec bornes
 *   3 = branchement amélioré (4.3)
 */
int	*calculer_couverture_branchement_generique(Graphe *graphe, int *taille,
		int version_algo)
{
	Arbre	arbre;
	int		sizeSolution;
	int		nSommet;
	int		*solution;
	int		*couvertureTmp;
	int		*best_solution;
	int		best_size;
	int		*solutionFinale;
		int *resultat;

	sizeSolution = INT_MAX;
	nSommet = nombre_sommets(graphe);
	solution = malloc(sizeof(int) * nSommet);
	couvertureTmp = malloc(sizeof(int) * nSommet);
	best_solution = NULL;
	if (!solution || !couvertureTmp)
	{
		perror("Erreur d'allocation mémoire");
		free(solution);
		free(couvertureTmp);
		*taille = 0;
		return (NULL);
	}
	// Choisir la version d'algorithme
	switch (version_algo)
	{
	case 1: // Branchement naïf
		arbre = branchement1(*graphe);
		rechercheMin(&arbre, &sizeSolution, 0, couvertureTmp, solution);
		break ;
	case 2: // Branchement avec bornes
		best_solution = couverture_sommets_couplage(graphe, &best_size);
		if (!best_solution)
		{
			best_size = INT_MAX;
		}
		arbre = branchement_avec_bornes(*graphe, &best_size, &best_solution);
		// Pour version 2, la solution est déjà dans best_solution
		if (best_size != INT_MAX && best_solution)
		{
			free(solution);
			free(couvertureTmp);
			*taille = best_size;
			return (best_solution);
		}
		free(best_solution);
		free(solution);
		free(couvertureTmp);
		*taille = 0;
		return (NULL);
	case 3:
	{
		resultat = couverture_branchement_ameliore(graphe, &sizeSolution);
		free(solution);
		free(couvertureTmp);
		*taille = sizeSolution;
		return (resultat);
	}
	default:
		free(solution);
		free(couvertureTmp);
		*taille = 0;
		return (NULL);
	}
	// Pour versions 1 et 3 : extraire la solution de l'arbre
	if (sizeSolution == 0 || sizeSolution == INT_MAX)
	{
		*taille = 0;
		free(solution);
		free(couvertureTmp);
		return (NULL);
	}
	solutionFinale = realloc(solution, sizeSolution * sizeof(int));
	if (!solutionFinale)
	{
		perror("Erreur d'allocation solution");
		free(couvertureTmp);
		exit(EXIT_FAILURE);
	}
	free(couvertureTmp);
	*taille = sizeSolution;
	return (solutionFinale);
}
