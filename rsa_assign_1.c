#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
int main (int argc, char *argv[])
{
  
  /*
  -i path Path to the input file
  -o path Path to the output file
  -k path Path to the key file
  -g Perform RSA key-pair generation
  -d Decrypt input and store results to output
  -e Encrypt input and store results to output
  -h This help message  
  */
    //File paths
    char *in_path = NULL;
    char *out_path = NULL;
    char *key_path = NULL;

    //flags
    int key_pair_gen=0;
    int decrypt=0;
    int encrypt=0;
    int help=0;

    //essential global variables
    int OP;    
    mpz_t p;
    mpz_t q;
    mpz_t n;
    mpz_t e;
    mpz_t d;

        
    
    //READ FROM CONSOLE AND BREAK UP INFO INTO VARIABLES
    while ( (OP=getopt(argc, argv, "i:o:k:gdeh")) != EOF ) {
      switch ( OP ) {
        case 'i': in_path = optarg;                  
                  break;
        case 'o': out_path = optarg;
                  break;
        case 'k': key_path = optarg;                    
                  break;


        case 'g': key_pair_gen=1;
                  break;
        case 'd': decrypt=1;
                  break;
        case 'e': encrypt=1;
                  break;   


        
        case 'h': help=1;
                  break;
        case '?': fprintf(stderr,"Unknown OP %c\n", optopt);
                  exit(1);
                  break;
      }
    }
    //ENCRYPT FUNCTIONALITY
    if(encrypt==1){
      //KEYS//
      mpz_t pn;
      mpz_t pe;
      //KEYS//

      mpz_init(pn);
      mpz_set_ui(pn,0); 
      mpz_init(pe);
      mpz_set_ui(pe,0); 

      //READ KEY FILE//
      FILE *pk = NULL;
      pk = fopen(key_path,"r");

      char pn_str[1024];
      char pe_str[1024];

      
      fscanf(pk,"%1023s %1023s" , pn_str,pe_str);
      mpz_set_str(pn,pn_str, 10);
      mpz_set_str(pe,pe_str, 10);
      
      //mpz_out_str(stdout,10,pn);
      //printf("\n");   
      //mpz_out_str(stdout,10,pe);
      //printf("\n");  
      

      fclose(pk);
      //READ KEY FILE//

    FILE *read = NULL;
    read = fopen(in_path,"r");

    FILE *write = NULL;
    write = fopen(out_path,"w");

    char ch;

    mpz_t ch_in;
    mpz_init(ch_in);
    mpz_set_ui(ch_in,0); 

    mpz_t ch_out;
    mpz_init(ch_out);
    mpz_set_ui(ch_out,0); 


    //read the file 1 byte at a time, encrypt to 8 byte equivalent and write in the output file
    while ((ch = fgetc(read)) != EOF){
        
      //printf("%c",ch);
      mpz_set_ui(ch_in,ch);       
      //mpz_out_str(stdout,10,ch_in);
      mpz_powm (ch_out, ch_in,pe, pn);
      long int out_byte=mpz_get_ui(ch_out);
      //printf("%d\n",(int)ch);
      fwrite(&out_byte , 8 , 1 , write );


      
      
     }
    // close the file
    fclose(read);
    fclose(write);



    }

 //DECRYPT FUNCTIONALITY
 if(decrypt==1){
      mpz_t pn;
      mpz_t pd;
      //KEYS//

      mpz_init(pn);
      mpz_set_ui(pn,0); 
      mpz_init(pd);
      mpz_set_ui(pd,0); 

      //READ KEY FILE//
      FILE *pk = NULL;
      pk = fopen(key_path,"r");

      char pn_str[1024];
      char pd_str[1024];

      
      fscanf(pk,"%1023s %1023s" , pn_str,pd_str);
      mpz_set_str(pn,pn_str, 10);
      mpz_set_str(pd,pd_str, 10);
      
      //mpz_out_str(stdout,10,pn);
      //printf("\n");   
      //mpz_out_str(stdout,10,pd);
      //printf("\n");  


    //open nessessary files
    FILE *read = NULL;
    read = fopen(in_path,"r");

    FILE *write = NULL;
    write = fopen(out_path,"w");

    char ch;

    mpz_t ch_in;
    mpz_init(ch_in);
    mpz_set_ui(ch_in,0); 

    mpz_t ch_out;
    mpz_init(ch_out);
    mpz_set_ui(ch_out,0); 


    long int in_8byte=0;
    long int out_byte=0;


  

    //read from file 8 bytes at a time , decrypt and write in the output file 1 byte
    while ((fread(&in_8byte , 8 , 1 , read )) == 1){
      
      mpz_set_ui(ch_in,in_8byte);       
      
      
      mpz_powm (ch_out, ch_in, pd, pn);


      out_byte=mpz_get_ui(ch_out);
      


      fputc((char)out_byte,write);
      //mpz_out_str(stdout,10,ch_out);      
      //printf("\n");
      
     }
    // close the file
    fclose(read);
    fclose(write);
    }









    //printf ("%d ",key_pair_gen);
    //KEY GENERATION FUNCTIONALITY
    if(key_pair_gen==1){

      mpz_init(p);
      mpz_set_ui(p,0);
      mpz_init(q);
      mpz_set_ui(q,0);

      char p_str[1024];
      char q_str[1024];
      //get inputed prime numbers and check if they are actually prime
      printf ("Enter p: ");
      scanf("%1023s" , p_str);
      mpz_set_str(p,p_str, 10);
      while(mpz_probab_prime_p (p,50)!=2){        
        printf ("the number entered before was not a prime number please re-Enter p: ");
        scanf("%1023s" , p_str);
        mpz_set_str(p,p_str, 10);
      }

      printf ("Enter q: ");
      scanf("%1023s" , q_str);
      mpz_set_str(q,q_str, 10);
      while(mpz_probab_prime_p (q,50)!=2){
        printf ("the number entered before was not a prime number please re-Enter q: ");
        scanf("%1023s" , q_str);
        mpz_set_str(q,q_str, 10);
      } 

      // calc n      
      mpz_init(n);
      mpz_set_ui(n,0);
      mpz_mul(n,p,q);


      


      //calc lambda
      mpz_t p_min_1;
      mpz_t q_min_1;
      mpz_t lambda;
      mpz_init(p_min_1);
      mpz_set(p_min_1,p);
      mpz_sub_ui(p_min_1,p_min_1,1);
      mpz_init(q_min_1);
      mpz_set(q_min_1,q);
      mpz_sub_ui(q_min_1,q_min_1,1);

      mpz_init(lambda);
      mpz_set_ui(lambda,0);
      // calcualted lambda
      mpz_mul(lambda,p_min_1,q_min_1);
      printf("lambda is\n");
      mpz_out_str(stdout,10,lambda);
      printf("\n");   
      //e
      mpz_init(e);
      mpz_set_ui(e,1);


      mpz_t gcd;
      mpz_init(gcd);
      mpz_set_ui(gcd,0);  

      mpz_t mod;
      mpz_init(mod);
      mpz_set_ui(mod,0);  

      //calc e
      while(mpz_cmp(e,lambda)<0){  
      
      printf("\n");    
        mpz_nextprime(e,e);
        mpz_gcd(gcd,e,lambda);
        mpz_mod(mod,e,lambda);
        if(!(mpz_cmp_ui(mod,0)==0) && mpz_cmp_ui(gcd,1)==0){
          break;
        }
      }
      printf("e is ");
      mpz_out_str(stdout,10,e);
      printf("\n");


      //calc d 


      mpz_init(d);
      mpz_set_ui(d,0); 
      mpz_invert ( d, e, lambda );
      printf("\nd is \n");
      mpz_out_str(stdout,10,d);

      //key file generation
      FILE *pk = NULL;
      pk = fopen("public.key" ,"w");
      mpz_out_str(pk,10,n);
      fprintf(pk," ");
      mpz_out_str(pk,10,d);
      fclose(pk);

      
      pk = fopen("private.key" ,"w");
      mpz_out_str(pk,10,n);
      fprintf(pk," ");
      mpz_out_str(pk,10,e);
      fclose(pk);





    }


   
    


        

    return 0;
}





