#ifndef RKIN_H
#define RKIN_H

#define emass1 emass1_
#define rkin rkin_
#define loademass loademass_

extern "C" double emass1(int &A, int &Z);

extern "C" void rkin(double &inang, double &inen, 
		     char inreac[], double &inex,
		     double &Q, double &en3,
		     double &labang3, double &cmang3,
		     double &en4, double &labang4, double &ratiop);

extern "C" void loademass();

inline double em(int A, int Z)
{
  return emass1(A, Z);
}

struct particle
{
  double energy;//MeV
  double labangle;//deg
  double cmangle;//deg
  double ratio;//SigCM/SigLab
  double Q;//MeV
  //                 char pname[32];
  //                 int charge;
  //                 int spin;
  //                 int parity;
  //                 float mass;
};

inline void rk(double inang, double inen, char inreac[], double inex, particle &p3, particle &p4)
{
  double Q, en3, labang3, cmang3;
  double ratiop, labang4, en4;

  char tmp[255];
  sprintf(tmp,"%-25s",inreac);

  rkin(inang, inen, tmp, inex,Q,en3,labang3,cmang3,
       en4,labang4,ratiop);

  if ((Q+inen)>0)
  {
    //Detected particle
    p3.energy=en3;
    p3.labangle=labang3;
    p3.cmangle=cmang3;
    p3.Q=Q;
    p3.ratio=ratiop;
    
    //Undetected particle
    p4.energy=en4;
    p4.labangle=labang4;
    p4.cmangle=180-cmang3;
    p4.Q=Q;
    p4.ratio=ratiop;
  }

  else
  {
    //Detected particle
    p3.energy=-1;
    p3.labangle=0;
    p3.cmangle=0;
    p3.Q=Q;
    p3.ratio=0;
    
    //Undetected particle
    p4.energy=-1;
    p4.labangle=0;
    p4.cmangle=0;
    p4.Q=Q;
    p4.ratio=0;
  }
}

inline void loadmasses()
{
  loademass();
}

#endif
