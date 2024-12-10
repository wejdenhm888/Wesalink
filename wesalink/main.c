#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <unistd.h>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define CLEAR_SCREEN "\033[2J"
#define CLEAR_TO_END_SCREEN "\033[0J"
#define CLEAR_TO_BEGIN_SCREEN "\033[1J"
#define CLEAR_SCROLLBACK "\033[3J"
#define CURSOR_HOME "\033[H"
#define CURSOR_MOVE(row, col) "\033[" #row ";" #col "H"
#define CURSOR_UP(n) "\033[" #n "A"
#define CURSOR_DOWN(n) "\033[" #n "B"
#define CURSOR_RIGHT(n) "\033[" #n "C"
#define CURSOR_LEFT(n) "\033[" #n "D"
#define CLEAR_LINE "\033[2K"
#define CLEAR_LINE_TO_END "\033[0K"
#define CLEAR_LINE_TO_BEGIN "\033[1K"
#define SAVE_CURSOR "\033[s"
#define RESTORE_CURSOR "\033[u"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"
#define SCROLL_UP(n) "\033[" #n "S"
#define SCROLL_DOWN(n) "\033[" #n "T"
struct utilisateur;

typedef struct Publication {
    int id;
    char contenu[256];
    struct Publication *suivant;
} Publication;

Publication *tete;
Publication *queue;

typedef struct demande {
    struct utilisateur *sender;
    struct demande *suivt;
} demande;


typedef struct utilisateur {
    char nom[20];
    char prenom[20];
    int age;
    char identifiant[20];
    char mdp[20];
    char indication[5];
    Publication *publications;
    demande *demande;
    struct utilisateur *amis[1000];
    int nb_amis;
    struct utilisateur *next;

}utilisateurs;

void enfiler_publication(utilisateurs *utilisateur) {
    Publication *temp = (Publication *)malloc(sizeof(Publication));
    if (!temp) {
        printf(RED "Erreur: Mémoire insuffisante pour ajouter une publication.\n" RESET);
        return;
    }
    fflush(stdin);
    char cont[256];
    printf("Donner le contenu de la publication :\n");
    fgets(cont, 256, stdin);
    cont[strcspn(cont, "\n")] = '\0';
    strcpy(temp->contenu, cont);

    static int id_pub = 0;
    id_pub++;
    temp->id = id_pub;


    strcpy(temp->contenu, cont);

    temp->suivant = NULL;

    if (tete == NULL && queue == NULL) {
        tete = temp;
        queue = temp;
    } else {
        queue->suivant = temp;
        queue = temp;
    }


    temp->suivant = utilisateur->publications;
    utilisateur->publications = temp;

    printf("Publication ajoutee: ID = %d, Contenu = \"%s\"\n", temp->id, temp->contenu);
}


void modifier_publication(utilisateurs *utilisateur, int id_pub, const char *nouveau_contenu) {
    if (utilisateur == NULL || utilisateur->publications == NULL) {
        printf("Erreur: L'utilisateur ou la liste des publications est vide.\n");
        return;
    }

    Publication *temp = utilisateur->publications;
    while (temp != NULL) {
        if (temp->id == id_pub) {
            strncpy(temp->contenu, nouveau_contenu, sizeof(temp->contenu) - 1);
            temp->contenu[sizeof(temp->contenu) - 1] = '\0';
            printf(MAGENTA"Publication modifiee avec succes. ID = %d, Nouveau contenu = \"%s\"\n"RESET, temp->id, temp->contenu);
            return;
        }
        temp = temp->suivant;
    }

    printf("Erreur: Aucune publication trouvée avec l'ID %d.\n", id_pub);
}




void publications_anciennes() {
    if (tete ==NULL) {
        printf("Aucune publication ancienne disponible\n");
        return;
    }
    Publication *aux;

    if (tete ) {
        printf("LA Publication ancienne : ID = %d, Contenu = \"%s\"\n",
               tete->id, tete->contenu);
        free(aux);
    }
}


void publications_recentes() {
    if (queue == NULL) {
        printf("Aucune publication recente disponible\n");
        return;
    }
    printf("La publication la plus recente est : ID = %d, Contenu = \"%s\"\n",
           queue->id, queue->contenu);
}


