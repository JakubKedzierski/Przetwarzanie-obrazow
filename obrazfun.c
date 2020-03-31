#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "struct.h"

/* Funkcje programu */

/*  Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
int wyswietl(dane *obraz_w) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */
  int i,j,k;
  char n_pliku[100]="Testowy";
  FILE *test; /*wskaznik do pliku testowego pomocnego przy wyswietlaniu*/
  test = fopen(n_pliku,"wra");
  
/*Tablica wskaznikow dzieki ktorej mozna operowac na tablicy dwuwymiarowej*/
int (*piksele)[obraz_w->wymx][obraz_w->typ];
piksele=(int(*)[obraz_w->wymx][obraz_w->typ])obraz_w->piksele;

/*zapisanie danych do pomocniczego pliku dzieki czemu mozemy wyswietlic obraz po zmianach bez dokonywania zapisu*/
fprintf(test,"P%d\n%d %d\n %d\n",obraz_w->magic,obraz_w->wymx,obraz_w->wymy,obraz_w->szarosci);
for(i=0;i<obraz_w->wymy;i++){
    for(j=0;j<obraz_w->wymx;j++){
      for(k=0;k<obraz_w->typ;k++){
	fprintf(test,"%d  ",piksele[i][j][k]);
      }
    }
  }
 
  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */

  fclose(test);
  return 0;
}

int negatyw(dane *obraz_n) {
  int i,j,k;

/*Tablica wskaznikow dzieki ktorej mozna operowac na tablicy trojwymiarowej*/
int (*piksele)[obraz_n->wymx][obraz_n->typ]; 
piksele=(int(*)[obraz_n->wymx][obraz_n->typ]) obraz_n->piksele;

  for(i=0;i<obraz_n->wymy;i++){
    for(j=0;j<obraz_n->wymx;j++){
      for(k=0;k<obraz_n->typ;k++){
      piksele[i][j][k]=(obraz_n->szarosci-piksele[i][j][k]); /*od max szarosci odejmij wartosc punktu*/
      }
    }
  }
  return 0;
}

int progowanie(wopcje *opcje,dane *obraz_p){
  int i,j,k;

/*Tablica wskaznikow dzieki ktorej mozna operowac na tablicy trojwymiarowej*/
int (*piksele)[obraz_p->wymx][obraz_p->typ]; 
piksele=(int(*)[obraz_p->wymx][obraz_p->typ]) obraz_p->piksele;
  
      for(i=0;i<obraz_p->wymy;i++){
	for(j=0;j<obraz_p->wymx;j++){
	  for(k=0;k<obraz_p->typ;k++){
	  if(piksele[i][j][k]<=opcje->w_progu*(obraz_p->szarosci)/100){ /*gdy szarosc w odpowiednim przedziale*/
	    piksele[i][j][k]=0;
	  }else
	    piksele[i][j][k]=obraz_p->szarosci;
	  }
	}
      }
  
      return 0;
}
	      
