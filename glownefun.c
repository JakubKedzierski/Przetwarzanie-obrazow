#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "struct.h"

/* Funkcje odczytu i zapisu */

int czytaj(wopcje *uchwyty,dane *obraz_c) { /*funkcja odczytu*/
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j,k;

  /* Sprawdzenie "numeru magicznego" - powinien byæ P* ( * - liczba 2 lub 3) */
  if (fgets(buf,DL_LINII,uchwyty->plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM ani PPM!\n");
    return(0);
  }
  
/*Sprawdzamy czy obraz jest typu PPM czy PGM*/
  if(buf[1]=='2'){
    obraz_c->typ=1;
    obraz_c->magic=2;
    /*zapisujemy do struktury jakiego typu jest obraz */
    /*jesli obraz PGM typ=1 (co pomoze nam przy deklaracji tablicy dla obrazow PGM) oraz numer magiczny (magic)=2*/
    /*jesli obraz PPM typ=3 (co pomoze nam przy deklaracji tablicy dla obrazow PPM) oraz numer magiczny (magic)=3*/
  }else if(buf[1]=='3'){
    obraz_c->typ=3;
    obraz_c->magic=3;
    /*w pozostalych przypadkach blad*/
  }else{
    fprintf(stderr,"Blad: To nie jest plik PGM ani PPM!\n");
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(uchwyty->plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,uchwyty->plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,uchwyty->plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(uchwyty->plik_we,"%d %d %d",&(obraz_c->wymx),&(obraz_c->wymy),&(obraz_c->szarosci))!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }
  
/*deklaracja dynamicznej tablicy*/
obraz_c->piksele = malloc(obraz_c->typ*obraz_c->wymx*obraz_c->wymy*sizeof(int));

/*Tablica wskaznikow dzieki ktorej mozna operowac na tablicy trojwymiarowej, sposob deklaracji tablicy wedlug metodologi na wykladzie*/
int (*piksele)[obraz_c->wymx][obraz_c->typ];
piksele=(int(*)[obraz_c->wymx][obraz_c->typ])obraz_c->piksele;
  
  
/* Pobranie obrazu i zapisanie w tablicy piksele*/
  for (i=0;i<obraz_c->wymy;i++) {
    for (j=0;j<obraz_c->wymx;j++) {
      for(k=0;k<obraz_c->typ;k++){

	if (fscanf(uchwyty->plik_we,"%d",&piksele[i][j][k])!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
	}
      }
    }
  }
  
  /*zakomentowanie warunku gdy niekotre piksele przekraczaja maksymalna mozliwa wartosc piksela*/  
   for (i=0;i<obraz_c->wymy;i++) {
    for (j=0;j<obraz_c->wymx;j++) {
      for(k=0;k<obraz_c->typ;k++){
	if(piksele[i][j][k]>obraz_c->szarosci){
	  fprintf(stderr,"Uwaga! Niektore piksele obrazu przekraczaja maksymalna mozliwa wartosc piksela\n");
	}
      }
    }
   }

return obraz_c->wymx*obraz_c->wymy*obraz_c->typ;
  /* Czytanie zakonczone sukcesem    */
  /* Zwroc liczbe wczytanych pikseli */
}

	  
int zapisz(wopcje *uchwyty,dane *obraz_za){
  int i,j,k;

/*Tablica wskaznikow dzieki ktorej mozna operowac na tablicy dwuwymiarowej*/  
int (*piksele)[obraz_za->wymx][obraz_za->typ]; 
piksele=(int(*)[obraz_za->wymx][obraz_za->typ]) obraz_za->piksele;

time_t czas; /*do zapisu w pliku obrazu czasu wykonania operacji*/
time(&czas);

/* w pliku znajduja sie dodatkowe informacje o wykonanej ostatniej operacji oraz czasie jej wykonania*/
fprintf(uchwyty->plik_wy,"P%d\n#Data wykonania modyfikacji: %s#Wykonana operacja: %s\n %d %d\n %d\n",obraz_za->magic,asctime(localtime(&czas)),obraz_za->nazwa_op,obraz_za->wymx,obraz_za->wymy,obraz_za->szarosci);
 
  for(i=0;i<obraz_za->wymy;i++){ 
    for(j=0;j<obraz_za->wymx;j++){
      for(k=0;k<obraz_za->typ;k++){
	fprintf(uchwyty->plik_wy,"%d  ",piksele[i][j][k]); /*zapisanie danych do pliku*/
      }
    }
  }

  fclose(uchwyty->plik_wy); 

  return 0;
}
