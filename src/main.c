/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   main.c                                        :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"

// NB : le main est assez chargé, parfois même plutot redondant mais c'est plus pratique pour les tests 


static void	afficher_graphe(const Graphe *graphe)
{
	int	id;

	printf("\n=== Structure du graphe ===\n");
	for (int i = 0; i < nombre_sommets(graphe); ++i)
	{
		id = id_sommet(graphe, i);
		printf("Sommet %d:", id);
		for (int j = 0; j < graphe->sommets[i].nombre_voisins; ++j)
		{
			printf(" %d", graphe->sommets[i].voisins[j]);
		}
		printf("\n");
	}
	printf("===========================\n\n");
}

static void	afficher_couverture(const char *etiquette, const int *couverture, int taille)
{
	printf("%s (taille %d):", etiquette, taille);
	for (int i = 0; i < taille; ++i)
	{
		printf(" %d", couverture[i]);
	}
	printf("\n");
}



static Graphe	*charger_graphe_utilisateur(void)
{
	char	chemin[256];
	Graphe	*graphe;

	printf("\nEntrez le chemin du fichier: ");
	if (scanf("%255s", chemin) != 1)
	{
		fprintf(stderr, "Erreur de lecture\n");
		return (NULL);
	}
	graphe = charger_fichier(chemin);
	if (!graphe)
	{
		fprintf(stderr, "Impossible de charger le fichier %s\n", chemin);
		return (NULL);
	}
	printf(" Graphe chargé avec succès (%d sommets)\n", nombre_sommets(graphe));
	return (graphe);
}

static Graphe	*generer_graphe_aleatoire(void)
{
	int		n;
	double	p;
	Graphe	*graphe;

	printf("\nNombre de sommets: ");
	if (scanf("%d", &n) != 1 || n <= 0)
	{
		fprintf(stderr, "Nombre de sommets invalide\n");
		return (NULL);
	}
	printf("Probabilité p (entre 0 et 1): ");
	if (scanf("%lf", &p) != 1 || p <= 0 || p >= 1)
	{
		fprintf(stderr, "Probabilité invalide\n");
		return (NULL);
	}
	graphe = graphe_aleatoire(n, p, (unsigned int)time(NULL));
	if (!graphe)
	{
		fprintf(stderr, "Erreur lors de la génération\n");
		return (NULL);
	}
	printf(" Graphe aléatoire généré (%d sommets, p=%.2f)\n", n, p);
	return (graphe);
}

static Graphe	*construire_graphe_exemple(void)
{
	const int	ids[] = {1, 2, 3, 4, 5, 6};
	Graphe		*graphe;

	graphe = creer_graphe(6, ids);
	if (!graphe)
		return (NULL);
	ajouter_arete(graphe, 3, 6);
	ajouter_arete(graphe, 1, 3);
	ajouter_arete(graphe, 1, 4);
	ajouter_arete(graphe, 2, 3);
	ajouter_arete(graphe, 4, 5);
	printf(" Graphe d'exemple créé (6 sommets)\n");
	return (graphe);
}



// Version unifiée pour exécuter les algorithmes de branchement
static void executer_branchement(Graphe *graphe, int version_algo)
{
	const char *noms_algo[] = {"", "Branchement (sans bornes)",
	                            "Branchement (avec bornes)",
	                            "Branchement (amélioré 4.3)"};
	int taille;
	int *couverture;
	clock_t start, end;
	double temps;

	if (!graphe) {
		printf("Aucun graphe chargé.\n");
		return;
	}

	printf("\n--- Algorithme de %s ---\n", noms_algo[version_algo]);
	start = clock();
	couverture = calculer_couverture_branchement_generique(graphe, &taille, version_algo);
	end = clock();
	temps = ((double)(end - start)) / CLOCKS_PER_SEC;

	if (couverture) {
		afficher_couverture("Couverture minimale", couverture, taille);
		printf("Temps d'exécution: %.6f secondes\n", temps);
		free(couverture);
	} else {
		printf("Aucune couverture trouvée.\n");
	}
}

static void	executer_couplage(Graphe *graphe)
{
	int		taille;
	int		*couverture;
	clock_t	start, end;
	double	temps;

	if (!graphe)
	{
		printf("Aucun graphe chargé.\n");
		return ;
	}
	printf("\n--- Algorithme de Couplage Maximal ---\n");
	start = clock();
	couverture = couverture_sommets_couplage(graphe, &taille);
	end = clock();
	temps = ((double)(end - start)) / CLOCKS_PER_SEC;
	
	if (couverture)
	{
		afficher_couverture("Couverture par couplage", couverture, taille);
		printf("Temps d'exécution: %.6f secondes\n", temps);
		free(couverture);
	}
}

