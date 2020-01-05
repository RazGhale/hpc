#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>


/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o CrackAZZ99_raj_threeinitials CrackAZZ99.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./CrackAZZ99_raj_threeinitials > three_initials.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$NIF3htzuLRfY0cDgL3MeN6hqrPebxyrVDPgNpViJy5bZCXgYi/QIH4AZ2OmXAU.iZjg8KPhMy2LWtUUX/ln3x/",
  "$6$KB$L03HA9SY00w1B/aoNC.GRqULpwUshf8SkWvWZwEMqwUfB2UhuknQyrMq97pciSGd887H.fGaogjwNoV0stB261",
  "$6$KB$TXpskVbZOE8s3FTllkFly/pjXVvfjj2l/3ryJoa4b.wu.LvjsIUjBzbw91FflyMJ2QdEjv//yRRbP7ryiIR5L.",
  "$6$KB$HdhrJCMi7FHrI7trF2pAmLsS8n1kp/JXfjSV3mApB5.UuES.ULgzXXx0L9.4TlpjqU5WxfYimYs7jRs15VCoT1"
};


void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}


void crackAZ(char *salt_and_encrypted){
  int r, a, g, j;     
  char salt[7];    
  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);
  for(r='A'; r<='Z'; r++){
  for(a='A'; a<='Z'; a++){
    for(g='A'; g<='Z'; g++){
      for(j=0; j<=99; j++){
        sprintf(plain, "%c%c%c%02d", r, a, g, j); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
}
  printf("%d solutions explored\n", count);
}
int time_difference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char *argv[]){
  int i;
struct timespec start,finish;
long long int difference;
int account=0;
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  for(i=0;i<n_passwords;i<i++) {
    crackAZ(encrypted_passwords[i]);
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);
  printf("Time elapsed was %lldns\n",difference);

  return 0;
}
