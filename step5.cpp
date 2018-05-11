// added to last step used avx instructions which load in this case 4 
// cells by 4 cells. NB: make sure when you ./a.out to use a matrix size divisible by 4
#include <iostream>
#include <sys/time.h>
#include <immintrin.h>
#include <cstdlib>

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return double(tv.tv_sec)+double(tv.tv_usec)*1e-6;
}

void fill_arrays(int n, double a[], double b[]) {
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      a[i*n + j] = rand()%10; //range from 0 to 99
      b[i*n + j] = rand()%10;
    }
  }
}

void print_arrays(int n, double a[], double b[], double c[]) {
  std::cout<<"printing a"<<std::endl;
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      std::cout<<a[i*n + j]<<" ";
    }
     std::cout<<"\n";
   }

   std::cout<<"printing b"<<std::endl;
   for(int i=0; i<n; i++) {
     for(int j=0; j<n; j++) {
     std::cout<<b[i*n + j]<<" ";
    }
      std::cout<<"\n";
   }

  std::cout<<"printing c"<<std::endl;
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      std::cout<<c[i*n + j]<<" ";
    }
      std::cout<<"\n";
  }
}

void mat_mult(int n, double a[], double b[], double c[]) {
  // double size 8 bytes. _256md can hold 256bit->32byte which means i can 
  // use 32/8=4 cells at a time.j's step will be 4
  #pragma omp parallel for
  for(int i=0; i<n; i++) {
    for(int k=0; k<n; k++) {
      __m256d a_tmp = _mm256_broadcast_sd(&a[i*n + k]);
      for(int j=0; j<n; j+=4) {
        __m256d b_tmp = _mm256_loadu_pd(&b[k*n + j]);
        __m256d c_tmp = _mm256_loadu_pd(&c[i*n + j]);
        c_tmp = _mm256_fmadd_pd(a_tmp,b_tmp,c_tmp);
        _mm256_storeu_pd(&c[i*n + j], c_tmp);
      }
    }
  }
}


int main(int argc, char **argv) {
  int n = atoi(argv[1]);
  double *a = new double[n*n]();
  double *b = new double[n*n]();
  double *c = new double[n*n]();
  
  fill_arrays(n,a,b);
  double tic = get_time();
  mat_mult(n,a,b,c);
  double toc = get_time();
  std::cout<<"N="<<n<<": "<<toc-tic<<"s ("<<2.*n*n*n/(toc-tic)/1e9<<"GFlops)"<<std::endl;
  //print_arrays(n,a,b,c);

  delete[] a;
  delete[] b;
  delete[] c;
}