static void	executer_glouton(Graphe *graphe)
{
	int		taille;
	int		*couverture;
	clock_t	start, end;
	double	temps;

	if (!graphe)
	{
		printf("Aucun graphe chargé.\n");
		return ;
	}
	printf("\n--- Algorithme Glouton ---\n");
	start = clock();
	couverture = couverture_sommets_glouton(graphe, &taille);
	end = clock();
	temps = ((double)(end - start)) / CLOCKS_PER_SEC;
	
	if (couverture)
	{
		afficher_couverture("Couverture gloutonne", couverture, taille);
		printf("Temps d'exécution: %.6f secondes\n", temps);
		free(couverture);
	}
}


static void	comparer_tous_algorithmes(Graphe *graphe)
{
	if (!graphe) { printf("Aucun graphe chargé.\n"); return; }

	printf("\n========================================\n");
	printf("COMPARAISON DES ALGORITHMES\n");
	printf("========================================\n");
	
	executer_couplage(graphe);
	printf("\n");
	executer_glouton(graphe);
	printf("\n");
	executer_branchement(graphe, 2);
	printf("\n");
	executer_branchement(graphe, 3);
	
	printf("========================================\n");
}

static void	demo_operations_base(Graphe *graphe)
{
	Graphe		*graphe_modifie;
	int			choix;
	int			sommet_a_supprimer;
	int			nb_sommets_a_supprimer;
	int			*sommets_a_supprimer;
	int			*degres;
	int			valeur_degre_max;
	int			sommet_max;
	int			continuer = 1;

	if (!graphe)
	{
		printf("\n Aucun graphe chargé. Veuillez d'abord charger ou créer un graphe.\n");
		return ;
	}

	printf("\n========================================\n");
	printf("DÉMONSTRATION DES OPÉRATIONS DE BASE\n");
	printf("========================================\n");

	while (continuer)
	{
		printf("\n--- Graphe actuel ---\n");
		afficher_graphe(graphe);

		printf("\nOpérations disponibles:\n");
		printf("1. Supprimer un sommet spécifique\n");
		printf("2. Supprimer plusieurs sommets spécifiques\n");
		printf("3. Supprimer un sommet aléatoire\n");
		printf("4. Supprimer plusieurs sommets aléatoires\n");
		printf("5. Afficher les degrés des sommets\n");
		printf("6. Trouver le sommet de degré maximum\n");
		printf("0. Retour au menu principal\n");
		printf("\nVotre choix: ");

		if (scanf("%d", &choix) != 1)
		{
			while (getchar() != '\n');
			printf("Entrée invalide.\n");
			continue;
		}

		switch (choix)
		{
			case 1:
				printf("\nEntrez l'ID du sommet à supprimer: ");
				if (scanf("%d", &sommet_a_supprimer) != 1)
				{
					printf("Entrée invalide.\n");
					while (getchar() != '\n');
					break;
				}
				graphe_modifie = supprimer_sommet(graphe, sommet_a_supprimer);
				if (graphe_modifie)
				{
					printf("\n Sommet %d supprimé avec succès.\n", sommet_a_supprimer);
					printf("\n--- Graphe après suppression ---\n");
					afficher_graphe(graphe_modifie);
					
					printf("\nVoulez-vous conserver ce graphe modifié ? (1=Oui, 0=Non): ");
					int conserver;
					if (scanf("%d", &conserver) == 1 && conserver == 1)
					{
						liberer_graphe(graphe);
						graphe = graphe_modifie;
						printf(" Graphe mis à jour.\n");
					}
					else
					{
						liberer_graphe(graphe_modifie);
						printf("Graphe original conservé.\n");
					}
				}
				else
				{
					printf(" Erreur: sommet %d introuvable.\n", sommet_a_supprimer);
				}
				break;

			case 2:
				printf("\nCombien de sommets voulez-vous supprimer ? ");
				if (scanf("%d", &nb_sommets_a_supprimer) != 1 || nb_sommets_a_supprimer <= 0)
				{
					printf("Nombre invalide.\n");
					while (getchar() != '\n');
					break;
				}
				sommets_a_supprimer = malloc(sizeof(int) * nb_sommets_a_supprimer);
				if (!sommets_a_supprimer)
				{
					printf("Erreur d'allocation mémoire.\n");
					break;
				}
				printf("Entrez les IDs des sommets à supprimer:\n");
				for (int i = 0; i < nb_sommets_a_supprimer; i++)
				{
					printf("  Sommet %d: ", i + 1);
					if (scanf("%d", &sommets_a_supprimer[i]) != 1)
					{
						printf("Entrée invalide.\n");
						free(sommets_a_supprimer);
						while (getchar() != '\n');
						goto case_2_end;
					}
				}
				graphe_modifie = supprimer_liste_sommets(graphe, sommets_a_supprimer, nb_sommets_a_supprimer);
				if (graphe_modifie)
				{
					printf("\n Sommets supprimés avec succès.\n");
					printf("\n--- Graphe après suppression ---\n");
					afficher_graphe(graphe_modifie);
					
					printf("\nVoulez-vous conserver ce graphe modifié ? (1=Oui, 0=Non): ");
					int conserver;
					if (scanf("%d", &conserver) == 1 && conserver == 1)
					{
						liberer_graphe(graphe);
						graphe = graphe_modifie;
						printf(" Graphe mis à jour.\n");
					}
					else
					{
						liberer_graphe(graphe_modifie);
						printf("Graphe original conservé.\n");
					}
				}
				else
				{
					printf(" Erreur lors de la suppression.\n");
				}
				free(sommets_a_supprimer);
				case_2_end:
				break;

			case 3:
				if (nombre_sommets(graphe) == 0)
				{
					printf(" Le graphe est vide.\n");
					break;
				}
				sommet_a_supprimer = id_sommet(graphe, rand() % nombre_sommets(graphe));
				printf("\nSommet aléatoire sélectionné: %d\n", sommet_a_supprimer);
				graphe_modifie = supprimer_sommet(graphe, sommet_a_supprimer);
				if (graphe_modifie)
				{
					printf("\n Sommet %d supprimé avec succès.\n", sommet_a_supprimer);
					printf("\n--- Graphe après suppression ---\n");
					afficher_graphe(graphe_modifie);
					
					printf("\nVoulez-vous conserver ce graphe modifié ? (1=Oui, 0=Non): ");
					int conserver;
					if (scanf("%d", &conserver) == 1 && conserver == 1)
					{
						liberer_graphe(graphe);
						graphe = graphe_modifie;
						printf(" Graphe mis à jour.\n");
					}
					else
					{
						liberer_graphe(graphe_modifie);
						printf("Graphe original conservé.\n");
					}
				}
				break;

			case 4:
				printf("\nCombien de sommets voulez-vous supprimer aléatoirement ? ");
				if (scanf("%d", &nb_sommets_a_supprimer) != 1 || nb_sommets_a_supprimer <= 0)
				{
					printf("Nombre invalide.\n");
					while (getchar() != '\n');
					break;
				}
				if (nb_sommets_a_supprimer > nombre_sommets(graphe))
				{
					printf(" Il n'y a que %d sommets dans le graphe.\n", nombre_sommets(graphe));
					break;
				}
				sommets_a_supprimer = malloc(sizeof(int) * nb_sommets_a_supprimer);
				if (!sommets_a_supprimer)
				{
					printf("Erreur d'allocation mémoire.\n");
					break;
				}
				// sélection aléatoire sans répétition
				int *indices_utilises = calloc(nombre_sommets(graphe), sizeof(int));
				for (int i = 0; i < nb_sommets_a_supprimer; i++)
				{
					int idx;
					do {
						idx = rand() % nombre_sommets(graphe);
					} while (indices_utilises[idx]);
					indices_utilises[idx] = 1;
					sommets_a_supprimer[i] = id_sommet(graphe, idx);
				}
				free(indices_utilises);
				
				printf("Sommets aléatoires sélectionnés:");
				for (int i = 0; i < nb_sommets_a_supprimer; i++)
					printf(" %d", sommets_a_supprimer[i]);
				printf("\n");
				
				graphe_modifie = supprimer_liste_sommets(graphe, sommets_a_supprimer, nb_sommets_a_supprimer);
				if (graphe_modifie)
				{
					printf("\n Sommets supprimés avec succès.\n");
					printf("\n--- Graphe après suppression ---\n");
					afficher_graphe(graphe_modifie);
					
					printf("\nVoulez-vous conserver ce graphe modifié ? (1=Oui, 0=Non): ");
					int conserver;
					if (scanf("%d", &conserver) == 1 && conserver == 1)
					{
						liberer_graphe(graphe);
						graphe = graphe_modifie;
						printf(" Graphe mis à jour.\n");
					}
					else
					{
						liberer_graphe(graphe_modifie);
						printf("Graphe original conservé.\n");
					}
				}
				free(sommets_a_supprimer);
				break;

			case 5:
				degres = malloc(sizeof(int) * nombre_sommets(graphe));
				if (!degres)
				{
					printf("Erreur d'allocation mémoire.\n");
					break;
				}
				calculer_degres(graphe, degres);
				printf("\n--- Degrés des sommets ---\n");
				for (int i = 0; i < nombre_sommets(graphe); ++i)
				{
					printf("Sommet %d → degré %d\n", id_sommet(graphe, i), degres[i]);
				}
				free(degres);
				break;

			case 6:
				valeur_degre_max = 0;
				sommet_max = sommet_degre_max(graphe, &valeur_degre_max);
				printf("\n--- Sommet de degré maximum ---\n");
				printf("Sommet %d avec un degré de %d\n", sommet_max, valeur_degre_max);
				break;

			case 0:
				continuer = 0;
				printf("\nRetour au menu principal.\n");
				break;

			default:
				printf("Choix invalide.\n");
		}
	}

	printf("========================================\n");
}

