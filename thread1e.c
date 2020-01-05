#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <malloc.h>

/******************************************************************************


  Compile with:
    cc -o thread1e thread1e.c -lcrypt -lpthread

    ./thread1e > results.txt

******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$LmvMGhIR4PnYKlPXAOo2K4zt.BEjUN/9fKPMYT1zyisqo4WxY.FKfiCGz15GAoyNV5dW3GP.uQSiIaBQ3zxgo0",
  "$6$KB$ymSJbEU/YEcUQahIx54QuOhrEALiC1nZKDml63OTNJ5yPufkRaj4UpFrG.TuWJ9sMmaNU4KPTE3SnWbmAMseC0",
  "$6$KB$4gYD0T5W10w14A6KFHFrZ0aoEFpx50SFu.GmfbsYczg27nHOo8t6cij5/4Fvu9uqbKMOKswrwSmaONkHfEZTo1",
  "$6$KB$oM.i.qnwxu3X0K19Ok.OFDqqF4pmjOU1MhrOb5bKe1n2GjGuXY/w4ksOFlwEADNMFX6MEgR/7X7WarIx9G8Fz0"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}



void thread_crypt(char *encrypted_pwd){
  pthread_t t1,t2;

  void *kernel_function_1();
  void *kernel_function_2();

  pthread_create(&t1, NULL, kernel_function_1, (void *)encrypted_pwd);
  pthread_create(&t2, NULL, kernel_function_2, (void *)encrypted_pwd);
 
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
}


void *kernel_function_1(char *salt_and_encrypted){
  int r, a,j;     
  char salt[7];    
  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(r='A'; r<='M'; r++){
    for(a='A'; a<='Z'; a++){
      for(j=0; j<=99; j++){
        sprintf(plain, "%c%c%02d", r, a, j);
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
  printf("%d solutions explored\n", count);
}

void *kernel_function_2(char *salt_and_encrypted){
  int g, h, k;     
  char salt[7];    
  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(g='N'; g<='Z'; g++){
    for(h='A'; h<='Z'; h++){
      for(k=0; k<=99; k++){
        sprintf(plain, "%c%c%02d", g, h, k);
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
  struct timespec start, finish;  
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<n_passwords;i<i++) {
    thread_crypt(encrypted_passwords[i]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9));

  return 0;
}
