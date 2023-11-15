#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <cmath>

using namespace std;

#define L  1024
#define Lsqrt 32
#define T 1
#define NumberOfIteration  10000
#define P 100
#define minB  -200
#define maxB  200



double*  initB(int, int, int);
double** initW(int, int);
int* createVector(int);
int** vectorToMatrix(int* X, int lenMat);
double* ising(double** W, int* X, int lenX, double M [], double E []);

ostream &operator << ( ostream &out, double** Matrix )
{
  //cout<<"test"<<endl;
   for (int i=0; i<L; i++)
   {
      for ( int j=0;j<L;j++ ){ out << Matrix[i][j] << '\t'; }
      out << '\n';
     //cout<<'\n';
   }
   return out;
}

ostream &operator << ( ostream &out, int** Matrix )
{
  //cout<<"test"<<endl;
   for (int i=0; i<Lsqrt; i++)
   {
      for ( int j=0;j<Lsqrt;j++ ){ out << Matrix[i][j] << '\t';}
      out << '\n';
     //cout<<'\n';
   }
   return out;
}

ostream &operator << ( ostream &out, double* List )
{
  
   for (int i=0; i<NumberOfIteration; i++)
   {
      out << List[i] ;
      out << '\n';
     
   }
   return out;
}

int main()
{
  int* X=createVector(L);
  double * M =new double[NumberOfIteration];
  double * E =new double[NumberOfIteration];
  
  double * B;
  B = initB(L, minB, maxB);
 
  double ** W;
  W= initW(L,Lsqrt);
  ofstream outFile( "W.out" );
  outFile<<W;

  //cout<<X[L]<<endl;
 
  int ** Mat;
  Mat = vectorToMatrix( X, Lsqrt);
  ofstream outFile2( "Mat_init.out" );
  outFile2<<Mat;
  
  ising(W, X, L, M, E);

  ofstream outEnfile( "Energy.out" );
  outEnfile<<E;

  ofstream outMagfile( "Mag.out" );
  outMagfile<<M;

  Mat=vectorToMatrix(X, Lsqrt);
  ofstream outFileMatFinal( "Mat_Final.out" );
  outFileMatFinal<<Mat;
  //for(int i=0; i<NumberOfIteration; i++)
    //cout<<" "<<E[i]<<endl;

  //std::cout<<X[0]<<std::endl<<M[0]<<std::endl<<E[0]<<std::endl<<B[0]<<std::endl<<W[0][0]<<std::endl;
  
  return 0;
}

double*  initB(int lenX, int min, int max){
  double * B = new double[lenX];

  return B;
}

double** initW2(int lenX, int minW, int maxW){
  double** W=new double* [lenX];

  return W;
}

double** initW(int lenX, int lenMat){
  double** W=new double* [lenX];
  for(int i=0; i<lenX; i++){
    W[i]=new double [lenX]; 
    for(int j=0; j<lenX; j++){
      W[i][j]=0.0;
    }
    W[i][(i-1)%lenX]=1.0;
    W[i][(i+1)%lenX]=1.0;
    W[i][(i-lenMat)%lenX]=1.0;
    W[i][(i+lenMat)%lenX]=1.0;

  }
  return W;
}

int* createVector(int lenX){
  int* X = new int[lenX];
  for(int i=0;i<lenX;i++){
    X[i]=random()%2;
    if (X[i]==0)
      X[i]=X[i]-1;      
  }
  return X;
}

int** vectorToMatrix(int* X, int lenMat){
  int** Mat = new int* [lenMat];
  for(int i=0; i<lenMat; i++){
    Mat[i] = new int [lenMat];
    for(int j=0; j<lenMat; j++){
      Mat[i][j]=X[i*lenMat+j];
    }
  }
  return Mat;
}


void showIsingModel(int **X, int lenX){
}


double energy(double** W, int* X, int lenX){
  double E=0;
  for(int i=0; i<lenX; i++){
    for(int j=0;j<lenX;j++){
      E+=W[i][j]*X[i]*X[j];
    }
  }
  //cout<<"Energy: "<<-E<<endl;
  return -E;
}

int magnetization(int* X, int lenX){
  int M=0;
  for(int i=0;i<lenX;i++){
    M+=X[i];
  }
  return M;
}

double deltaEnergy(double** W, int* X, int lenX, int i){
  double deltaE=0;
  double H_i=0.0;
  for(int j=0; j<lenX; j++){
    H_i=H_i+W[i][j]*X[j];
  }
  deltaE=H_i*X[i];
  return deltaE;
}


double metropolis(double** W, int* X, int lenX, double OldE){
  int i = random()%lenX;
  X[i]*=-1;
  double E = energy(W,X,lenX);
  double deltaE = E-OldE;
  if(deltaE<0){
    //cout<<"case 1 D: "<<deltaE<<endl;
    return E;
  }
  const long max_rand = 1000000L;
  double r = (0.0+(random() % max_rand))/ max_rand;
  double pp =exp(-deltaE/T);
  if (r <pp){
    //cout<<"case 2 D: "<<deltaE<<" pp: "<<pp<<" r : "<<r<<endl;
    return E;
  }
  //cout<<"\t\tcase3"<<" D: "<<deltaE<<" pp: "<<pp<<endl;
  X[i]*=-1;
  return OldE;
}


double* ising(double** W, int* X, int lenX, double M [], double E []){


  E[0] = energy(W,X,lenX);
  M[0] = magnetization(X,lenX);

  for(int t=1;t<NumberOfIteration;t++){
    E[t]=metropolis(W,X,lenX,E[t-1]);
    M[t]=magnetization(X,lenX);
  }
  return E;
}