// ============================================================================
// MENUS
// ============================================================================
static void	afficher_menu_principal(void)
{
	printf("\n");
	printf("╔════════════════════════════════════════════════╗\n");
	printf("║                 MENU PRINCIPAL                 ║\n");
	printf("╠════════════════════════════════════════════════╣\n");
	printf("║  1. Charger un graphe depuis un fichier        ║\n");
	printf("║  2. Générer un graphe aléatoire                ║\n");
	printf("║  3. Utiliser le graphe d'exemple               ║\n");
	printf("║  4. Afficher le graphe actuel                  ║\n");
	printf("║  ─────────────────────────────────────────     ║\n");
	printf("║  5. Branchement naïf (sans bornes)             ║\n");
	printf("║  6. Branchement avec bornes                    ║\n");
	printf("║  7. Branchement amélioré (4.3)                 ║\n");
	printf("║  8. Algorithme de Couplage                     ║\n");
	printf("║  9. Algorithme Glouton                         ║\n");
	printf("║ 10. Comparer tous les algorithmes              ║\n");
	printf("║  ─────────────────────────────────────────     ║\n");
	printf("║ 11. Démonstration des opérations de base       ║\n");
	printf("║  ─────────────────────────────────────────     ║\n");
	printf("║  0. Quitter                                    ║\n");
	printf("╚════════════════════════════════════════════════╝\n");
	printf("\nVotre choix: ");
}


