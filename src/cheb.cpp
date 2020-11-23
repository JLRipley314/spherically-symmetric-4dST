#include <cassert> 
#include <cmath> 
using std::pow;
#include <iomanip>
#include <iostream>
using std::cout; 
using std::endl;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
using std::stod;
#include <vector>
using std::vector;

#include "cheb.hpp"
	
/*===========================================================================*/
Cheb::Cheb(const string home_dir, const int Nx)
: nx{Nx},

  cheb_mat(    nx,vector<double>(nx)), 
  inv_cheb_mat(nx,vector<double>(nx))
{
/*---------------------------------------------------------------------------*/
    string dir= home_dir+"/cheb_tables";

    get_cheb(dir, "/cheb_mat",     cheb_mat);
    get_cheb(dir, "/inv_cheb_mat", inv_cheb_mat);
/*---------------------------------------------------------------------------*/
}
/*===========================================================================*/
Cheb::~Cheb(void)
{
}
/*===========================================================================*/
void Cheb::get_cheb(
   const string dir, const string file_name, 
   vector<vector<double>> mat)
{
    ifstream infile(dir+"/"+file_name+".txt");
    assert(infile.good());

    string val;

    for (int i=0; i<nx; ++i) {
        for (int j=0; j<nx; ++j) {
            infile >> val;
            mat[i][j]= stod(val);
        }
    }
    return;
}
/*===========================================================================*/
void Cheb::derivative(const vector<double> v, vector<double> dv) const
{
   for (int i=0; i<nx; i++) {
      dv[i]= 0;
      for (int j=0; j<nx; j++) {
         dv[i]+= cheb_mat[i][j]*v[j];
      }
   }
   return;
}
/*===========================================================================*/
void Cheb::integral(const vector<double> v, vector<double> iv) const
{
   for (int i=0; i<nx; i++) {
      iv[i]= 0;
      for (int j=0; j<nx; j++) {
         iv[i]+= inv_cheb_mat[i][j]*v[j];
      }
   }
   return;
}
/*===========================================================================*/
void Cheb::r_derivative(
   const double cl,
   const vector<double> x_v,
   const vector<double> v, vector<double> dv) const
{
   const double x_exc= x_v[0];
   const double inv_a = 1.0 / (0.5*(cl - x_exc));

   derivative(v,dv);

   for (int i=0; i<nx; i++) {
      dv[i]*= inv_a*pow(1.0 - (x_v[i]/cl),2);
   }

   return;
}
/*===========================================================================*/
void Cheb::r_integral(
   const double cl,
   const vector<double> x_v,
   const vector<double> v, vector<double> iv) const
{
   const double x_exc= x_v[0];
   const double a = 0.5*(cl - x_exc);

   integral(v,iv);

   for (int i=0; i<nx-1; i++) {
      iv[i]*= a*pow(1.0 - (x_v[i]/cl),-2);
   }
   iv[nx-1]= iv[nx-2];

   return;
}