int rozmywanie_poziome(dane *obraz_r){
  int i,j,k;

  
                     /*tablica glowna*/ 
/*Tablica wskaznikow dzieki ktorej mozna operowac na tablicy trojwymiarowej*/
int (*piksele)[obraz_r->wymx][obraz_r->typ]; 
piksele=(int(*)[obraz_r->wymx][obraz_r->typ]) obraz_r->piksele;
/******************************************************************************/

                  /*tablica pomocnicza*/
/*deklaracja dynamicznej tablicy pomocniczej*/
obraz_r->pomoc = malloc(obraz_r->typ*obraz_r->wymx*obraz_r->wymy*sizeof(int));
/*Tablica wskaznikow dzieki ktorej mozna operowac na pomocniczej tablicy trojwymiarowej*/
int (*pomoc)[obraz_r->wymx][obraz_r->typ];
pomoc=(int(*)[obraz_r->wymx][obraz_r->typ])obraz_r->pomoc;
/***********************************************************************************************/  
  

  for(i=0;i<obraz_r->wymy;i++){   
    for(j=0;j<obraz_r->wymx;j++){
      for(k=0;k<obraz_r->typ;k++){
      pomoc[i][j][k]=piksele[i][j][k]; /*tablica pomocnicza do funkcji-aby mozliwe bylo wykonanie operacji na kilku punktach jednoczesnie*/
      }
    }
  }
  
  for(i=0;i<obraz_r->wymy;i++){   
    for(j=1;(j<obraz_r->wymx-1);j++){
      /*j=1 i j<obraz_r->wymx-1 aby nie wychodzic poza zakres tablicy*/
      /*operacje na pojedycznym elemencie tablicy przy uzyciu dwoch sasiednich*/
      for(k=0;k<obraz_r->typ;k++){
        piksele[i][j][k]=(pomoc[i][j-1][k]+pomoc[i][j][k]+pomoc[i][j+1][k])/3;
      }
    }
 }
  free(pomoc);
  return 0;
}

int rozciaganie_histogramu(dane *obrazroz){
  int i=0,j=1,k=0,max,min,nowaszar;

/*Tablica wskaznikow dzieki ktorej mozna operowac na tablicy trojwymiarowej*/
int (*piksele)[obrazroz->wymx][obrazroz->typ]; 
piksele=(int(*)[obrazroz->wymx][obrazroz->typ]) obrazroz->piksele;
  

  
  max=piksele[i][j-1][k];  /*max i min aby odpowiednio posortowac elementy obrazu*/
  min=piksele[i][j-1][k];

      for(i=0;i<obrazroz->wymy;i++){
	for(j=1;j<obrazroz->wymx;j++){
	  for(k=0;k<obrazroz->typ;k++){
	  /*szukanie max wartosci szarosci punktu*/

       if(piksele[i][j][k]>max)  
	 max=piksele[i][j][k];   /*sortowanie max*/

       if(piksele[i][j][k]<min)
	 min=piksele[i][j][k];    /*sortowanie min*/
	  }
	}
      }

      for(i=0;i<obrazroz->wymy;i++){
	for(j=0;j<obrazroz->wymx;j++){
	  for(k=0;k<obrazroz->typ;k++){
	  piksele[i][j][k]=((piksele[i][j][k]-min)*obrazroz->szarosci)/(max-min); /*rozciaganie histogramu*/
	  }
	}
      }
      return 0;
}
/*dodatkowa funckja maski*/
int filtracja_usredniajaca(dane *obraz_f){
  int i=0,j=0,k,suma=0,max,min;

  
                     /*tablica glowna*/ 
/*Tablica wskaznikow dzieki ktorej mozna operowac na tablicy trojwymiarowej*/
int (*piksele)[obraz_f->wymx][obraz_f->typ]; 
piksele=(int(*)[obraz_f->wymx][obraz_f->typ]) obraz_f->piksele;
/***********************************************************************************************/  
 
                  /*tablica pomocnicza*/
/*deklaracja dynamicznej tablicy pomocniczej*/
obraz_f->pomoc = malloc(obraz_f->typ*obraz_f->wymx*obraz_f->wymy*sizeof(int));
/*Tablica wskaznikow dzieki ktorej mozna operowac na pomocniczej tablicy trojwymiarowej*/
int (*pomoc)[obraz_f->wymx][obraz_f->typ];
pomoc=(int(*)[obraz_f->wymx][obraz_f->typ])obraz_f->pomoc;
/***********************************************************************************************/  
  
  /*tablica wspolczynnikow*/
 int tab[9] = {1,1,1,1,1,1,1,1,1} ;
 
 /*zliczanie sumy (na potrzeby wzoru,wszystkie wspolczynniki dodatnie)*/
 for(i=0;i<9;i++){
    suma=suma+tab[i]; 
  }

    for(i=0;i<obraz_f->wymy;i++){   
    for(j=0;j<obraz_f->wymx;j++){
      for(k=0;k<obraz_f->typ;k++){
      pomoc[i][j][k]=piksele[i][j][k]; /*tablica pomocnicza do funkcji*/
      }
    }
  }

 
  for(i=1;(i<(obraz_f->wymy-1));i++){
    for(j=1;(j<(obraz_f->wymx-1));j++){
      for(k=0;k<obraz_f->typ;k++){
      /*operacje wykonuje sie bez puntkow skrajnych obrazu*/
      
 piksele[i][j][k]=((tab[0]*pomoc[i-1][j-1][k])+(tab[1]*pomoc[i-1][j][k])+(tab[2]*pomoc[i-1][j+1][k])+tab[3]*pomoc[i][j-1][k]+tab[4]*pomoc[i][j][k]+tab[5]*pomoc[i][j+1][k]+tab[6]*pomoc[i+1][j-1][k]+tab[7]*pomoc[i+1][j][k]+tab[8]*pomoc[i+1][j+1][k]);
 piksele[i][j][k]=piksele[i][j][k]/suma;       
      }
    }
  }
  
  free(pomoc);
      return 0;
}