int	main(void)
{
	Graphe	*graphe_courant = NULL;
	int		choix;
	int		continuer = 1;

	srand((unsigned int)time(NULL));

	printf("\n");
	printf("═══════════════════════════════════════════════════════\n");
	printf("  Projet COMPLEX - Vertex Cover \n");
	printf("  Auteurs: BARAHIMI Imane                              \n");
	printf("═══════════════════════════════════════════════════════\n");

	while (continuer)
	{
		afficher_menu_principal();
		
		if (scanf("%d", &choix) != 1)
		{
			while (getchar() != '\n');
			printf("Entrée invalide.\n");
			continue;
		}

		switch (choix)
		{
			case 1:
				if (graphe_courant)
					liberer_graphe(graphe_courant);
				graphe_courant = charger_graphe_utilisateur();
				break;

			case 2:
				if (graphe_courant)
					liberer_graphe(graphe_courant);
				graphe_courant = generer_graphe_aleatoire();
				break;

			case 3:
				if (graphe_courant)
					liberer_graphe(graphe_courant);
				graphe_courant = construire_graphe_exemple();
				break;

			case 4:
				if (graphe_courant)
					afficher_graphe(graphe_courant);
				else
					printf("Aucun graphe chargé.\n");
				break;

			case 5:
				executer_branchement(graphe_courant, 1);
				break;

			case 6:
				executer_branchement(graphe_courant, 2);
				break;

			case 7:
				executer_branchement(graphe_courant, 3);
				break;

			case 8:
				executer_couplage(graphe_courant);
				break;

			case 9:
				executer_glouton(graphe_courant);
				break;

			case 10:
				comparer_tous_algorithmes(graphe_courant);
				break;

			case 11:
				demo_operations_base(graphe_courant);
				break;

			case 0:
				continuer = 0;
				break;

			default:
				printf("Choix invalide. Veuillez réessayer.\n");
		}
	}

	if (graphe_courant)
		liberer_graphe(graphe_courant);

	return (0);
}