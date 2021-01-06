#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define VMIN 5
#define VMAX 30

//création des types structures
typedef struct
{
   int l;
   int c;
}T_Case;

typedef struct //création d'une structure qui permet d'enregistrer les paramètres du partie
{
   int nlig;
   int ncol;
   int niveau;
   int next;
   int nban;
}param_nim;

T_Case T_Tab_Case[VMAX*VMAX];
T_Case pion;
T_Case ban[VMAX*VMAX];
int nim[VMAX][VMAX];
//fonctions
/*fonctions permettant de saisir et de retourner un entier
 compris entre deux bornes donnes*/
int Lire_Entier(int min,int max)
{
   int var;
   do
   {
      scanf("%d", &var);
      if(var<min || var>max)
         printf("\nLe nombre doit etre compris entre %d et %d. Veuillez ressaisir le nombre: ", min, max);
   }
   while(var<min || var>max);
   return var;
}
int max(int nb1, int nb2) //fonction max qui permet de retourner le maximum entre 2 nombres
{
   int maximum;
   if (nb1<nb2)
      maximum=nb2;
   else
      maximum=nb1;

   return maximum;
}
//fonctions hasard
int Hasard(int min,int max)
{
   return rand()%max+min+1;
}
int bannissable(int i, param_nim tab)
{
   int j;
    // test des cases sur la bordure extérieure
   if ((T_Tab_Case[i].l==0) || (T_Tab_Case[i].c==0) || (T_Tab_Case[i].l==tab.nlig-1) || (T_Tab_Case[i].c==tab.ncol-1))
      return (0);
   for (j=0; j<tab.nban; j++) //parcours du tableau de cases bannies
   {
        //si la case est déjà bannie
        // on retourne 0
      if ((T_Tab_Case[i].l == ban[j].l) && (T_Tab_Case[i].c == ban[j].c))
      {
         return (0);
      }
      if ((T_Tab_Case[i-tab.ncol+1].l == ban[j].l) && (T_Tab_Case[i-tab.ncol+1].c == ban[j].c))
      {
            //si la case en diagonale en haut à droite de celle actuelle est bannie
            // on retourne 0
         return (0);
      }
      if ((T_Tab_Case[i+tab.ncol-1].l == ban[j].l) && (T_Tab_Case[i+tab.ncol-1].c == ban[j].c))
      {
            //si la case en diagonale en bas à gauche de celle actuelle est bannie
            // on retourne 0
         return (0);
      }
   }
   return (1);
}

void hasard_ban(param_nim tab)
{
   int i,j,i_tab_ban=0;
   srand((unsigned int)time(NULL));
   while (i_tab_ban < tab.nban)
   {
        // au hasard
      i = rand()%(tab.nlig*tab.ncol)+1;

        //si la case en diagonale en haut à droite de celle actuelle n'est pas bannie, alors on rajoute la case actuelle dans les bannies
      if (bannissable(i, tab) == 1)
      {
         ban[i_tab_ban].l=T_Tab_Case[i].l;
         ban[i_tab_ban].c=T_Tab_Case[i].c;
         i_tab_ban+=1;
      }
   }

}


//Lance la fonction lire entier pour la fonctions paramètres
param_nim Lire_Parametres()
{
   param_nim resultat;
   srand((unsigned int)time(NULL));
   printf("Parametres du jeu\n");

   printf("Nombre de lignes: "); resultat.nlig=Lire_Entier(VMIN,VMAX);

   printf("Nombre de colonnes: "); resultat.ncol=Lire_Entier(VMIN,VMAX);

   printf("/***1.Niveau debutant***/\n");
   printf("/***2.Niveau moyen***/\n");
   printf("/***3.Niveau expert***/\n");
   printf("/***4.Niveau virtuose***/\n");
   printf("Veuillez saisir le niveau de difficultes que vous souhaiter : ");
   resultat.niveau=Lire_Entier(1,4);

   printf("qui commence ?\n Ordinateur (1) - Joueur (2): ");
   resultat.next=Lire_Entier(1,2);
   printf("\n");
   resultat.nban=rand()%max(resultat.nlig,resultat.ncol)+1;
   return resultat;
}
void remplit(param_nim tab)
{
   int i,j;
   for (i=0;i<tab.nlig;i++)
   {
      for(j=i*tab.ncol;j<tab.ncol*(i+1);j++)
      {
         T_Tab_Case[j].l=i;
         T_Tab_Case[j].c=j%tab.ncol;
      }
   }
}

