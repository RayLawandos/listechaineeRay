#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Element
{
    int nombre;
    struct Element *suivant;
} Element;

typedef struct Liste
{
    Element *premier;
} Liste;

Liste *initialisation()
{
    Liste *liste = malloc(sizeof(*liste));
    Element *element = malloc(sizeof(*element));

    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    element->nombre = 0;
    element->suivant = NULL;
    liste->premier = element;

    return liste;
}

void insertion(Liste *liste, int nvNombre)
{
    /* Création du nouvel élément */
    Element *nouveau = malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    nouveau->nombre = nvNombre;

    /* Insertion de l'élément au début de la liste */
    nouveau->suivant = liste->premier;
    liste->premier = nouveau;
}

void suppressionDebut(Liste *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste->premier != NULL)
    {
        Element *aSupprimer = liste->premier;
        liste->premier = liste->premier->suivant;
        free(aSupprimer);
    }
}

void suppressionFin(Liste *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste->premier != NULL)
    {
        Element *actuel = liste->premier;
        Element *precedent = NULL;
        while (actuel->suivant != NULL)
        {
            precedent = actuel;
            actuel = actuel->suivant;
        }

        if (precedent != NULL)
        {
            precedent->suivant = NULL;
        }
        free(actuel);
    }
}

void afficherListe(Liste *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *actuel = liste->premier;

    while (actuel != NULL)
    {
        printf("%d -> ", actuel->nombre);
        actuel = actuel->suivant;
    }
    printf("NULL\n");
}

void libererListe(Liste *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *actuel = liste->premier;
    while (actuel != NULL)
    {
        Element *aSupprimer = actuel;
        actuel = actuel->suivant;
        free(aSupprimer);
    }

    free(liste);
}

void sauvegarderListe(Liste *liste, const char *nomFichier)
{
    if (liste == NULL || nomFichier == NULL)
    {
        exit(EXIT_FAILURE);
    }

    FILE *fichier = fopen(nomFichier, "w");
    if (fichier == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *actuel = liste->premier;
    while (actuel != NULL)
    {
        fprintf(fichier, "%d\n", actuel->nombre);
        actuel = actuel->suivant;
    }

    fclose(fichier);
}

void chargerListe(Liste *liste, const char *nomFichier)
{
    if (liste == NULL || nomFichier == NULL)
    {
        exit(EXIT_FAILURE);
    }

    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int nombre;
    while (fscanf(fichier, "%d", &nombre) != EOF)
    {
        insertion(liste, nombre);
    }

    fclose(fichier);
}

int rechercherElement(Liste *liste, int element)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *actuel = liste->premier;
    int position = 1;
    while (actuel != NULL)
    {
        if (actuel->nombre == element)
        {
            return position;
        }
        position++;
        actuel = actuel->suivant;
    }

    return -1;
}

int main(int argc, char *argv[])
{
    Liste *maListe = initialisation();

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-A") == 0 || strcmp(argv[i], "--append") == 0)
        {
            if (i + 1 < argc)
            {
                insertion(maListe, atoi(argv[i + 1]));
                i++;
            }
        }
        else if (strcmp(argv[i], "-P") == 0 || strcmp(argv[i], "--prepend") == 0)
        {
            if (i + 1 < argc)
            {
                insertion(maListe, atoi(argv[i + 1]));
                i++;
            }
        }
        else if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--delend") == 0)
        {
            suppressionDebut(maListe);
        }
        else if (strcmp(argv[i], "-X") == 0 || strcmp(argv[i], "--delete") == 0)
        {
            suppressionFin(maListe);
        }
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--display") == 0)
        {
            afficherListe(maListe);
        }
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--init") == 0)
        {
            libererListe(maListe);
            maListe = initialisation();
        }
        else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--load") == 0)
        {
            if (i + 1 < argc)
            {
                chargerListe(maListe, argv[i + 1]);
                i++;
            }
        }
        else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--save") == 0)
        {
            if (i + 1 < argc)
            {
                sauvegarderListe(maListe, argv[i + 1]);
                i++;
            }
        }
        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--test") == 0)
        {
            if (i + 1 < argc)
            {
                int element = atoi(argv[i + 1]);
                int position = rechercherElement(maListe, element);
                if (position != -1)
                {
                    printf("L'élément %d se trouve à la position %d.\n", element, position);
                }
                else
                {
                    printf("L'élément %d n'est pas dans la liste.\n", element);
                }
                i++;
            }
        }
        else if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quit") == 0)
        {
            libererListe(maListe);
            return 0;
        }
    }

    libererListe(maListe);
    return 0;
}