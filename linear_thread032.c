#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
/******************************************************************************

 * To compile:
 *   cc -o linear_thread linear_thread032.c -lm -pthread
 *
 * To run:
 *   ./linear_thread
 *

 *****************************************************************************/

double bm = 1.3;
double bc = 10;
double be;
double dm[8];
double dc[8];
double e[8];
double step = 0.01;
double best_error = 999999999;
int best_error_i;
int minimum_found = 0;
double om[] = {0,1,1, 1, 0,-1,-1,-1};
double oc[] = {1,1,0,-1,-1,-1, 0, 1};


typedef struct point_t {
  double s;
  double p;
} point_t;

int n_data = 1000;
point_t data[];

double residual_error(double s, double p, double m, double c) {
  double e = (m * s) + c - p;
  return e * e;
}

double rms_error(double m, double c) {
  int i;
  double mean;
  double error_sum = 0;
 
  for(i=0; i<n_data; i++) {
    error_sum += residual_error(data[i].s, data[i].p, m, c); 
  }
 
  mean = error_sum / n_data;
 
  return sqrt(mean);
}

void *linear_regression_thread(void *args){

  int *a = args;
  int i = *a;
 // printf("\n i in thread fun=%d", i);
  dm[i] = bm + (om[i] * step);
      dc[i] = bc + (oc[i] * step);

  e[i] = rms_error(dm[i], dc[i]);
  if(e[i] < best_error) {
  best_error = e[i];
  best_error_i = i;
  pthread_exit(NULL);
  }
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

int main() {
  int i;
  pthread_t p_threads[8];
 struct timespec start, finish;  
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
  be = rms_error(bm, bc);

  while(!minimum_found) {
    for(i=0;i<8;i++) {
      pthread_create(&p_threads[i], NULL, (void*)linear_regression_thread, &i);
      pthread_join(p_threads[i], NULL);
    }

  //  printf("best m,c is %lf,%lf with error %lf in direction %d\n",
    //  dm[best_error_i], dc[best_error_i], best_error, best_error_i);

    if(best_error < be) {
      be = best_error;
      bm = dm[best_error_i];
      bc = dc[best_error_i];
    } else {
      minimum_found = 1;
    }
  }
  printf("minimum m,c is %lf,%lf with error %lf\n", bm, bc, be);
clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9));
  pthread_exit(NULL);
  return 0;
}