void Affiche_grille(param_nim tab,T_Case pion)
{
   int i,j,k,bannie;

   printf("  ");
    //affichage du numéro de colonne
   for (i=0; i<tab.ncol; i++)
   {
      if(i<=9)
         printf("   %d",i+1);
      else
         printf("  %d",i+1);
   }
   printf("\n");

   for (i=0; i<tab.nlig; i++)
   {
      printf("%2d |",i+1);
      for(j=i*tab.ncol; j<tab.ncol*(i+1);j++)
      {
         bannie=0;

         for (k=0; k<tab.nban; k++)
         {
            if (T_Tab_Case[j].l==ban[k].l && T_Tab_Case[j].c==ban[k].c)
               bannie=1;
         }

         if (T_Tab_Case[j].l==pion.l && T_Tab_Case[j].c==pion.c)
            printf(" O |");

         else if (bannie)
            printf(" X |");

         else
            printf(" - |");
      }
      printf("\n");
   }
}
void calcul_nimbers(param_nim tab)
{

   int i=tab.nlig-1, j=tab.ncol-1, k, bannie, compt=1, full=1, vois;

    //on commence par numéroter la case en bas ?gauche : le puit
   nim[i][j]=0;

    //tant que toutes les cases ne sont pas numérotées
   while (full<=tab.nlig*tab.ncol)
   {
      vois=0;


      for (k=1; k<=2; k++)
      {
            //si la case est ?l'intérieur du tableau
         if ((i+k)<tab.nlig)
         {
                //si la case en dessous possède la valeur 0 (différent de 1)
            if (nim[i+k][j]!=1)
               vois=1;
         }

         if ((j+k)<tab.ncol)
         {
                //si la case ?droite possède la valeur 0 (différent de 1)
            if (nim[i][j+k]!=1)
               vois=1;
         }
      }


   }
}

int coup_pos(param_nim par, T_Case pion, T_Case *tab)
{

   int mouv=1, i_coup_pos=0;
   int i,j;

   printf("Vous pouvez vous deplacer aux cases :\n");

    //on cherche les cases sur la même colonne pour lesquelles le mouvement est possible
   for (j=1; j<=2; j++)
   {
      i=0;
        //on vérifie toutes les cases du tableau des cases bannies
      while (i<par.nban && mouv==1)
      {
            //printf("(%d;%d)",ban[i].ilig+1,ban[i].icol+1);
            //si la case en dessous est bannie le mouvement n'est pas possible
         if (pion.l+j==ban[i].l && pion.c==ban[i].c)
            mouv=0;
            //si la case en dessous est en dehors du tableau, le mouvement n'est pas possible
         else if (pion.l+j >= par.nlig)
            mouv=0;
            //sinon le mouvement est possible
         else
            mouv=1;
         i+=1;
      }

      if (mouv==1)
      {
            //on affiche la case sur laquelle le mouvement est possible
       printf("(%d:%d)", pion.l+j+1, pion.c+1);

            //et on rajoute cette case dans le tableau des coups possibles
         tab[i_coup_pos].l=pion.l+j;
         tab[i_coup_pos].c=pion.c;
         i_coup_pos+=1;
      }
   }

    //on refait la même opération pour les cases en lignes
   mouv=1;
   for (j=1; j<=2; j++)
   {
      i=0;
      while (i<par.nban && mouv==1)
      {
         if (pion.l==ban[i].l && pion.c+j==ban[i].c)
            mouv=0;
         else if (pion.c+j >= par.ncol)
            mouv=0;
         else
            mouv=1;
         i+=1;
      }

      if (mouv==1)
      {
         printf("(%d:%d)", pion.l+1, pion.c+j+1);

         tab[i_coup_pos].l=pion.l;
         tab[i_coup_pos].c=pion.c+j;
         i_coup_pos+=1;
      }
   }
   printf("\n\n");

   return i_coup_pos;
}

T_Case coup_joueur(param_nim par, T_Case pion)

{
   int i, l=0, c=0, bon;
   int nb_coup_pos,ChoixDep;
   T_Case tab_coup_pos[4];

   nb_coup_pos=coup_pos(par, pion, tab_coup_pos);



   while (l==0 && c==0)
   {
      i=0;
      printf("Choisir la case dans laquelle vous souhaitez deplacer votre pion :\n");


      scanf("%d%d",&l,&c);
      printf("\n");
      while((l > pion.l+3)||(c > pion.c+3))
      {
        printf("Impossible veuiller saisir un deplacement valide:\n");
        scanf("%d%d",&l,&c);
        printf("\n");
      }
      while (i<nb_coup_pos && ((l-1) != tab_coup_pos[i].l || (c-1) != tab_coup_pos[i].c))
      {
         i+=1;
      }


      if ((l-1) == tab_coup_pos[i].l && (c-1) == tab_coup_pos[i].c)
         bon=1;
   }


   pion.l=l-1;
   pion.c=c-1;

   Affiche_grille(par, pion);
   printf("Vous avez deplace le pion a la case (%d:%d)\n", pion.l+1, pion.c+1);

   return (pion);
}

