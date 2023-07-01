#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <gmp.h>

int main (int argc, char *argv[])
{
 
  /*
  -o path
  -p prime number
  -g primitive root
  -a private key a 
  -b private key b
  -h help
  */

    char *path = NULL;

    //prime num
    long long  P = 1;
    //primitive root
    long long  G = 1;
    //private key a
    long long  Priv_A= 1;
    //private key b
    long long  Priv_B = 1;

    //public keys
    long long  Public_A= 1;
    long long  Public_B = 1;

    //common secret key
    long long  Secret_A = 0;
    long long  Secret_B = 0;
    
    
    int OP;
    
    int help=0;
    //BREAK UP CONSOLE INPUT
    while ( (OP=getopt(argc, argv, "o:p:g:a:b:h")) != EOF ) {
      switch ( OP ) {
        case 'p': P = atoi(optarg);
                  break;
        case 'g': G = atoi(optarg);
                  break;
        case 'a': Priv_A = atoi(optarg);
                  break;  
        case 'b': Priv_B = atoi(optarg);
                  break;
        case 'o': path = optarg;
                  break;
        case 'h': help=1;
                  break;
        case '?': fprintf(stderr,"Unknown OP %c\n", optopt);
                  exit(1);
                  break;
      }
    }

    //initialise gmp library variables
    mpz_t mP;
    mpz_init(mP);
    mpz_set_ui(mP,P); 
    mpz_t mG;
    mpz_init(mG);
    mpz_set_ui(mG,G); 
    mpz_t mPriv_A;
    mpz_init(mPriv_A);
    mpz_set_ui(mPriv_A,Priv_A); 
    mpz_t mPriv_B;
    mpz_init(mPriv_B);
    mpz_set_ui(mPriv_B,Priv_B);
    mpz_t mPublic_A;
    mpz_init(mPublic_A);
    mpz_set_ui(mPublic_A,Public_A);
    mpz_t mPublic_B;
    mpz_init(mPublic_B);
    mpz_set_ui(mPublic_B,Public_B);
    mpz_t mSecret_A;
    mpz_init(mSecret_A);
    mpz_set_ui(mSecret_A,Secret_A);
    mpz_t mSecret_B;
    mpz_init(mSecret_B);
    mpz_set_ui(mSecret_B,Secret_B);

    //check if given prime is actually prime
    if(mpz_probab_prime_p (mP,50)!=2){
      printf("ERROR:prime number given is not prime\n");
      return 0;
    }
    
    if (P)    printf("prime number: %lld\n", P);
    if (G)    printf("primitive root: %lld\n", G);
    if (Priv_A)    printf("private key a : %lld\n", Priv_A);
    if (Priv_B)    printf("private key b: %lld\n", Priv_B);  
    if (help)    printf("---------------\n");

    //CREATE OUTPUT FILE AND CALCULATE KEYS
    FILE *fp = NULL;
    fp = fopen(path ,"w");
    if (path) printf("output txt generated with name: %s\n", path);


    
    //CALCULATE KEYS
    mpz_powm(mPublic_A,mG,mPriv_A,mP);
    mpz_powm(mPublic_B,mG,mPriv_B,mP);
   
    mpz_powm(mSecret_A,mPublic_B,mPriv_A,mP);
    mpz_powm(mSecret_B,mPublic_A,mPriv_B,mP);
    
    

    
    fprintf(fp,"<");
    mpz_out_str(fp,10,mPublic_A);
    fprintf(fp,">");
    fprintf(fp,",<");
    mpz_out_str(fp,10,mPublic_B);
    fprintf(fp,">,");

    //CHECK IF EVERYTHING IS CALCULATED CORRECTLY
    if(mpz_cmp(mSecret_A,mSecret_B)==0){
      //printf("they are the same\n");
      fprintf(fp,"<");
      mpz_out_str(fp,10,mSecret_A);
      fprintf(fp,">");
      //fprintf(fp,"<%lld>",Secret_A);
    }
    

    fclose(fp);

    


        

    return 0;
}

