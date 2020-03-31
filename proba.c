
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>




#define MAX 800            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */
#define prog_min 0          /*stale pomocne przy rozciaganiu histogramu*/
#define prog_max 100


typedef struct {
int wymx, wymy, szarosci;
int piksele[MAX][MAX];
} dane;



int czytaj(FILE *plik,dane *obraz_c) { /*funkcja odczytu*/
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;
  
  /*Sprawdzenie czy podano prawid³owy uchwyt pliku */
  if (plik==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
     return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien byæ P2 */
  if (fgets(buf,DL_LINII,plik)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
      return(0);
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik,"%d %d %d",&(obraz_c->wymx),&(obraz_c->wymy),&(obraz_c->szarosci))!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
     return(0);
  }
  /* Pobranie obrazu i zapisanie w tablicy obraz*/
  for (i=0;i<obraz_c->wymy;i++) {
    for (j=0;j<obraz_c->wymx;j++) {

      if (fscanf(plik,"%d",&(obraz_c->piksele[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
		return(0);
      }
    }
   
  }
   return (obraz_c->wymx)*(obraz_c->wymy);   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */


/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
int wyswietl(FILE *test,dane *obraz_w) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */
  int i,j;
  char n_pliku[100]="Testowy";


  fprintf(test,"P2\n%d %d\n %d\n",obraz_w->wymx,obraz_w->wymy,obraz_w->szarosci);
  for(i=0;i<obraz_w->wymy;i++){
    for(j=0;j<obraz_w->wymx;j++){
      fprintf(test,"%d  ",obraz_w->piksele[i][j]);
      
    }
  }
  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
 
  return 0;
}

int negatyw(dane *obraz_n) {
  int i,j;
  
  /*funkcja negatywu, operacje na kazdym elemencie tablicy*/
  for(i=0;i<obraz_n->wymy;i++){
    for(j=0;j<obraz_n->wymy;j++){
      obraz_n->piksele[i][j]=(obraz_n->szarosci-obraz_n->piksele[i][j]); /*od max szarosci odejmij wartosc punktu*/
    }
   }
  return 0;
}

int progowanie( dane *obraz_p){
  int i,j,prog;
  printf("\nWybierz procentowa wartosc progu(z zakresu 0-100 [%%])\nTwoj prog procentowy to:");
    scanf("%d",&prog);
       if((prog>prog_min)&&(prog<prog_max)){ /*gdy uzytkownik poda dobry prog*/

      for(i=0;i<obraz_p->wymy;i++){
	for(j=0;j<obraz_p->wymx;j++){
	  if(obraz_p->piksele[i][j]<=prog*(obraz_p->szarosci)/prog_max){ /*gdy szarosc w odpowiednim przedziale*/
	    obraz_p->piksele[i][j]=0;
	  }else
	    obraz_p->piksele[i][j]=obraz_p->szarosci;
	  
	}
      }
    }else{ /*gdy uzytkownik poda zly prog*/
	printf("Niepoprawny prog\n");
	}
      return 0;
}
	      
int rozmywanie_poziome(dane *obraz_r){
  int i,j,pomoc[MAX][MAX];
  
  for(i=0;i<obraz_r->wymy;i++){   
    for(j=0;j<obraz_r->wymx;j++){
      pomoc[i][j]=obraz_r->piksele[i][j]; /*tablica pomocnicza do funkcji-aby mozliwe bylo wykonanie operacji na kilku punktach jednoczesnie*/
    }
  }

  
  for(i=0;i<obraz_r->wymy;i++){   
    for(j=1;(j<obraz_r->wymx-1);j++){ /*j=1 i j<obraz_r->wymx-1 aby nie wychodzic poza zakres tablicy*/
    /*operacje na pojedycznym elemencie tablicy przy uzyciu dwoch sasiednich*/
    obraz_r->piksele[i][j]=(pomoc[i][j-1]+pomoc[i][j]+pomoc[i][j+1])/3;
  }
 }

  return 0;
}

int rozciaganie_histogramu( dane *obrazroz){
  int i=0,j=1,max,min,nowaszar;
 

  max=obrazroz->piksele[i][j-1];  /*max i min aby odpowiednio posortowac elementy obrazu*/
  min=obrazroz->piksele[i][j-1];

      for(i=0;i<obrazroz->wymy;i++){
	for(j=1;j<obrazroz->wymx;j++){
	  /*szukanie max wartosci szarosci punktu*/

       if(obrazroz->piksele[i][j]>max)  
	 max=obrazroz->piksele[i][j];   /*sortowanie max*/

       if(obrazroz->piksele[i][j]<min)
	 min=obrazroz->piksele[i][j];    /*sortowanie min*/
	}
      }
       if(max==obrazroz->szarosci){
	 printf("Twoj obraz jest juz maksymalnie rozciagniety\nZmien poziom szarosci.Podaj nowy poziom szarosci (od 100 do x [%%]):");
	 /*dla ulatwineia obslugi przez uzytkownika rozciaganie podajemy w %*/
	 scanf("%d",&nowaszar);

	 while(nowaszar<=100){
	
	   printf("Podales niepoprawna skale szarosci.Podaj nowy poziom szarosci (od 100 do x [%%]):\n");
	   scanf("%d",&nowaszar);
	 }
	 obrazroz->szarosci=(nowaszar*(obrazroz->szarosci))/100; /* nowaszar to % szarosci o jaki chcemy zwiekszyc*/
	 
       }

     

      for(i=0;i<obrazroz->wymy;i++){
	for(j=0;j<obrazroz->wymx;j++){
	  obrazroz->piksele[i][j]=((obrazroz->piksele[i][j]-min)*obrazroz->szarosci)/(max-min); /*rozciaganie histogramu*/
	}
      }
      return 0;
}
/*dodatkowa funckja maski*/
int filtracja_usredniajaca(dane *obraz_f){
  
  int tab[9],i=0,j=0,suma=0,max,min,pomoc[MAX][MAX];
  for(i=0;i<9;i++){
    tab[i]=1;
  }
  for(i=0;i<9;i++){
    suma=suma+tab[i];  /*zliczanie sumy (na potrzeby wzoru,wszystkie wspolczynniki dodatnie)*/
  }

    for(i=0;i<obraz_f->wymy;i++){   
    for(j=0;j<obraz_f->wymx;j++){
      pomoc[i][j]=obraz_f->piksele[i][j]; /*tablica pomocnicza do funkcji*/
    }
  }

 
  for(i=1;(i<(obraz_f->wymy-1));i++){
    for(j=1;(j<(obraz_f->wymx-1));j++){
      /*operacje wykonuje sie bez puntkow skrajnych obrazu*/
      
 obraz_f->piksele[i][j]=((tab[0]*pomoc[i-1][j-1])+(tab[1]*pomoc[i-1][j])+(tab[2]*pomoc[i-1][j+1])+tab[3]*pomoc[i][j-1]+tab[4]*pomoc[i][j]+tab[5]*pomoc[i][j+1]+tab[6]*pomoc[i+1][j-1]+tab[7]*pomoc[i+1][j]+tab[8]*pomoc[i+1][j+1]);
 obraz_f->piksele[i][j]=obraz_f->piksele[i][j]/suma;       
   
    }
  }
  

      return 0;
}

	  
int zapisz(FILE *plik,const char *nazwa_op,dane *obraz_za){
  int i,j;
  
  time_t czas; /*do zapisu w pliku obrazu czasu wykonania operacji*/
    time(&czas);
    /* w pliku znajduja sie dodatkowe informacje o wykonanej ostatniej operacji oraz czasie jej wykonania*/
    fprintf(plik,"P2\n#Data wykonania modyfikacji: %s#Wykonana operacja: %s\n %d %d\n %d\n",asctime(localtime(&czas)),nazwa_op,obraz_za->wymx,obraz_za->wymy,obraz_za->szarosci);
  for(i=0;i<obraz_za->wymy;i++){
    for(j=0;j<obraz_za->wymx;j++){
      fprintf(plik,"%d  ",obraz_za->piksele[i][j]);
      
    }
  }
  
	  return 0;
}
 int main() {
  
  int wybor;
  const char *nazwa_op; /*do zapisywania w pliku operacji jaka wykonano*/
  int odczytano = 0;
  FILE *plik;
  char nazwa[100]; /*nazwa pliku*/
  FILE *test; /*wskaznik do pliku testowego pomocnego przy wyswietlaniu*/
  char n_pliku[100]="Testowy"; /*nazwa pliku testowego*/
    dane *obraz1; /* albo dynamicznie */
    obraz1 = (dane *) malloc(sizeof(dane));  

    while(wybor!=8){
  
    printf("\n----------------------------------------\n");
    printf("Interfejs uzytkownika:\n");
    printf("1.Odczytaj plik\n2.Negatyw obrazu\n3.Progowanie\n4.Rozmywanie poziome\n5.Rozciaganie histogramu\n6.Zapisz\n7.Wyswietl obraz\n8.Wyjscie\nDodatkowe opcje:\n9.Filtracja usredniajaca\n");
    printf("Twoj wybor to:");
  scanf("%d",&wybor);
    printf("\n");
  
  switch(wybor){ 
    /* w poszczegolnych operacjach zakomendowano warunek gdy nie otwarto pliku*/
  case 1:

  /* Wczytanie zawartosci wskazanego pliku do pamieci */
  printf("Podaj nazwe pliku do odczytu:\n");
  scanf("%s",nazwa);
  plik=fopen(nazwa,"rw");

  if (plik != NULL) {       /* co spowoduje zakomentowanie tego warunku */

    czytaj(plik, obraz1);
   
    fclose(plik);
  }
  
  break;

  case 2:
    if(odczytano==0){
      printf("\nNie odczytano pliku !\n");
    }else{
      negatyw(obraz1);
      nazwa_op="negatyw";

    printf("Jesli chcesz zachowac zmiany zapisz plik!\n");
    }
      break;

  case 3:
    if(odczytano==0){
     printf("\nNie odczytano pliku !\n");
    }else{
    progowanie(obraz1);
    nazwa_op="progowanie";

    printf("Jesli chcesz zachowac zmiany zapisz plik!\n");
    }
 break;
    
  case 4:
    if(odczytano==0){
     printf("\nNie odczytano pliku !\n");
    }else{
      rozmywanie_poziome(obraz1);
     nazwa_op="rozmywanie poziome";

    printf("Jesli chcesz zachowac zmiany zapisz plik!\n");
    }
      break;

  case 5:
     if(odczytano==0){
     printf("\nNie odczytano pliku !\n");
     }else{
       rozciaganie_histogramu(obraz1);
       nazwa_op="rozciaganie histogramu";
 printf("Jesli chcesz zachowac zmiany zapisz plik!\n");
     }
    
       break;
       
  case 6:
    printf("Podaj nazwe pliku wynikowego:");
    scanf("%s",nazwa);
    plik=fopen(nazwa,"w");
    zapisz(plik,nazwa_op,obraz1);
    fclose(plik);
    break;

  case 7:
  /* Wyswietlenie poprawnie wczytanego obrazu zewnetrznym programem */
  
    if (odczytano != 0){
  test = fopen(n_pliku,"wra");
  wyswietl(test,obraz1);
  fclose(test);
  }
   
  break;

  case 8:
    /* zamkniecie programu wraz z usunieciem pliku testowego*/
    /* usuwanie pliku w funkcji wyswietl prowadzi do bledu programu(remove wykona sie szybciej niz display)*/
    remove("Testowy");
    return 0;
 
  case 9:
   /*dodatkowa funkcja maski*/
    if(odczytano==0){
     printf("\nNie odczytano pliku !\n");
     }else{
      filtracja_usredniajaca(obraz1);
      nazwa_op="filtracja usredniajaca";
 printf("Jesli chcesz zachowac zmiany zapisz plik!\n");
    }
    
    
    break;
    
  default:
    printf("Niepoprawny wybor");
    break;
  }
  }
    

    return odczytano;
}