T_Case coup_ordi_hasard(param_nim par, T_Case pion)
{
   int i, nb_coup_pos;
   T_Case tab_coup_pos[4];

   nb_coup_pos=coup_pos(par, pion, tab_coup_pos);

   i=rand()%nb_coup_pos;

   pion.l=tab_coup_pos[i].l;
   pion.c=tab_coup_pos[i].c;

   Affiche_grille(par, pion);
   printf("L'ordinateur a deplace le pion a la case (%d:%d)\n\n", pion.l+1, pion.c+1);

   return (pion);
}

T_Case coup_ordi_gagnant(param_nim par, T_Case *tab_gagnant, T_Case pion)
{
   int i, j, k, nb_coup_pos, pos=0;
   T_Case tab_coup_pos[4];

   nb_coup_pos=coup_pos(par, pion, tab_coup_pos);

   for (i=0; i<nb_coup_pos; i++)
   {
      for (j=0; j<par.nlig; j++)
      {
         for (k=0; k<par.ncol; k++)
         {
            if (tab_coup_pos[i].l==j && tab_coup_pos[i].c==k && nim[j][k]==0)
            {
               tab_gagnant[pos].l = tab_coup_pos[i].l;
               tab_gagnant[pos].c = tab_coup_pos[i].c;
               pos+=1;
            }
         }
      }
   }



   if (pos==0)
   {
      i=rand()%nb_coup_pos;

      pion.l=tab_coup_pos[i].l;
      pion.c=tab_coup_pos[i].c;
      printf("Pas de coup gagnant possible, coup au hasard\n");
   }
   else
   {
      i=rand()%pos;
      pion.l = tab_gagnant[pos-1].l;
      pion.c = tab_gagnant[pos-1].c;
      printf("coup gagnant effectue\n");
   }


   Affiche_grille(par, pion);
   printf("L'ordinateur a deplace le pion a la case (%d:%d)\n\n", pion.l+1, pion.c+1);

   return (pion);
}
int main()
{
//declarations de variables
   int nlig,ncol, Has,joue,continuerpartie=1;
   T_Case pion;
   int fin=0;
   T_Case *tab_gagnant;
   pion.l=0;
   pion.c=0;
   char *str1="TU AS PERDU !";
   char *str2="TU AS GAGNE !";
   char *str3;
do
{
   param_nim params;
   printf("---JEU DE NIM---\n");
   params=Lire_Parametres();
   remplit(params);
   printf("C'est parti !\n");
   hasard_ban(params);
   Affiche_grille(params,pion);

 //On "démarre" la partie jusqu'a ce qu'elle soit gagnée
   do
   {
      switch(params.next)
      {
         case 1://l'ordi joue
            {
               Has=Hasard(1,100);
               switch(params.niveau)
               {
                  case 1:
                     {
                        if(Has<=10)
                        {
                           pion=coup_ordi_gagnant(params,tab_gagnant,pion);
                        }
                        else if(Has>10)
                        {
                           pion=coup_ordi_hasard(params,pion);
                        }
                        break;
                     }
                  case 2:
                     {
                        if(Has<=50)
                        {
                           pion=coup_ordi_gagnant(params,tab_gagnant,pion);
                        }
                        else if(Has>50)
                        {
                           pion=coup_ordi_hasard(params,pion);
                        }
                        break;
                     }
                  case 3:
                     {
                        if(Has<=90)
                        {
                           pion=coup_ordi_gagnant(params,tab_gagnant,pion);
                        }
                        else if(Has>90)
                        {
                           pion=coup_ordi_hasard(params, pion);
                        }
                        break;
                     }
                  case 4:
                     {
                        pion=coup_ordi_gagnant(params,tab_gagnant,pion);
                     }
               }
               params.next=2;


               //Si victoire
               if((pion.l==params.nlig-1)&&(pion.c==params.ncol-1))
               {
                  fin=1;
                  str3=str1;
               }

               break;
            }




         case 2://joueur joue
            {
               pion=coup_joueur(params,pion);
               params.next=1;

               //Si victoire
               if((pion.l==params.nlig-1)&&(pion.c==params.ncol-1))
               {
                  fin=1;
                  str3=str2;
               }
               break;
            }

      }
   }
   while (fin!=1);

   printf("\nC'est termine, %s",str3);
   printf("\n\nNous vous remercions d'avoir joue.\n\n/-- Mauchien Thomas--Lantz Theo--Giron Maxence--/\n");
      printf("Voulez vous refaire une partie? 1 ou 0\n");
   scanf("%d",&continuerpartie);
   }
   while(continuerpartie==1);
}