point_t data[] = {
  {69.09,102.61},{82.13,152.08},{73.94,133.41},{67.94,130.43},
  {83.26,134.77},{83.05,140.13},{79.17,149.94},{78.38,151.22},
  {83.10,152.31},{73.50,140.04},{75.41,129.24},{87.18,142.30},
  {65.12,132.63},{49.91,98.86},{ 5.65,45.64},{67.40,142.60},
  { 4.96,40.62},{45.49,89.81},{50.42,104.86},{89.77,164.46},
  {18.97,51.92},{68.36,128.98},{ 5.51,20.69},{23.82,73.75},
  {66.69,139.56},{93.58,169.37},{66.54,128.90},{13.22,50.82},
  {46.69,84.63},{60.31,120.24},{61.09,105.13},{40.11,97.65},
  {49.86,96.39},{53.57,110.10},{ 6.19,47.58},{12.38,64.19},
  {78.14,141.00},{27.83,61.09},{41.00,76.48},{61.04,120.42},
  {24.78,68.13},{52.50,107.45},{14.94,69.36},{35.97,79.09},
  {40.30,73.31},{92.79,182.68},{ 2.26,38.15},{33.80,70.49},
  {65.87,121.84},{45.00,78.95},{83.62,159.92},{35.90,85.48},
  { 6.00,51.97},{59.55,110.64},{18.14,49.26},{46.19,75.07},
  {42.78,103.67},{21.68,66.82},{64.63,146.59},{96.75,168.65},
  {20.19,44.40},{66.85,143.42},{61.71,116.91},{74.56,133.91},
  {98.14,166.53},{91.82,154.60},{ 6.83,27.37},{91.03,163.50},
  {41.90,92.72},{65.35,121.86},{59.70,116.10},{70.07,122.91},
  {97.62,173.25},{21.01,55.66},{17.16,47.12},{71.91,131.35},
  { 0.31,17.13},{33.27,56.88},{69.57,128.78},{66.31,121.39},
  {81.24,144.19},{90.57,176.58},{77.25,142.55},{94.31,162.47},
  {42.88,84.68},{64.35,111.81},{ 2.18,26.05},{84.89,139.55},
  { 3.69,42.18},{46.86,111.42},{91.62,162.75},{48.86,103.04},
  {11.69,49.69},{94.38,180.77},{35.87,68.86},{13.57,53.53},
  {37.92,93.43},{53.11,100.27},{ 9.44,56.21},{66.37,119.22},
  {96.89,143.15},{79.79,151.46},{29.43,104.35},{97.53,166.99},
  {18.39,75.76},{41.46,88.34},{28.20,71.24},{15.77,48.03},
  {23.44,72.10},{79.20,145.25},{ 6.38,37.64},{20.94,61.35},
  {31.44,73.70},{58.22,111.84},{35.73,93.21},{54.87,115.86},
  {92.84,161.21},{57.35,111.33},{48.13,104.70},{39.15,84.25},
  {76.73,131.17},{33.16,86.42},{79.30,152.22},{32.79,85.88},
  {61.08,124.63},{33.65,86.36},{30.30,67.58},{25.36,55.42},
  {60.60,132.22},{54.55,107.11},{61.55,120.12},{21.43,51.10},
  {55.28,114.45},{69.39,146.15},{71.36,136.26},{78.32,158.06},
  {53.78,104.73},{41.16,88.31},{58.21,105.38},{22.62,48.68},
  {57.70,107.41},{87.20,160.88},{53.56,112.60},{45.79,103.69},
  {87.23,172.35},{26.91,66.80},{90.24,146.45},{10.52,51.41},
  {88.18,177.14},{89.88,146.67},{ 3.40,44.19},{63.10,118.63},
  {72.20,127.43},{ 7.21,44.09},{68.95,116.40},{93.06,141.28},
  { 1.78,47.44},{21.78,58.19},{95.38,167.88},{26.07,69.08},
  {82.38,144.48},{44.78,87.81},{ 5.45,47.63},{14.61,46.51},
  {47.12,97.36},{91.40,146.00},{ 3.92,31.27},{30.83,54.33},
  {51.07,118.44},{11.70,45.32},{37.06,76.74},{93.60,154.41},
  {88.67,158.77},{58.76,102.75},{18.18,71.37},{61.55,96.09},
  {64.81,126.94},{38.89,80.15},{40.33,89.31},{ 6.98,42.89},
  {26.84,77.22},{ 1.48,38.03},{69.64,125.26},{60.44,124.70},
  {44.32,103.70},{23.96,61.48},{41.32,66.25},{76.06,136.13},
  {74.79,139.10},{48.14,86.10},{90.45,175.78},{87.86,133.98},
  {92.55,170.68},{58.06,111.65},{79.93,134.77},{61.20,126.32},
  {99.31,173.71},{47.55,96.83},{ 5.95,58.93},{54.41,117.09},
  {49.62,95.15},{20.48,52.60},{26.69,64.39},{77.38,138.43},
  {21.50,61.58},{37.20,100.88},{26.12,60.30},{45.04,101.03},
  {12.08,59.04},{13.59,46.57},{97.04,166.44},{58.34,109.75},
  { 4.47,41.91},{ 4.82,26.79},{49.25,120.66},{47.96,92.87},
  {94.68,160.36},{38.52,78.12},{18.86,67.64},{65.01,128.12},
  {94.62,168.74},{46.49,100.40},{18.67,59.96},{61.25,126.12},
  {26.30,63.87},{16.32,75.77},{47.98,83.09},{97.67,167.17},
  {90.04,155.34},{61.05,119.81},{34.12,79.69},{36.37,81.55},
  {67.37,147.11},{94.03,166.73},{27.01,54.22},{14.88,38.59},
  {53.43,101.46},{44.85,102.66},{96.98,172.50},{29.25,65.32},
  {20.28,82.30},{82.38,148.74},{47.99,95.71},{54.39,113.38},
  {24.78,60.98},{46.40,112.11},{83.11,154.36},{78.56,142.47},
  {52.44,120.30},{85.45,139.05},{18.44,55.81},{33.82,91.05},
  {81.25,148.47},{28.27,71.42},{28.72,81.91},{43.69,113.37},
  {95.79,176.73},{20.00,66.36},{ 1.90,51.15},{69.08,102.50},
  {66.37,126.22},{75.30,137.17},{50.89,107.35},{26.57,71.46},
  {96.71,173.85},{61.50,136.37},{24.52,55.79},{ 3.31,25.80},
  {58.29,121.62},{38.72,73.27},{58.48,109.58},{79.94,143.31},
  {45.13,95.27},{14.01,41.17},{63.65,117.17},{73.39,150.88},
  {57.61,118.65},{68.16,125.50},{78.05,134.05},{84.45,140.93},
  {64.23,127.52},{51.91,105.59},{52.26,103.94},{48.43,74.06},
  {45.19,95.99},{16.62,66.21},{34.10,70.76},{46.25,96.86},
  {65.54,128.94},{73.65,134.18},{87.08,161.45},{45.86,101.31},
  {99.65,155.49},{62.47,116.88},{60.77,123.30},{10.00,73.95},
  {69.31,138.95},{99.81,190.04},{18.80,61.57},{11.29,37.86},
  {32.22,61.46},{83.93,165.72},{23.79,61.15},{61.98,134.84},
  {36.38,81.78},{91.53,162.22},{17.14,42.61},{31.77,81.24},
  {41.24,88.26},{53.74,120.40},{ 7.59,46.01},{69.20,145.90},
  {31.57,76.95},{41.83,103.94},{76.31,145.60},{34.57,86.48},
  {78.44,140.01},{35.91,83.76},{67.19,122.40},{28.51,69.92},
  {41.12,109.23},{38.78,89.35},{87.32,160.07},{81.17,141.79},
  {27.93,80.99},{43.71,85.72},{35.55,76.40},{48.41,96.15},
  {35.86,93.18},{58.45,103.95},{32.51,76.14},{22.98,47.63},
  {84.80,150.30},{19.37,67.26},{62.21,124.12},{72.56,125.75},
  {12.62,44.66},{84.63,150.02},{35.79,77.18},{91.59,167.42},
  {56.79,135.96},{60.75,128.75},{75.17,132.66},{50.03,97.47},
  {22.77,81.91},{79.14,140.88},{30.32,89.46},{64.27,117.96},
  {14.61,72.92},{13.17,65.89},{73.53,123.85},{77.44,118.69},
  {56.04,107.10},{46.18,92.11},{54.48,94.07},{73.79,151.52},
  { 8.83,39.47},{ 3.58,40.64},{33.35,83.41},{63.51,127.53},
  {63.18,134.55},{38.65,77.96},{81.77,143.31},{28.99,71.78},
  {46.57,107.91},{31.88,61.51},{69.75,121.75},{39.03,92.38},
  {78.95,138.81},{33.03,62.89},{93.32,160.29},{51.73,96.94},
  {99.52,178.80},{39.66,81.62},{20.81,63.06},{54.52,101.68},
  {13.88,77.63},{11.54,47.90},{57.06,111.62},{34.85,73.47},
  { 1.03,38.38},{99.98,147.72},{15.65,37.52},{93.98,171.57},
  {26.00,61.54},{26.34,97.49},{44.45,89.46},{30.19,85.94},
  {65.17,121.23},{96.37,174.62},{17.59,57.65},{49.21,85.15},
  {97.14,164.23},{69.46,128.85},{52.29,116.47},{59.90,97.23},
  {41.62,96.65},{46.43,96.75},{64.79,120.04},{16.39,42.84},
  {96.74,160.51},{15.06,67.38},{12.89,32.01},{59.36,96.22},
  {36.49,85.66},{ 6.12,33.73},{87.56,144.59},{58.04,116.48},
  {45.12,109.10},{90.29,155.38},{15.50,58.50},{ 8.17,39.33},
  {36.71,87.05},{95.02,175.91},{56.45,102.69},{29.55,76.64},
  {81.35,144.65},{51.25,106.51},{47.80,101.57},{39.43,89.38},
  {16.54,75.06},{18.23,39.97},{38.47,84.60},{72.95,137.89},
  {20.95,63.99},{53.89,104.26},{62.01,111.63},{77.09,132.07},
  {75.62,131.00},{82.52,143.40},{29.78,67.90},{24.28,65.09},
  {60.18,116.00},{64.87,116.99},{66.81,118.50},{97.63,173.78},
  {82.52,145.14},{17.04,56.60},{69.23,128.14},{11.14,35.51},
  {44.81,75.59},{79.91,130.95},{73.98,123.15},{30.70,68.34},
  {16.56,69.96},{44.54,111.55},{42.63,98.01},{ 8.33,52.94},
  {23.70,81.71},{72.51,126.68},{51.65,91.93},{18.86,52.82},
  {51.40,105.16},{33.69,78.93},{48.25,95.34},{95.60,165.19},
  {70.55,135.12},{ 7.54,25.40},{84.10,147.82},{ 5.46,55.62},
  {51.25,105.36},{33.31,70.29},{91.09,155.95},{93.79,150.87},
  {39.64,105.11},{23.94,70.09},{ 2.79,57.46},{93.12,157.26},
  {82.66,153.35},{20.67,50.18},{25.48,55.36},{73.93,125.80},
  {82.75,164.55},{66.85,115.72},{ 6.63,57.24},{56.61,128.98},
  {73.65,146.00},{28.61,76.79},{80.74,154.19},{42.26,82.19},
  {48.99,96.21},{29.42,74.92},{47.75,96.08},{30.98,74.08},
  {21.47,58.18},{40.25,84.61},{71.79,134.58},{62.21,100.02},
  { 6.50,44.15},{23.18,54.16},{99.44,173.73},{34.55,74.88},
  {44.42,90.87},{94.40,160.68},{44.12,83.50},{38.81,83.14},
  {17.89,48.98},{41.90,89.86},{53.95,124.31},{34.65,77.79},
  {99.46,188.27},{45.96,96.10},{56.07,108.36},{52.48,94.99},
  {51.23,105.75},{13.28,48.42},{81.65,142.71},{27.37,57.49},
  {47.88,98.01},{45.77,83.07},{67.87,113.75},{ 6.56,20.13},
  {57.51,99.93},{45.05,98.83},{86.15,154.73},{32.01,66.76},
  {10.76,37.49},{15.01,59.42},{56.76,111.61},{34.09,78.70},
  {88.70,179.11},{50.88,120.11},{22.56,51.27},{77.01,143.66},
  {31.56,69.66},{45.50,100.94},{64.12,114.38},{86.51,162.80},
  {91.85,153.99},{89.59,139.43},{62.84,120.04},{ 1.00,45.36},
  {76.04,134.88},{ 2.31,39.43},{ 4.08,36.49},{50.56,91.92},
  {80.18,165.25},{84.88,150.80},{85.96,154.04},{58.01,121.88},
  {83.62,142.62},{78.78,133.92},{24.57,47.24},{10.38,51.07},
  {70.57,129.78},{ 6.24,63.42},{95.52,158.82},{47.53,99.59},
  {37.80,86.23},{57.04,125.93},{98.17,189.61},{18.58,56.15},
  {43.64,95.36},{97.10,155.81},{41.32,104.04},{58.80,110.09},
  {46.92,109.08},{55.66,115.05},{43.44,91.67},{ 1.16,18.39},
  {44.90,79.00},{88.95,146.71},{77.11,140.55},{38.22,70.52},
  {33.63,94.80},{82.24,147.90},{32.39,76.09},{68.71,137.44},
  {61.91,139.84},{ 2.72,31.93},{31.01,83.44},{43.82,82.06},
  {90.41,138.37},{74.29,146.66},{34.46,82.13},{27.66,70.23},
  { 0.75,35.40},{70.47,122.31},{38.41,74.27},{59.11,111.32},
  {94.20,145.15},{61.79,112.71},{25.69,72.79},{17.38,73.54},
  { 2.57,27.14},{40.88,85.46},{59.63,106.14},{23.24,43.26},
  {24.06,73.14},{ 3.38,46.52},{24.26,58.40},{93.00,155.20},
  {48.69,81.21},{32.00,80.68},{67.43,124.66},{76.88,118.26},
  {97.26,169.42},{52.05,86.99},{26.68,80.02},{88.80,164.27},
  {30.79,92.40},{13.76,46.00},{ 4.39,20.46},{47.45,110.48},
  {22.42,68.09},{39.34,105.98},{90.58,167.10},{45.22,80.64},
  { 2.29,24.47},{51.00,101.65},{16.63,52.64},{43.24,91.93},
  {33.47,77.44},{69.44,121.59},{93.63,154.70},{97.34,176.12},
  {82.83,145.39},{84.31,123.04},{51.96,110.56},{13.79,49.08},
  {85.09,147.33},{19.50,64.58},{25.47,81.74},{51.68,106.78},
  {27.99,75.05},{ 2.78,60.03},{93.13,166.31},{27.66,83.80},
  {43.98,86.41},{ 6.34,27.97},{41.25,74.57},{10.49,37.94},
  {94.29,159.02},{33.41,82.45},{19.80,64.16},{18.35,65.02},
  {93.57,182.68},{ 7.90,51.96},{85.56,157.84},{50.76,88.32},
  {65.70,121.81},{72.32,151.58},{23.45,66.39},{ 4.44,33.36},
  {58.98,115.21},{12.18,46.07},{30.66,60.74},{69.63,124.96},
  { 2.69,37.64},{16.96,57.25},{60.58,126.99},{73.60,135.37},
  {11.98,68.00},{ 0.42,38.89},{47.45,90.83},{44.59,89.17},
  { 3.95,27.62},{78.31,145.77},{81.91,153.36},{47.00,109.88},
  { 3.10,31.96},{59.53,105.57},{14.67,55.59},{30.44,90.14},
  {64.61,123.88},{40.33,96.71},{67.44,133.61},{42.29,68.72},
  {44.38,90.29},{65.86,136.25},{91.75,157.96},{24.71,74.32},
  {89.50,153.96},{95.40,160.30},{ 2.51,32.12},{51.63,109.59},
  {41.35,101.13},{19.94,59.70},{65.45,132.43},{38.21,117.97},
  {40.69,84.45},{50.78,126.50},{47.42,103.31},{ 6.98,47.56},
  {95.49,162.77},{62.64,129.54},{45.89,106.75},{71.43,126.03},
  {95.26,169.84},{81.30,128.51},{16.16,48.62},{ 8.97,75.32},
  {28.86,95.51},{10.27,41.50},{78.84,153.22},{83.90,173.33},
  {51.33,105.65},{36.43,88.93},{70.23,131.60},{23.96,67.38},
  {45.21,97.59},{76.50,118.70},{63.22,105.01},{33.75,102.37},
  {72.57,140.64},{18.16,68.62},{27.39,71.28},{ 3.22,51.94},
  {89.94,151.77},{23.53,59.07},{18.69,44.83},{25.87,75.41},
  {90.76,147.03},{39.84,92.09},{89.20,136.14},{ 1.70,38.68},
  {32.49,69.34},{ 6.78,38.27},{32.36,78.73},{57.57,128.00},
  {81.11,147.45},{93.22,161.07},{75.48,137.02},{70.72,129.74},
  {33.18,80.42},{16.09,52.35},{12.92,42.89},{14.63,47.45},
  {16.33,60.65},{26.52,60.33},{65.75,144.10},{60.94,100.72},
  {28.23,76.55},{20.77,77.79},{46.35,83.09},{85.82,151.43},
  {51.71,91.53},{42.60,70.64},{ 4.22,48.90},{61.25,93.32},
  {14.76,46.45},{44.55,83.34},{57.30,106.17},{66.70,130.32},
  {47.19,113.56},{14.60,19.99},{29.04,67.68},{72.16,136.94},
  {24.30,60.44},{44.74,96.11},{58.89,105.52},{53.13,107.86},
  {67.93,143.40},{60.48,113.91},{47.24,104.36},{58.88,114.66},
  {80.90,148.45},{58.37,113.83},{89.03,138.68},{ 8.12,41.01},
  {24.14,68.27},{37.59,74.19},{44.67,83.16},{ 5.31,35.62},
  {57.88,103.64},{76.66,135.69},{47.92,97.61},{34.11,71.58},
  {73.72,133.44},{67.16,123.69},{ 2.70,45.81},{13.06,47.63},
  {70.19,132.68},{40.77,85.91},{68.57,122.66},{83.35,147.40},
  {95.10,159.26},{76.55,140.12},{44.42,92.71},{77.80,141.02},
  {18.03,54.85},{16.33,44.89},{54.49,102.97},{88.63,149.21},
  {53.20,104.65},{68.60,128.48},{13.34,52.97},{17.39,48.69},
  {95.50,153.55},{75.90,130.21},{93.37,171.51},{59.28,117.72},
  {22.33,72.20},{31.01,81.27},{16.35,50.69},{ 9.23,54.49},
  {86.04,158.82},{46.01,99.38},{32.61,47.24},{14.21,40.26},
  {37.28,76.21},{60.05,106.09},{94.82,165.99},{98.03,169.17},
  { 8.14,37.52},{29.47,58.43},{91.59,180.13},{64.46,123.22},
  {81.92,164.97},{40.79,93.04},{98.66,170.59},{85.47,151.64},
  {13.01,51.89},{69.39,134.86},{77.60,128.60},{89.83,157.49},
  {22.48,52.09},{61.31,118.09},{39.29,91.09},{57.26,80.48},
  {62.54,129.76},{30.22,81.60},{21.56,60.61},{65.89,99.43},
  {78.90,151.00},{93.57,158.74},{38.70,76.63},{ 8.25,47.63},
  {91.94,169.65},{54.05,110.82},{98.12,177.20},{17.59,55.03},
  {74.27,140.33},{58.40,122.38},{ 6.27,35.39},{90.31,164.73},
  {87.59,150.48},{47.17,97.51},{71.25,127.19},{56.58,106.56},
  {52.14,108.51},{24.20,80.20},{30.66,79.03},{59.72,116.04},
  {60.16,123.78},{22.09,68.92},{45.29,111.19},{92.16,156.21},
  {45.05,113.83},{81.73,145.26},{34.92,85.83},{13.92,67.65},
  {24.14,70.45},{81.85,153.42},{41.19,85.93},{ 0.67,39.26},
  {36.09,90.68},{19.54,62.25},{33.38,74.35},{10.19,55.59},
  {59.29,93.03},{11.58,62.19},{87.30,152.51},{25.43,59.13},
  {74.35,134.81},{65.23,139.29},{32.23,83.27},{70.94,130.63},
  {70.57,128.03},{15.13,65.98},{88.17,138.65},{ 9.49,43.25},
  {68.37,120.54},{74.69,143.68},{50.10,119.13},{ 8.76,32.56},
  {37.70,101.07},{ 4.04,42.48},{29.84,97.01},{69.88,130.23},
  { 8.65,45.47},{66.47,118.24},{12.35,55.24},{20.41,64.50},
  {91.28,166.16},{91.82,165.77},{10.65,52.39},{79.95,147.31},
  {16.06,57.21},{71.00,131.19},{91.27,155.74},{28.57,60.74},
  {31.63,92.00},{15.84,50.24},{80.22,135.00},{51.52,96.40},
  {67.42,117.71},{15.46,65.55},{85.71,151.69},{99.32,165.22},
  {28.38,107.61},{75.32,129.56},{96.48,161.61},{36.18,95.12},
  {34.18,71.39},{ 7.77,40.69},{37.98,65.63},{68.95,131.41},
  {34.08,78.15},{29.70,84.37},{92.61,178.05},{82.22,128.06},
  { 1.32,32.39},{ 2.57,41.54},{72.80,142.40},{19.88,47.67},
  {95.42,167.29},{ 7.56,63.21},{90.52,146.47},{67.80,115.47},
  {89.41,162.67},{87.72,165.63},{71.28,138.33},{25.08,70.34},
  {41.57,86.86},{51.45,104.07},{61.54,121.29},{32.53,93.45},
  { 3.26,29.72},{74.50,137.34},{45.07,102.67},{67.55,124.04},
  {38.14,88.93},{ 2.46,37.56},{21.27,66.20},{32.26,67.66},
  {36.05,64.93},{78.82,147.50},{ 7.26,40.83},{86.28,157.01},
  {49.02,95.59},{80.36,136.69},{ 2.94,46.23},{50.55,102.49},
  {28.35,69.73},{19.99,77.88},{22.42,72.52},{15.98,58.85},
  {68.79,130.10},{23.15,58.87},{ 3.27,40.79},{79.02,150.83},
  {37.04,72.00},{45.11,79.16},{30.61,58.70},{74.69,133.55},
  {25.47,71.56},{89.34,157.40},{61.39,125.26},{ 1.52,31.44},
  {78.37,136.81},{48.33,123.74},{71.58,126.89},{ 5.20,49.01},
  {90.20,142.37},{94.03,153.03},{97.27,167.60},{91.90,156.09},
  {56.45,131.39},{59.09,118.83},{61.75,115.24},{22.31,75.23},
  {73.98,142.59},{24.38,61.52},{73.46,110.70},{27.35,85.50},
  {55.66,115.25},{67.56,119.99},{61.81,130.15},{12.05,48.88},
  {84.16,146.44},{48.33,99.47},{83.37,138.40},{23.51,81.53},
  {85.14,138.65},{92.12,176.32},{53.13,115.81},{90.59,162.45},
  {92.52,182.64},{76.45,149.04},{48.40,96.60},{ 8.70,45.67},
  {96.13,174.15},{21.67,43.90},{15.13,43.97},{73.66,137.40},
  {80.31,136.39},{79.59,139.56},{55.33,112.94},{27.38,47.87}
};