void supprimer_publications(utilisateurs *utilisateur, int id_publ) {
    if (!utilisateur || !utilisateur->publications) {
        printf("Erreur: Pas de publication A  supprimer.\n" );
        return;
    }
    Publication *publication = utilisateur->publications;
    Publication *pub_precd = NULL;
    while (publication && publication->id != id_publ) {
        pub_precd = publication;
        publication = publication->suivant;
    }
    if (!publication) {
        printf("La publication %d n a pas ete trouvee.\n" , id_publ);
        return;
    }
    if (!pub_precd) {
        utilisateur->publications = publication->suivant;
    } else {
        pub_precd->suivant = publication->suivant;
    }
    free(publication);
    printf( "La publication %d a ete supprimee.\n" , id_publ);
}

// hne bdina fel gestion des utilisateurs


utilisateurs* creer_utilisateurs() {
    utilisateurs* new_user = (utilisateurs*) malloc(sizeof(utilisateurs));
    if (!new_user) {
        printf("Erreur: Memoire insuffisante pour creer un utilisateur.\n");
        return NULL;
    }

    printf("Veuillez saisir votre nom:\n ");
    scanf("%s", new_user->nom);



    printf("Veuillez saisir votre prenom:\n ");
    scanf("%s", new_user->prenom);


    printf("Veuillez saisir votre age:\n ");
    scanf("%d", &new_user->age);

    printf("Veuillez saisir votre identifiant: \n");
    scanf("%s", new_user->identifiant);

    do {
        printf("veuillez creer un mot de passe securisee : \n");
        scanf("%s", new_user->mdp);
    }while (strlen(new_user->mdp)<8);

    printf("Creez l indication de votre compte\n");
    scanf("%s",new_user->indication);

        return new_user;
}

bool mot_de_passe_correct(char *password,utilisateurs*utilisateur) {
    return (strcmp(utilisateur->mdp,password)==0);
}

void enregistrer_utilisateur_dans_fichier_des_utilisateurs(utilisateurs *utilisateur, char*nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "a");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
        return;
    }
    fprintf(fichier, "%s\n", utilisateur->identifiant);
    fclose(fichier);
}

void afficher_liste_utilisateurs(utilisateurs *tete) {
    utilisateurs *temp = tete;
    while (temp != NULL) {
        printf("- %s\n", temp->identifiant);
        temp = temp->next;
    }
}

utilisateurs* rechercher_utilisateur_par_id_dans_fichier(utilisateurs *tete, const char *id, const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
        return NULL;
    }

    char id_fichier[20];
    utilisateurs *temp = tete;

    while (fgets(id_fichier, sizeof(id_fichier), fichier)) {

        id_fichier[strcspn(id_fichier, "\n")] = 0;
        if (strcmp(id_fichier, id) == 0) {
            while (temp != NULL) {
                if (strcmp(temp->identifiant, id) == 0) {
                    fclose(fichier);
                    return temp;
                }
                temp = temp->next;
            }
        }
    }
    fclose(fichier);
    return NULL;
}

void envoyer_invi(utilisateurs *utilisateur, utilisateurs *cible) {
    if (!cible || !utilisateur) {
        printf(RED "Utilisateur ou cible introuvable.\n" RESET);
        return;
    }
    demande *nouvelle_demande = (demande *) malloc(sizeof(demande));
    if (!nouvelle_demande) {
        printf(RED "Memoire insuffisante.\n" RESET);
        return;
    }
    nouvelle_demande->sender=utilisateur;
    nouvelle_demande->suivt = NULL;
    if (!cible->demande) {
        cible->demande = nouvelle_demande;
    } else {
        demande *aux = cible->demande;
        while (aux->suivt) {
            aux = aux->suivt;
        }
        aux->suivt = nouvelle_demande;
    }
    printf(GREEN "Invitation envoyee A %s.\n" RESET, cible->identifiant);
}