int konwersja(dane *obrazk){
  int i,j,k;
 
  if(obrazk->magic!=3){
    fprintf(stderr,"Ta operacje mozna wykonac tylko na obrazach kolorowych (PPM). \nOdczytany obraz nie jest kolorowy !\n");
    return 0;
  }

                     /*tablica glowna (pierweotna)*/ 
/*Tablica wskaznikow dzieki ktorej mozna operowac na tablicy trojwymiarowej*/
int (*pierwotna)[obrazk->wymx][obrazk->typ]; 
pierwotna=(int(*)[obrazk->wymx][obrazk->typ]) obrazk->piksele;
/******************************************************************************/  
                    /*tablica pomocnicza*/
         /*deklaracja dynamicznej tablicy pomocniczej*/
obrazk->pomoc = malloc(obrazk->typ*obrazk->wymx*obrazk->wymy*sizeof(int));
/*Tablica wskaznikow dzieki ktorej mozna operowac na pomocniczej tablicy trojwymiarowej*/
int (*pomoc)[obrazk->wymx][obrazk->typ];
pomoc=(int(*)[obrazk->wymx][obrazk->typ])obrazk->pomoc;

/*przepisanie wartosci pikseli do tablicy pomocniczej*/ 
  for(i=0;i<obrazk->wymy;i++){   
    for(j=0;j<obrazk->wymx;j++){
      for(k=0;k<obrazk->typ;k++){
      pomoc[i][j][k]=pierwotna[i][j][k]; 
      }
    }
  }
/***********************************************************************************************/    


/* Po konwersji obraz z PPM staje sie PGM wiec musimy zmienic typ i "magica" */
 obrazk->typ=1;
 obrazk->magic=2;
/*musimy zmienic tez rozmiar tablicy na ktorej operujemy (z operowania na 3 wymiarach przechodzimy na 2 wymiary*/

free(pierwotna); //zwalniamy miejsce dotychczasowej tablicy (pierwotnej)

/*i tworzymy nowa tablice*/
obrazk->piksele = malloc(obrazk->typ*obrazk->wymx*obrazk->wymy*sizeof(int));
int (*piksele)[obrazk->wymx][obrazk->typ]; 
piksele=(int(*)[obrazk->wymx][obrazk->typ]) obrazk->piksele; /* typ=1 */

         /*konwersja*/
 for(i=0;i<obrazk->wymy;i++){
  for(j=0;j<obrazk->wymx;j++){
    for(k=0;k<obrazk->typ;k++){ 
       piksele[i][j][k]=(pomoc[i][j][0]+pomoc[i][j][1]+pomoc[i][j][2])/3;
     }
   }
 }

 free(pomoc); 
 return 0;
}

