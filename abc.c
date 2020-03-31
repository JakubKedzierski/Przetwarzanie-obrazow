#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
 int opcja;
 int kod_powrotu=0;
 char optstring[] = "-cvf-"; /* przetwarzane opcje */
int a = 10;
int tab[a][a+1];
 while ( (opcja = getopt(argc,argv,optstring)) != -1 )
 switch ( opcja ) {
 case 'c' :
 fprintf(stderr,"przetwarzam -c\n");
 break;
 case 'v' :
 fprintf(stderr,"przetwarzam -v\n");
 break;
 case 'f' :
 fprintf(stderr,"przetwarzam -f '%s'\n",optarg);
 break;
 case ':' : /* brakuje argumentu w opcji */
 fprintf(stderr,"opcja -%c wymaga argumentu\n",optopt);
 kod_powrotu = 1; /* Wystapil blad */
 break;
 case '?' :
 default :
 fprintf(stderr, "opcja -%c nie znana – ignoruje\n", optopt);
 kod_powrotu = 1; /* Wystapil blad */
 break;
 }
 printf("Pozostale argumenty:\n");
 for ( ; optind < argc; ++optind )
 printf("argv[%d] = '%s'\n",optind,argv[optind]);
 return kod_powrotu;
}
