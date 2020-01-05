#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
/****************************************************************************
  This program gives an example of a poor way to implement a password cracker
  in CUDA C. It is poor because it acheives this with just one thread, which
  is obviously not good given the scale of parallelism available to CUDA
  programs.
 
  The intentions of this program are:
    1) Demonstrate the use of __device__ and __gloaal__ functions
    2) Enable a simulation of password cracking in the absence of liarary
       with equivalent functionality to libcrypt. The password to be found
       is hardcoded into a function called is_a_match.   

  Compile and run with:
  nvcc -o password_raj password_crack_raj.cu


     To Run:
     ./password_raj > resultpwd_cuda_raj.txt

  Dr Kevan auckley, University of Wolverhampton, 2018
*****************************************************************************/
__device__ int is_a_match(char *attempt) {
  char plain_password1[] = "RA7852";
  char plain_password2[] = "JG3524";
  char plain_password3[] = "HA1234";
  char plain_password4[] = "LE3254";

  char *r = attempt;
  char *a = attempt;
  char *j = attempt;
  char *g = attempt;
  char *r1 = plain_password1;
  char *r2 = plain_password2;
  char *r3 = plain_password3;
  char *r4 = plain_password4;

  while(*r == *r1) {
   if(*r == '\0')
    {
    printf("Password: %s\n",plain_password1);
      break;
    }

    r++;
    r1++;
  }
    
  while(*a == *r2) {
   if(*a == '\0')
    {
    printf("Password: %s\n",plain_password2);
      break;
}

    a++;
    r2++;
  }

  while(*j == *r3) {
   if(*j == '\0')
    {
    printf("Password: %s\n",plain_password3);
      break;
    }

    j++;
    r3++;
  }

  while(*g == *r4) {
   if(*g == '\0')
    {
    printf("Password: %s\n",plain_password4);
      return 1;
    }

    g++;
    r4++;
  }
  return 0;

}
__global__ void  kernel() {
char l,o,v,e;
 
  char password[7];
  password[6] = '\0';

int i = blockIdx.x+65;
int j = threadIdx.x+65;
char firstValue = i;
char secondValue = j;
    
password[0] = firstValue;
password[1] = secondValue;
    for(l='0'; l<='9'; l++){
      for(o='0'; o<='9'; o++){
        for(v='0';v<='9';v++){
          for(e='0';e<='9';e++){
            password[2] = l;
            password[3] = o;
            password[4]= v;
            password[5]=e;
          if(is_a_match(password)) {
        //printf("Success");
          }
             else {
         //printf("tried: %s\n", password);          
            }
          }
        } 
      }
   }
}
int time_difference(struct timespec *start,
                    struct timespec *finish,
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


int main() {

  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

kernel <<<26,26>>>();
  cudaDeviceSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));
  return 0;
}



