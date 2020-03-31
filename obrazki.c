#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define MAX 800            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */
#define prog_min 0          /*stale pomocne przy rozciaganiu histogramu*/
#define prog_max 100

int obraz[MAX][MAX]; /*tablica globalna do zapisu obrazu i przeksztalcen*/

int czytaj(FILE *plik,int *wymx,int *wymy, int *szarosci) { /*funkcja odczytu*/
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
  if (fscanf(plik,"%d %d %d",wymx,wymy,szarosci)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }
  /* Pobranie obrazu i zapisanie w tablicy obraz*/
  for (i=0;i<*wymy;i++) {
    for (j=0;j<*wymx;j++) {

      if (fscanf(plik,"%d",&(obraz[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
   
  }
  return *wymx**wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */


/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
int wyswietl(FILE *test,int *wymx,int *wymy,int *szarosci) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */
  int i,j;
  char n_pliku[100]="Testowy";
  
 fprintf(test,"P2\n%d %d\n %d\n",*wymx,*wymy,*szarosci);
  for(i=0;i<*wymy;i++){
    for(j=0;j<*wymx;j++){
      fprintf(test,"%d  ",obraz[i][j]);
      
    }
  }
  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
 
  return 0;
}

int negatyw(int *szarosci,int *wymx,int *wymy) {
  int i,j;
  /*funkcja negatywu, operacje na kazdym elemencie tablicy*/
  for(i=0;i<*wymy;i++){
    for(j=0;j<*wymx;j++){
      obraz[i][j]=(*szarosci-obraz[i][j]); /*od max szarosci odejmij wartosc punktu*/
    }
   }
  return 0;
}

int progowanie(int *wymx,int *wymy,int *szarosci){
  int i,j,prog;
  printf("\nWybierz procentowa wartosc progu(z zakresu 0-100 [%%])\nTwoj prog procentowy to:");
    scanf("%d",&prog);

    if((prog>prog_min)&&(prog<prog_max)){ /*gdy uzytkownik poda dobry prog*/

      for(i=0;i<*wymy;i++){
	for(j=0;j<*wymx;j++){
	  if(obraz[i][j]<=prog**szarosci/prog_max){ /*gdy szarosc w odpowiednim przedziale*/
	    obraz[i][j]=0;
	  }else
	    obraz[i][j]=*szarosci;
	  
	}
      }
    }else{ /*gdy uzytkownik poda zly prog*/
	printf("Niepoprawny prog\n");
	}
      return 0;
}
	      
int rozmywanie_poziome(int *wymx,int *wymy){
  int i,j,pomoc[MAX][MAX];

  for(i=0;i<*wymy;i++){   
    for(j=0;j<*wymx;j++){
      pomoc[i][j]=obraz[i][j]; /*tablica pomocnicza do funkcji-aby mozliwe bylo wykonanie operacji na kilku punktach jednoczesnie*/
    }
  }

  
  for(i=0;i<*wymy;i++){   
    for(j=1;(j<*wymx-1);j++){ /*j=1 i j<*wymx-1 aby nie wychodzic poza zakres tablicy*/
    /*operacje na pojedycznym elemencie tablicy przy uzyciu dwoch sasiednich*/
    obraz[i][j]=(pomoc[i][j-1]+pomoc[i][j]+pomoc[i][j+1])/3;
  }
 }
 return 0;
}

int rozciaganie_histogramu(int *wymx,int *wymy,int *szarosci){
  int i=0,j=1,max,min,nowaszar;


  max=obraz[i][j-1];  /*max i min aby odpowiednio posortowac elementy obrazu*/
  min=obraz[i][j-1];

      for(i=0;i<*wymy;i++){
	for(j=1;j<*wymx;j++){
	  /*szukanie max wartosci szarosci punktu*/
       if(obraz[i][j]>max)  
	 max=obraz[i][j];   /*sortowanie max*/
       if(obraz[i][j]<min)
	 min=obraz[i][j];    /*sortowanie min*/
	}
      }
       if(max==*szarosci){
	 printf("Twoj obraz jest juz maksymalnie rozciagniety\nZmien poziom szarosci.Podaj nowy poziom szarosci (od 100 do x [%%]):");
	 /*dla ulatwineia obslugi przez uzytkownika rozciaganie podajemy w %*/
	 scanf("%d",&nowaszar);

	 while(nowaszar<=100){
	
	   printf("Podales niepoprawna skale szarosci.Podaj nowy poziom szarosci (od 100 do x [%%]):\n");
	   scanf("%d",&nowaszar);
	 }
	 *szarosci=(nowaszar**szarosci)/100; /* nowaszar to % szarosci o jaki chcemy zwiekszyc*/
	 
       }

     

      for(i=0;i<*wymy;i++){
	for(j=0;j<*wymx;j++){
	  obraz[i][j]=((obraz[i][j]-min)**szarosci)/(max-min); /*rozciaganie histogramu*/
	}
      }
      return 0;
}
/*dodatkowa funckja maski*/
int filtracja_usredniajaca(int *wymx,int *wymy,int *szarosci){
  int tab[9],i=0,j=0,suma=0,max,min,pomoc[MAX][MAX];
  for(i=0;i<9;i++){
    tab[i]=1;
  }
  for(i=0;i<9;i++){
    suma=suma+tab[i];  /*zliczanie sumy (na potrzeby wzoru,wszystkie wspolczynniki dodatnie)*/
  }

    for(i=0;i<*wymy;i++){   
    for(j=0;j<*wymx;j++){
      pomoc[i][j]=obraz[i][j]; /*tablica pomocnicza do funkcji*/
    }
  }

 
  for(i=1;(i<(*wymy-1));i++){
    for(j=1;(j<(*wymx-1));j++){
      /*operacje wykonuje sie bez puntkow skrajnych obrazu*/
      
 obraz[i][j]=((tab[0]*pomoc[i-1][j-1])+(tab[1]*pomoc[i-1][j])+(tab[2]*pomoc[i-1][j+1])+tab[3]*pomoc[i][j-1]+tab[4]*pomoc[i][j]+tab[5]*pomoc[i][j+1]+tab[6]*pomoc[i+1][j-1]+tab[7]*pomoc[i+1][j]+tab[8]*pomoc[i+1][j+1]);
 obraz[i][j]=obraz[i][j]/suma;       
   
    }
  }
  

      return 0;
}

	  
int zapisz(FILE *plik,int *wymx,int *wymy,int *szarosci,const char *nazwa_op){
  int i,j;
  
  time_t czas; /*do zapisu w pliku obrazu czasu wykonania operacji*/
    time(&czas);
    /* w pliku znajduja sie dodatkowe informacje o wykonanej ostatniej operacji oraz czasie jej wykonania*/
    fprintf(plik,"P2\n#Data wykonania modyfikacji: %s#Wykonana operacja: %s\n %d %d\n %d\n",asctime(localtime(&czas)),nazwa_op,*wymx,*wymy,*szarosci);
  for(i=0;i<*wymy;i++){
    for(j=0;j<*wymx;j++){
      fprintf(plik,"%d  ",obraz[i][j]);
      
    }
  }
  
	  return 0;
}

int main() {
  int obraz[MAX][MAX] ;
  int wymx,wymy,szarosci,wybor=0;
  const char *nazwa_op; /*do zapisywania w pliku operacji jaka wykonano*/
  int odczytano = 0;
  FILE *plik;
  char nazwa[100]; /*nazwa pliku*/
  FILE *test; /*wskaznik do pliku testowego pomocnego przy wyswietlaniu*/
  char n_pliku[100]="Testowy"; /*nazwa pliku testowego*/
  while(wybor!=8){
  
printf("\n----------------------------------------\n Interfejs uzytkownika:\n1.Odczytaj plik\n2.Negatyw obrazu\n3.Progowanie\n4.Rozmywanie poziome\n5.Rozciaganie histogramu\n6.Zapisz\n7.Wyswietl obraz\n8.Wyjscie\nDodatkowe opcje:\n9.Filtracja usredniajaca\nTwoj wybor to:");
  scanf("%d",&wybor);
  printf("\n");
  
  switch(wybor){ 
    /* w poszczegolnych operacjach zakomendowano warunek gdy nie otwarto pliku*/
  case 1:

  /* Wczytanie zawartosci wskazanego pliku do pamieci */
  printf("Podaj nazwe pliku do odczytu:\n");
  scanf("%s",nazwa);
  plik=fopen(nazwa,"r");

  if (plik != NULL) {       /* co spowoduje zakomentowanie tego warunku */
    odczytano = czytaj(plik,&wymx,&wymy,&szarosci);
    fclose(plik);
  }
  
  break;

  case 2:
    if(odczytano==0){
      printf("\nNie odczytano pliku !\n");
    }else{
      negatyw(&szarosci,&wymx,&wymy);
      nazwa_op="negatyw";

    printf("Jesli chcesz zachowac zmiany zapisz plik!\n");
    }
      break;

  case 3:
    if(odczytano==0){
     printf("\nNie odczytano pliku !\n");
    }else{
    progowanie(&wymx,&wymy,&szarosci);
    nazwa_op="progowanie";

    printf("Jesli chcesz zachowac zmiany zapisz plik!\n");
    }
 break;
    
  case 4:
    if(odczytano==0){
     printf("\nNie odczytano pliku !\n");
    }else{
      rozmywanie_poziome(&wymx,&wymy);
     nazwa_op="rozmywanie poziome";

    printf("Jesli chcesz zachowac zmiany zapisz plik!\n");
    }
      break;

  case 5:
     if(odczytano==0){
     printf("\nNie odczytano pliku !\n");
     }else{
       rozciaganie_histogramu(&wymx,&wymy,&szarosci);
       nazwa_op="rozciaganie histogramu";
 printf("Jesli chcesz zachowac zmiany zapisz plik!\n");
     }
    
       break;
       
  case 6:
    printf("Podaj nazwe pliku wynikowego:");
    scanf("%s",nazwa);
    plik=fopen(nazwa,"w");
    zapisz(plik,&wymx,&wymy,&szarosci,nazwa_op);
    fclose(plik);
    break;

  case 7:
  /* Wyswietlenie poprawnie wczytanego obrazu zewnetrznym programem */
  
    if (odczytano != 0){
  test = fopen(n_pliku,"wra");
  wyswietl(test,&wymx,&wymy,&szarosci);
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
      filtracja_usredniajaca(&wymx,&wymy,&szarosci);
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

/* Na poczatku pojawil sie problem zamiany wymy z wymx, ktory rozwiazano poprzez zamiane arguemntow i obecnie program dziala dla dowolnych obrazow PMG (nie tylko kwadratowych). 
Dodano dodatkowa funkcje filtracji usredniajacej (maska), zapisu w pliku daty wykonania modyfikacji oraz nazwy operacji jaka przeporwadzono. 
Wykonano kilka testow sprawdzajac komenda cat zawartosc pliku z obrazem po zastosowaniu poszczegolnych operacji. Sprawdzono poprawnosc na roznych obrazach
(przyklady obrazow pobranych z internetu). Aby zobaczyc wynik operacji przed wyswietleniem nalezy najpierw zapisac poniewaz funkcja display operuje na pliku (nie na tablicy w programie).
Program powstal na bazie programu udostepninonego na stronie PProg odczyt.c. Program wykonal:Jakub Kedzierski*/
