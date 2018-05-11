/* instead of accessing the result array in each iteration we can do the summation in a variable called sum
 * and just accessing the result array in the end.
 */
#include <iostream>
#include <sys/time.h>
#include <cstdlib>

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return double(tv.tv_sec)+double(tv.tv_usec)*1e-6;
}

void fill_arrays(int n, double a[], double b[]) {
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      a[i*n + j] = rand()%100; //range from 0 to 99
      b[i*n + j] = rand()%100;
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
  double sum = 0;
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      for(int k=0; k<n; k++) {
        sum += a[i*n + k]*b[k*n + j];
      }
      c[i*n + j] = sum;
      sum = 0;
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