void afficher_info_utilisateur(utilisateurs *utilisateur) {
    if (!utilisateur) {
        printf(RED "Erreur: Aucun utilisateur trouve\n" RESET);
        return;
    }
    printf("Nom: %s\nPrenom: %s\nAge: %d\nIdentifiant: %s\n",
           utilisateur->nom, utilisateur->prenom, utilisateur->age, utilisateur->identifiant);
}

void modifier_mdp(utilisateurs *utilisateur,char *new_pass) {
    if (!utilisateur) {
        printf("Erreur : utilisateur invalide\n");
        return;
    }
    char ancien_mdp[20];
    do {
        printf("Veuillez saisir votre ancien mdp\n");
        scanf("%s",ancien_mdp);
    }while (strcmp(utilisateur->mdp, ancien_mdp) != 0);

        strcpy(utilisateur->mdp, new_pass);
        printf("Mot de passe modifiee avec succes , le nouveau mot de passe est %s:\n",new_pass);
}

void modifier_id(utilisateurs *utilisateur,char *new_id) {
    if (!utilisateur) {
        printf(RED "Erreur : utilisateur invalide\n" RESET);
        return;
    }
    char old_id[20];
    strcpy(old_id, utilisateur->identifiant);

    strcpy(utilisateur->identifiant, new_id);
    printf("Identifiant de %s modifiee avec succes en : %s\n",old_id,new_id);
}


void afficher_demandes(utilisateurs *utilisateur) {
    if (!utilisateur) {
        printf("Erreur : utilisateur invalide\n");
    }
    demande*aux=utilisateur->demande;
    while (aux!=NULL) {
        printf("%s Vous avez recu une demande de : %s \n",utilisateur->identifiant,aux->sender->identifiant);
        aux=aux->suivt;
    }

}

void accepter_invi(utilisateurs *user, const char *id_sender) {
    if (!user || !id_sender) {
        printf("Erreur : utilisateur ou identifiant invalide.\n");
        return;
    }

    demande *prev = NULL, *current = user->demande;

    while (current) {
        if (strcmp(current->sender->identifiant, id_sender) == 0) {

            if (user->nb_amis < 1000) {
                user->amis[user->nb_amis] = current->sender;
                user->nb_amis++;
                printf("%s ,Vous etes devenu(e) ami(e) avec %s\n", user->identifiant, current->sender->identifiant);
            } else {
                printf("Erreur : liste d'amis pleine.\n");
                return;
            }

            if (prev) {
                prev->suivt = current->suivt;
            } else {
                user->demande = current->suivt;
            }

            free(current);
            printf("Demande acceptee et supprimee de la liste des demandes.\n");
            return;
        }

        prev = current;
        current = current->suivt;
    }

    printf("Aucune demande trouvée pour l'identifiant : %s\n", id_sender);
}

void refuser_invi(utilisateurs *user, const char *id_sender) {
    if (!user || !id_sender) {
        printf("Erreur : utilisateur ou identifiant invalide.\n");
        return;
    }

    demande *prev = NULL, *current = user->demande;

    // Parcourir la liste des demandes
    while (current) {
        if (strcmp(current->sender->identifiant, id_sender) == 0) {
            if (prev) {
                prev->suivt = current->suivt;
            } else {
                user->demande = current->suivt;
            }

            free(current);
            printf("Demande de %s a ete refusee et supprimee.\n", current->sender->identifiant);
            return;
        }
        prev = current;
        current = current->suivt;
    }

    printf("Aucune demande trouvee venant de l identifiant : %s\n", id_sender);
}

void afficher_amis(utilisateurs *user) {
    if (!user) {
        printf("Erreur : utilisateur invalide.\n");
        return;
    }

    if (user->nb_amis == 0) {
        printf("%s n'a pas encore d'amis.\n", user->identifiant);
        return;
    }

    printf("Voici la Liste des amis de %s :\n", user->identifiant);

    for (int i = 0; i < user->nb_amis; i++) {
        printf("%d. %s %s\n", i + 1, user->amis[i]->prenom, user->amis[i]->nom);
    }
}





