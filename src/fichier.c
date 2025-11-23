/* **************************************************************************** */
/*                                                                             	*/
/*                                                   ::::::::       :::    :::  */
/*   fichier.c                                     :+:    :+:      :+:    :+:   */
/*                                                +:+             +:+    +:+    */
/*   Auteurs: BARAHIMI Imane                      +#+  +:+       +#+    +:+     */
/*                                                +#+  +:+      +#+    +:+      */
/*                                                    +#+      +#+    +#+       */
/*             								  #+#    #+#      #+#    #+#        */
/*             								  ########        ########          */
/*                                                                              */
/* **************************************************************************** */

#include "graph.h"

/* fonctions de parsing pour la lecture de graphes d'exemple */

static int	lire_entier_simple(FILE *fichier, int *valeur)
{
	char	ligne[BUFFER_LIGNE];
	char	*fin;
	long	temp;

	if (!parser_fichier(fichier, ligne))
	{
		return (0);
	}
	temp = strtol(ligne, &fin, 10);
	if (*fin != '\0')
	{
		return (0);
	}
	*valeur = (int)temp;
	return (1);
}

static int	lire_arete(FILE *fichier, int *id_u, int *id_v)
{
	char	ligne[BUFFER_LIGNE];

	if (!parser_fichier(fichier, ligne))
	{
		return (0);
	}
	return (sscanf(ligne, "%d %d", id_u, id_v) == 2);
}

/*fonction pour parser le fichier d'input et créer le graphe correspondant */

int	parser_fichier(FILE *f, char *ligne)
{
	size_t	len;
	size_t	debut;

	while (fgets(ligne, BUFFER_LIGNE, f))
	{
		len = strcspn(ligne, "\r\n");
		ligne[len] = '\0';
		debut = 0;
		while (ligne[debut] == ' ' || ligne[debut] == '\t')
		{
			++debut;
		}
		if (ligne[debut] != '\0')
		{
			if (debut > 0)
			{
				memmove(ligne, ligne + debut, strlen(ligne + debut) + 1);
			}
			return (1);
		}
	}
	return (0);
}

Graphe	*charger_fichier(const char *chemin)
{
	FILE *fichier;
	char etiquette[BUFFER_LIGNE];
	int nombre_sommets_fichier;
	int nombre_aretes_fichier;
	int *ids;
	Graphe *graphe;
	int id_u;
	int id_v;
	int lecture_reussie;
	int succes;

	if (!chemin)
	{
		return (NULL);
	}
	fichier = fopen(chemin, "r");
	if (!fichier)
	{
		return (NULL);
	}
	ids = NULL;
	graphe = NULL;
	succes = 0;
	do
	{
		if (!parser_fichier(fichier, etiquette))
		{
			break ;
		}
		if (!lire_entier_simple(fichier, &nombre_sommets_fichier)
			|| nombre_sommets_fichier <= 0)
		{
			break ;
		}
		if (!parser_fichier(fichier, etiquette))
		{
			break ;
		}
		ids = malloc(sizeof(int) * (size_t)nombre_sommets_fichier);
		if (!ids)
		{
			break ;
		}
		lecture_reussie = 1;
		for (int i = 0; i < nombre_sommets_fichier; ++i)
		{
			if (!lire_entier_simple(fichier, &ids[i]))
			{
				lecture_reussie = 0;
				break ;
			}
		}
		if (!lecture_reussie)
		{
			break ;
		}
		if (!parser_fichier(fichier, etiquette)
			|| !lire_entier_simple(fichier, &nombre_aretes_fichier)
			|| nombre_aretes_fichier < 0)
		{
			break ;
		}
		if (!parser_fichier(fichier, etiquette))
		{
			break ;
		}
		graphe = creer_graphe(nombre_sommets_fichier, ids);
		if (!graphe)
		{
			break ;
		}
		lecture_reussie = 1;
		for (int i = 0; i < nombre_aretes_fichier; ++i)
		{
			if (!lire_arete(fichier, &id_u, &id_v) || ajouter_arete(graphe,
					id_u, id_v) != 0)
			{
				lecture_reussie = 0;
				break ;
			}
		}
		if (!lecture_reussie)
		{
			break ;
		}
		succes = 1;
	} while (0);
	free(ids);
	fclose(fichier);
	if (!succes)
	{
		if (graphe)
		{
			liberer_graphe(graphe);
		}
		return (NULL);
	}
	return (graphe);
}