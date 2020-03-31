
/********************************************************************/
/*                                                                  */
/* ALTERNATYWNA DO PRZEDSTAWIONEJ NA WYKLADZIE WERSJA OPRACOWANIA   */
/* PARAMETROW WYWOLANIA PROGRAMU UWZGLEDNIAJACA OPCJE Z PARAMETRAMI */
/* Z ODPOWIEDNIO ZAPROPONOWANYMI STRUKTURAMI DANYCH PRZEKAZUJACYMI  */
/* WCZYTANE USTAWIENIA                                              */
/*                                    COPYRIGHT (c) 2007-2013 ZPCiR */
/*                                                                  */
/* Autorzy udzielaja kazdemu prawa do kopiowania tego programu      */
/* w calosci lub czesci i wykorzystania go w dowolnym celu, pod     */
/* warunkiem zacytowania zrodla                                     */
/*                                                                  */
/********************************************************************/

#include <stdio.h>
#include <string.h>
#include "struct.h"
#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4

/*******************************************************/
/* Funkcja inicjuje strukture wskazywana przez wybor   */
/* PRE:                                                */
/*      poprawnie zainicjowany argument wybor (!=NULL) */
/* POST:                                               */
/*      "wyzerowana" struktura wybor wybranych opcji   */
/*******************************************************/

void wyzeruj_opcje(wopcje *wybor) {
  wybor->plik_we=NULL;
  wybor->plik_wy=NULL;
  wybor->negatyw=0;
  wybor->progowanie=0;
  wybor->wyswietl=0;
  wybor->rozmywanie_poziome=0;
  wybor->rozciaganie_histogramu=0;
  wybor->filtracja_usredniajaca=0;
  wybor->konwersja=0;
  wybor->zapisz=0;
  wybor->w_progu=0;
  wybor->czytaj=0;
}

/***************************************************************************************/
/* Funkcja rozpoznaje opcje wywolania programu zapisane w tablicy argv                 */
/* i zapisuje je w strukturze wybor                                                    */
/* Skladnia opcji wywolania programu                                                   */
/* program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-rx] [-d] [-h] [-f] [-m kolor]}    */
/* gdzie [-n negatyw] [-p <próg> progowanie] [-rx rozmywanie poziome]                */
/* [-h rozciąganie histogramu] [-s konwersja do szarości] [-d wyswietlanie]      */
/* [-f filtracja usredniajaca] [-i <nazwa> -nazwa pliku do wczytania]        */
/* [-o <nazwa_pliku> nazwa pliku do zapisu] [-ms konwersja do szarosci]   */ 
/*   Argumenty funkcji:                                                 */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu   */
/*         argv  -  tablica argumentow wywolania                        */
/*         wybor -  struktura z informacjami o wywolanych opcjach       */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich       */
/*      w strukturze wybór, do tego ustawia na 1 pola, ktore            */
/*	poprawnie wystapily w linii wywolania programu,                 */
/*	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;     */
/*	zwraca wartosc W_OK, gdy wywolanie bylo poprawne                */
/*	lub kod bledu zdefiniowany stalymi B_* (<0)                     */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/

int przetwarzaj_opcje(int argc, char **argv, wopcje *wybor) {
  int i, prog;
  char *nazwa_pliku_we, *nazwa_pliku_wy;
  char *m;

  wyzeruj_opcje(wybor); /*wyzerowanie opcji*/
  wybor->plik_wy=stdout; 
 
  for (i=1; i<argc; i++) {
    if (argv[i][0] != '-')  /* blad: to nie jest opcja - brak znaku "-" */
      return B_NIEPOPRAWNAOPCJA;

    switch (argv[i][1]) {
    case 'i': {                 /* opcja z nazwa pliku wejsciowego */
      if (++i<argc) {           /* wczytujemy kolejny argument jako nazwe pliku */
	nazwa_pliku_we=argv[i];
	if (strcmp(nazwa_pliku_we,"-")==0){ /* gdy nazwa jest "-"        */
	  wybor->plik_we=stdin;          /* ustwiamy wejscie na stdin */
	  wybor->czytaj=1;               /* "zapalenie flagi" */
	}else{  /*otwieramy wskazany plik */
	  wybor->plik_we=fopen(nazwa_pliku_we,"r");
	  wybor->czytaj=1;
	  if(wybor->plik_we==NULL){
	   fprintf(stderr,"Zle podane opcja wczytania lub brak podanego pliku w twoim katalogu\n");
	   return B_BRAKPLIKU;
	  }
	}
    }else 
	return B_BRAKNAZWY;                   /* blad: brak nazwy pliku lub bledny uchwyt */
      break;
    }
      
    case 'o': {                 /* opcja z nazwa pliku wyjsciowego */
      if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	nazwa_pliku_wy=argv[i];
	if (strcmp(nazwa_pliku_wy,"-")==0){/* gdy nazwa jest "-"         */
	  wybor->plik_wy=stdout;/* ustwiamy wyjscie na stdout */
	  wybor->zapisz=1;
	}else{                              /* otwieramy wskazany plik    */
	  wybor->plik_wy=fopen(nazwa_pliku_wy,"w");
	  wybor->zapisz=1;
	}
      }else 
	return B_BRAKNAZWY;                   /* blad: brak nazwy pliku */
      break;
    }
    case 'p': {
      if (++i<argc) { /* wczytujemy kolejny argument jako wartosc progu */
	if (sscanf(argv[i],"%d",&prog)==1) {
	  if((prog<=100)&&(prog>=0)){
	  wybor->progowanie=1;
	  wybor->w_progu=prog;
	  }else{
	    fprintf(stderr,"Zla wartosc progu procentowego/Mozliwa wartosc progu: 0-100[%%]\n");
	    return B_BRAKWARTOSCI;} 
	}else
	  return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
      } else 
	return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
      break;
    }
    case 'n': {                 /* mamy wykonac negatyw */
      wybor->negatyw=1;
      break;
    }

    case 'd': {                 /* mamy wyswietlic obraz */
      wybor->wyswietl=1;
      break;
    }

    case 'm': {
      if (argv[i][2]=='s'){ /*uwaga poprawne wywolanie -ms , bo -m s to niepoprawna opcja*/
         wybor->konwersja=1;
  } else {
	return B_BRAKWARTOSCI; /*niepoprawnie wybrana konwersja*/
  }
      break;
    }

    case 'r': {

      if (argv[i][2]!='x'){ /*uwaga poprawne wywolanie -rx , bo -r x to niepoprawna opcja*/
	return B_BRAKWARTOSCI; /*niepoprawnie wybrane rozmycie*/
       }else{ 
       wybor->rozmywanie_poziome=1;
      }
	  break;
    }
      
    case 'h' : {
      wybor->rozciaganie_histogramu=1;
      break;
    }
    case 'f' : {
      wybor->filtracja_usredniajaca=1;
      break;
    }
      
           default: {                    /* nierozpoznana opcja */
        return B_NIEPOPRAWNAOPCJA;}
    } /*koniec switch */
  } /* koniec for */

  if (wybor->plik_we!=NULL)     /* ok: wej. strumien danych zainicjowany */
    return W_OK;
  else 
    return B_BRAKPLIKU;         /* blad:  nie otwarto pliku wejsciowego  */
}