int main() {
    int choix_1;
    int choix_2;
    char id[20];
    char password[20];
    utilisateurs *current_user;

    utilisateurs *tete = NULL;
    utilisateurs *aux = NULL;

    char *nom_fichier = "utilisateurs.txt";
    sleep(2);
    printf(GREEN"Remplissage de la base de donnees du reseau WeSaLINK \n"RESET);
    printf("-----------------------------------------------------\n");

    do {
        utilisateurs *nouveau = creer_utilisateurs();
        if (nouveau == NULL) {
            return 1;
        }

        if (tete == NULL) {
            tete= nouveau;
            aux=tete;
        } else {
            aux->next = nouveau;
            aux=aux->next;
        }

        enregistrer_utilisateur_dans_fichier_des_utilisateurs(nouveau, nom_fichier);

        printf("Voulez-vous ajouter un autre utilisateur ?\n");
        sleep(1);
        printf(GREEN"1. AJOUTER UN UTILISATEUR\n"RESET);
        sleep(1);
        printf(RED"2. TERMINER\n"RESET);
        sleep(1);
        scanf("%d", &choix_1);

    } while (choix_1 != 2);

    printf(GREEN"---------------------------------------------------\n"RESET);
    printf(CYAN"   Bienvenue sur WeSaLINK - Le nouveau Reseau Social\n"RESET);
    printf(GREEN"---------------------------------------------------\n"RESET);
    sleep(3);
    printf(BLUE"Le WeSaLINK cest We Safely LINK realisee par \n"RESET);
    printf(BLUE"Wejdene (We) et Sarah(Sa) \n"RESET);
    sleep(3);
    printf(MAGENTA"Connectez-vous avec vos amis, partagez vos idees\n"RESET);
    printf(MAGENTA"et explorez un reseau social innovant !\n"RESET);
    sleep(2);
    printf(YELLOW"veuillez vous connecter a votre compte s il vous plait ?\n"RESET);
    printf("---------------------------------------------------------------------------------------------\n");
    sleep(1);
    do {
        printf("Veuillez donner l identifiant du compte deja existant :\n");
        scanf("%s",id);
        printf("Veuillez saisir le mot de passe : \n");
        scanf("%s",password);

        current_user=rechercher_utilisateur_par_id_dans_fichier(tete,id,nom_fichier);

        if (current_user == NULL) {
            printf(RED"On a pas pu trouver l utilisateur dans la base \n"RESET);
        }
    }while ((rechercher_utilisateur_par_id_dans_fichier(tete,id,nom_fichier)==NULL)||(mot_de_passe_correct(password,current_user)==false));

    printf(GREEN"l utilisateur %s est bien dans la base\n "RESET,current_user->identifiant);

    printf("-----------------------------------------------------------------------------------------------\n");

    int choix_3;
    int choix_4;
    int choix_passage;
    int id_pub_a_modif;
    char nouveau_contenu[256];
    int id_pub_a_supp;

    printf(BLUE"En vous connectant VOUS etes disposee directement sur l Espace de Gestion des Publications\n"RESET);
    sleep(2);
    printf("-------------------------------------------------------------------------------------------------------\n");
    printf(BLUE"Que souhaitez vous faire dans la gestion des publications\n"RESET);
    sleep(1);
    printf("-------------------------------------------------------------------------------------------------------\n");
    do{
        printf(MAGENTA"1.Poster une publication dans votre profil \n"RESET);
        sleep(1);
        printf(MAGENTA"2.Modifier une publication\n"RESET);
        sleep(1);
        printf(MAGENTA"3.Afficher votre publication la plus ancienne\n"RESET);
        sleep(1);
        printf(MAGENTA"4.Afficher votre publication la plus recente\n"RESET);
        sleep(1);
        printf(MAGENTA"5.Supprimer une publication\n"RESET);
        printf("---------------------------------------------------------------------------------------------------\n");
        sleep(1);
        scanf("%d", &choix_2);


        switch (choix_2) {
            case 1:do {
                enfiler_publication(current_user);
                printf("Souhaitez vous ajouter une autre publication ? \n");
                printf(GREEN"1.OUI\n"RESET);
                printf(RED"2.NON\n"RESET);
                scanf("%d",&choix_3);
            }while (choix_3==1);
            break;

            case 2:printf("Veuillez donner l identifiant de la publication a modifiee \n");
            fflush(stdin);
            scanf("%d",&id_pub_a_modif);
            printf(BLUE"Donner le nouveau contenu que vous souhaitez avoir\n"RESET);
            fflush(stdin);
            fgets(nouveau_contenu,256,stdin);
            modifier_publication(current_user,id_pub_a_modif,nouveau_contenu);
            break;

            case 3:publications_anciennes();
            break;

            case 4:publications_recentes();
            break;

            case 5:printf(BLUE"Veuillez donner l id de la publication que vous souhaiter supprimer\n"RESET);
            scanf("%d",&id_pub_a_supp);
            supprimer_publications(current_user,id_pub_a_supp);
            break;


        }

        printf(MAGENTA"voulez vous rester dans l espace de gestion des publications \n"RESET);
        sleep(1);
        printf(GREEN"1.oui\n"RESET);
        sleep(1);
        printf(RED"2.non\n"RESET);
        scanf("%d",&choix_4);
    }while (choix_4==1);
    sleep(2);
    printf("---------------------------------------------------------------------------------------\n");
    printf(RED"Vous n etes plus sur l espace de Gestion des Publications\n"RESET);
    printf("--------------------------------------------------------------------------------------\n");



    char id_user_ciblee[20];
    utilisateurs*user_ciblee;
    int choice_1;
    int choice_2;
    int choix_further_tache;
    char indication[5];
    char new_password[20];
    char nouvel_identifiant[20];
    char ancien_id[20];
    int choix_quitter_tout_espace;


    do {
        printf("Voulez vous passer a l Espace de Gestion des Utilisateurs ou bien Gestion des amities ou bien Quitter\n");
        sleep(1);
        printf(BLUE"1.Passer a l Espace de Gestion des Utilisateurs\n"RESET);
        sleep(1);
        printf(MAGENTA"2.Gestion des amities\n"RESET);
        sleep(1);
        printf(RED"3.Quitter\n"RESET);
        scanf("%d",&choix_passage);
        if(choix_passage==1) {
            do {
                printf("--------------------------------------------------------------------------\n");
                printf(GREEN"Vous etes bien sur l Espace de Gestion Des Utilisateurs\n"RESET);
                printf("--------------------------------------------------------------------------\n");
                printf("Qu est ce que vous souhaitez faire\n");
                printf("--------------------------------------------------------------------------\n");
                sleep(1);
                printf(MAGENTA"1.Envoyer une invitation a un utilisateur\n"RESET);
                sleep(1);
                printf(MAGENTA"2.Afficher les informations de votre profil\n"RESET);
                sleep(1);
                printf(MAGENTA"3.Modifier votre mot de passe\n"RESET);
                sleep(1);
                printf(MAGENTA"4.Modifier votre identifiant \n"RESET);
                printf("---------------------------------------------------------------------------\n");
                scanf("%d",&choice_2);

                switch (choice_2) {
                    case 1:do {
                        printf("veuillez donner l identifiant de l utilisateur a qui vous souhaitez envoyer une invitation\n");
                        scanf("%s",id_user_ciblee);
                        user_ciblee=rechercher_utilisateur_par_id_dans_fichier(tete,id_user_ciblee,nom_fichier);
                        if (user_ciblee == NULL) {
                            printf(RED"on a pas pu trouver l utilisateur ciblee dans la base\n"RESET);
                        }
                        else {
                            envoyer_invi(current_user, user_ciblee);
                        }
                        printf("souhaitez vous envoyer une autre invitation\n");
                        sleep(1);
                        printf(GREEN"1.OUI\n"RESET);
                        sleep(1);
                        printf(RED"2.NON\n"RESET);
                        scanf("%d", &choice_1);
                    } while (choice_1 != 2);
                    break;

                    case 2: afficher_info_utilisateur(current_user);
                        printf("Si vous souhaitez afficher votre mot de passe il va falloir donner votre indication\n");
                        printf(GREEN".INDIQUER MON MDP\n"RESET);
                        sleep(1);
                                printf("Donnez l indication\n");
                            scanf("%s",indication);
                            if (strcmp(current_user->indication,indication)==0) {
                                printf(GREEN"Vous etes bien le detenteur de ce compte par consequent votre mdp va etre affichee\n"RESET);
                                sleep(1);
                                printf(current_user->mdp,"\n");
                            }
                            else {
                                printf(RED"Indication Incorrecte ce qui implique que vous n etes pas le detenteur du compte \n"RESET);
                            }




                    case 3:do {
                        printf(MAGENTA"Veuillez saisir le nouveau mdp\n"RESET);
                        scanf("%s",new_password);
                    }while (strlen(new_password)<8);
                    modifier_mdp(current_user,new_password);
                    break;

                    case 4:printf("Veuillez donner le nouvel identifiant\n");
                    scanf("%s",nouvel_identifiant);
                    modifier_id(current_user,nouvel_identifiant);
                    strcpy(ancien_id,current_user->identifiant);
                    break;

                }


                printf("voulez vous effectuer une autre tache sur l espace de gestion des utilisateurs\n");
                sleep(1);
                printf(GREEN"1.oui\n"RESET);
                sleep(1);
                printf(RED"2.non\n"RESET);
                scanf("%d",&choix_further_tache);
            }while (choix_further_tache==1);
        }


        int choix_amitie;
        int choix_rester_dans_espace_amitie;
        char id_a_gerer[20];
    // on va juste simuler que cette personne a recu 3 invitations et on verra comment elle va les gerer
        utilisateurs* simulating_user1=current_user->next;
        utilisateurs* simulating_user2=simulating_user1->next;
        utilisateurs* simulating_user3=simulating_user2->next;

        if (choix_passage==2) {
            envoyer_invi((simulating_user1),current_user);
            envoyer_invi((simulating_user2),current_user);
            envoyer_invi((simulating_user3),current_user);

            do {
                printf("--------------------------------------------------------------------------\n");
                printf(MAGENTA"Vous etes bien sur l Espace de Gestion Des Amitites\n"RESET);
                printf("--------------------------------------------------------------------------\n");
                printf("Qu est ce que vous souhaitez faire\n");
                printf("--------------------------------------------------------------------------\n");
                printf(BLUE"1.Afficher la liste de demandes que vous avez\n"RESET);
                sleep(1);
                printf(BLUE"2.Accepter une invitation\n"RESET);
                sleep(1);
                printf(BLUE"3.Refuser une invitation\n"RESET);
                sleep(1);
                printf(BLUE"4.Afficher la liste des amis\n"RESET);
                scanf("%d",&choix_amitie);


                switch (choix_amitie) {
                    case 1:
                        afficher_demandes(current_user);
                    break;

                    case 2:
                        printf(GREEN"Quelle identifiant souhaitez vous accepter ?\n"RESET);
                    scanf("%s",id_a_gerer);
                    accepter_invi(current_user,id_a_gerer);
                    break;

                    case 3:
                        printf(RED"Quelle identifiant souhaitez vous refuser ?\n"RESET);
                    scanf("%s",id_a_gerer);
                    refuser_invi(current_user,id_a_gerer);
                    break;

                    case 4:
                        afficher_amis(current_user);
                    break;


                }

                printf("Voulez vous rester encore dans l Espace de Gestion des Amities et Effectuer d autres taches\n");
                sleep(1);
                printf(GREEN"1.Oui\n"RESET);
                sleep(1);
                printf(RED"2.Non\n"RESET);
                scanf("%d",&choix_rester_dans_espace_amitie);
            }while (choix_rester_dans_espace_amitie==1);
        }
        printf(BLUE"Souhaitez vous quitter la platforme?\n"RESET);
        sleep(1);
        printf(GREEN"1.Non je veux pas quitter\n"RESET );
        sleep(1);
        printf(RED"2.Oui je veux quitter\n"RESET);
        scanf("%d",&choix_quitter_tout_espace);
    }while (choix_quitter_tout_espace!=2);


    printf(MAGENTA"Merci d avoir passer ce temps sur WeSaLINK\n"RESET);




}

