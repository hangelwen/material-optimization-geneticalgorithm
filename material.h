#ifndef COMP_MATERIAL_H_INCLUDED
#define COMP_MATERIAL_H_INCLUDED
//material information
double pi = 3.1415926;
double L1=36;            // length
double L2=30;             // width
double p[2]={0.0055,0.0048};  //material density
//double t[2]={0.00525,0.00716};    // ply thickness 
double t[2]={0.00525,0.00525};


double Nx=100; // load in x direction
double Ny=100; //load in y direction


double e11a[]={9.89E-3,2.87E-3};         //critical normal strain--x
double e22a[]={3.87E-3,3E-3};            //critical normal strain--y
double e12a[]={1.9E-3,1.21E-3};          //critical shear strain--xy

double Qxx[2][8]={{0,2.2094,1.9352,1.2858,0.6348,0.2564,0.1523,0.1506},{0,1.1938,1.0463,0.69590,0.34101,0.12911,0.0646,0.0602}};
double Qxy[2][8]={{0,0.0377,0.1740,0.4465,0.5828,0.4465,0.1740,0.0377},{0,0.0150,0.0865,0.22952,0.30101,0.22952,0.0865,0.0150}};
double Qyy[2][8]={{0,0.1506,0.1523,0.2564,0.6348,1.2858,1.9352,2.2094},{0,0.0602,0.0646,0.12911,0.34101,0.69590,1.0463,1.1938}};
double Qss[2][8]={{0,0.0520,0.3246,0.8698,1.1424,0.8698,0.3246,0.0520},{0,0.0400,0.1830,0.46895,0.61193,0.46895,0.1830,0.0400}};
#endif
