#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "obrazki.h" /*dolaczenie naglowkow funkcji*/
#include "struct.h"  /*dolaczenie struktury z obrazami*/

/*/////////////////////////////////////////////////////////

PRZETWARZANIE OBRAZOW 2
Program sklada sie z 6 modulow:
-obrazmain.c :funkcja main,
-glownefun.c :funkcje do odczytu i zapisu obrazu,
-obrazfun.c :pozostale funkcje programu
-obrazki.c : przetwarzanie opcji wywolania,
-struct.h : struktury uzyte w programie,
-obrazki.h : naglowki/prototypy funkcji,
-makefile : plik pomocniczy przy kompilacji calosci 

KOMPILACJA:
komenda : make
czyszczenie folderu z niepotrzebnych plikow po kompilacji : make clean
po kompilacji powstaje plik : obrazki2.out

OPIS:
-Program dziala poprawnie, umieszczono funkcje i polecenia podane na stronie programowania
i na laborkach.
-Wywolanie programu: 
nazwa_programu (./obrazki2.out) { [-i nazwa] [-o nazwa] [-p liczba] [-n] [-rx] [-d] [-h] [-f] [-ms] }    

gdzie [-n negatyw] [-p <próg> progowanie] [-rx rozmycie poziome]                
[-h rozciąganie histogramu] [-s konwersja do szarości] [-d wyswietlanie]      
[-f filtracja usredniajaca] [-i <nazwa> -nazwa pliku do wczytania, gdy '-' wczytuje z stdin]        
[-o <nazwa_pliku> nazwa pliku do zapisu, gdy '-' wypisuje na stdout] [-ms konwersja do szarosci] 

UWAGA!!!
Gdyby program nie dzialal i wyskakiwaly bledy typu: 
"display: no decode delegate for this image format" 
Nalezy zmienic opoznienie programu (w tym pliku (tj.obrazmain.c), linia 121)
na wieksze. 

Program wykonal:Jakub Kedzierski 248915

/////////////////////////////////////////////////////////*/


int main(int argc, char **argv) {
  
int czas; /*sluzy do zatrzymania na chwile programu aby display zdarzylo sie wykonac przed remove*/

wopcje opcje;
/*wywolanie funkcji przetwarzaj opcje*/
printf("%d\n", przetwarzaj_opcje(argc,argv,&opcje));  

/*dynamiczne przydzielenie miejsca dla struktury z obrazem*/
 dane *obraz1; 
 obraz1 = (dane *)malloc(sizeof(dane));

 if(obraz1==NULL)
 fprintf(stderr,"Blad przydzialu pamieci");

/*wywolania funkcji*/ 

 if(opcje.plik_we==NULL) /*gdy zly plik*/
   return 1; 
 
 if(opcje.czytaj==1){ /*gdy "flaga podniesiona"*/
 czytaj(&opcje,obraz1);
 }

 if(opcje.negatyw==1){
    negatyw(obraz1);
      obraz1->nazwa_op="negatyw";
  }
    
  if(opcje.progowanie==1){
    progowanie(&opcje,obraz1);
    obraz1->nazwa_op="progowanie";
  }

  if(opcje.rozmywanie_poziome==1){
      rozmywanie_poziome(obraz1);
      obraz1->nazwa_op="rozmywanie poziome";
  }
    
  if(opcje.rozciaganie_histogramu==1){
       rozciaganie_histogramu(obraz1);
       obraz1->nazwa_op="rozciaganie histogramu";
  }
    

  if(opcje.konwersja==1){
     konwersja(obraz1);
     obraz1->nazwa_op="konwersja do szarosci";
   }

 
  if(opcje.filtracja_usredniajaca==1){
      filtracja_usredniajaca(obraz1);
      obraz1->nazwa_op="filtracja usredniajaca";
  }

  if(opcje.zapisz==1){
    zapisz(&opcje,obraz1);
  }

  if(opcje.wyswietl==1){
  wyswietl(obraz1);
  }

  if (opcje.zapisz!=1)
    fprintf(stderr,"Uwaga ! Zmiany nie zostaly zapisane, aby zapisac zmiane dodaj opcje -o <nazwa>\n");

  
 free(obraz1); /*zwolnienie miejsca po strukturze*/


/*Aby remove wykonalo sie po wykonaniu display (bez opoznienia programu display probuje wyswietlic plik testowy ktory juz nie istnieje) */

czas=clock();
while(clock()<=czas+500000) /*Uwaga, czas potrzebny na wykonanie display jest rozny na roznych systemach, gdy display wywala bledy nalezy zwiekszyc dodana liczba do czasu*/
{}
remove("Testowy"); /*usumiecie pliku testowego sluzacego do wyswietlania*/

return 0;

}

