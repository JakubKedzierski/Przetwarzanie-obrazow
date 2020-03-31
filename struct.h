#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

/*Struktury do przeksztalcen obrazow*/ 
typedef struct {
int wymx, wymy, szarosci;
  /* wymx,wymy wymiary obrazu, szarosci-w przypadku PPM -maksymalne nasycenie koloru, PGM - odcien szarosci */
  int *piksele;
  /* piksele - tablica w ktorej zapisane zostana piksele obrazu */
  int typ;
  /* typ - sluzy do okreslenia czy tablica ma byc:
  -trojwymiarowa- obrazy PPM (typ=1),
  -dwuwymiarowa- obrazy PGM (typ=3)  
  */
  int magic;
  /*numer magiczny*/
  int *pomoc;
  /* tablica pomocnicza przy operacjach na kilku pikselach jednoczesnie */
  const char *nazwa_op;
  /* nazwa operacji jaka wykonala sie na pliku*/
} dane;

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct {
  FILE *plik_we, *plik_wy;        /* uchwyty do pliku wej. i wyj. */
  int negatyw,progowanie,rozmywanie_poziome,rozciaganie_histogramu,filtracja_usredniajaca,konwersja,wyswietl,zapisz,czytaj;      /* opcje */
  int w_progu;              /* wartosc progu dla opcji progowanie */ 
} wopcje;
