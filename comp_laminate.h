#ifndef COMP_LAMINATE_H_INCLUDED
#define COMP_LAMINATE_H_INCLUDED

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "material.h"

using namespace std;

vector <int> OrientationsetnoEmpty;
vector <int> Orientationset;
vector <int> Materialset;


//-----------------------------------------------------------------------------
//--------------------------------Gene class definition------------------------
class Gene{
  int value;
  vector<int> value_set;
 public:
  Gene(vector<int>valueset);
  ~Gene();
  int get();
  void mutate();
  void set(int a);

};
///create a gene,given its value set.randomly select a value from the set as its value.
Gene::Gene(vector<int>valueset)
{
  int num = rand()%valueset.size();
  value =valueset[num];
  value_set = valueset;
}
Gene::~Gene()
{
}

int Gene::get(){
  return value;
}

void Gene::set(int a){
  value=a;
}

void Gene::mutate(){
  int v;
  v=value;
  ///when we do a mutation, we make sure that the mutated value does not euqal to the origanl
  ///value or 0,which represent empty.
  while((v==value)||(v==0)) {
    v=value_set[rand()%(value_set.size())];
  }
  value =v;
}

//-----------------------------------------------------------------------------
//--------------------------Chromosome class definition------------------------
class Chromosome{
  vector <Gene> genes;
  int chromosomelength;
  vector<int> value_set;
  int nonempty;
  void create();
 public:
  Chromosome(int length,vector<int> valueset);
  Chromosome();
  ~Chromosome();

  ///arrange the genes in the chromosome so that all the empty genes are on the right side
  ///the return value is the number of non empty genes.
  int arrange();
  void createbycopy(Chromosome & chromosome);
  vector<Gene> & getAllgenes();
  int getGenevalue(int no);
  int length();
  void setGenevalue(int no,int value);
  void setGenevaluesFromParent(Chromosome &parent);
  void setNonEmpty(int value);
  void show();
  int getNonempty();
  void copyTo(Chromosome &re);
  int isIdeticalTo(Chromosome &c) ;
  void mutate(int no);
};
Chromosome::Chromosome()
{
}
Chromosome::Chromosome(int len, vector<int> valueset)
{
  chromosomelength = len;
  value_set = valueset;
  create();
  int ret =arrange();
  nonempty= ret;

}
Chromosome::~Chromosome()
{

}
void Chromosome::setNonEmpty(int value)
{
  nonempty = value;
}
int  Chromosome::getNonempty()
{
  return nonempty;
}
int Chromosome::arrange()
{
  int ret=0;
  int k=genes.size();
  for(int i=0;i<k;i++)
    {
      if(genes[i].get()==0)
	{
	  genes.erase(genes.begin()+i);
	  Gene g(value_set);
	  g.set(0);
	  genes.push_back(g);
	  ret++;
	  k--;
	  i--;
	}
    }
  ret = genes.size()-ret;
  return ret;
}

int Chromosome::length()
{
  return genes.size();
}

int Chromosome::getGenevalue(int no)
{
  if(no<0 || genes.size()<no)
    {
      cout << "Warning! Out of range (Gene)"<<endl;
      exit(1);
    }
  return genes[no].get();
}
vector<Gene>& Chromosome::getAllgenes()
{
  return genes;
}
void Chromosome::setGenevalue(int no,int value)
{
  genes[no].set(value);
}


///create a chromosome from a parent. just copy parent's genes to it.
void Chromosome::setGenevaluesFromParent(Chromosome &parent)
{
  for(int i=0;i<parent.length();i++)
    setGenevalue(i,parent.getGenevalue(i));
}

///after allocate a chromosome oject,call this to initialize the gene values.
void Chromosome::create()
{
  int i;
  for(i=0;i<chromosomelength;i++)
    {
      Gene gene(value_set);
      genes.push_back(gene);
    }
}

///mutate
void Chromosome::mutate(int no)
{
  genes[no].mutate();
}

void Chromosome::show()
{
  int i;
  cout << "Genes:{";
  for(i=0;i<chromosomelength;i++)
    cout << genes[i].get()<< "," ;
  cout << "}"<<endl;
}
void Chromosome::copyTo(Chromosome &re) 
{
  int i;
  for(i=0;i<chromosomelength;i++)
    re.setGenevalue(i,getGenevalue(i));
}
int Chromosome::isIdeticalTo(Chromosome &c)
{
  int i;
  for(i=0;i<chromosomelength;i++)
    if(getGenevalue(i) != c.getGenevalue(i)) 
      return 0;
  return 1;
}


//-----------------------------------------------------------------------------
//--------------------------Individual class definition------------------------
class Individual {
  Chromosome plyorientation;
  Chromosome material;
  vector<int> orientationset;
  vector<int> materialset;
  //the fitness value;
  double weightvalue;
  //the max load of this laminate
  double load;
  double rank;
  void create();

 public :
  Individual(int length);
  Individual(int length,vector<int>valuesetorientation, vector<int>valuesetmater);
  ~Individual();
  void createFromThis(Individual &in);
  void setRank(double x) {rank =x;}
  double getRank(){return rank;}
  double getFitnessValue()const;
  void setFitnessValue(double value);
  double getLoad()const;
  void setLoad(double maxload);
  int arrange();
  void setNonEmpty(int value);
  int getNonEmpty();
  Chromosome getChromosomeOrientation(){return plyorientation;}
  Chromosome getChromosomeMaterial(){return material;}
  vector<int> getValuesetOrientation(){return orientationset; }
  vector<int> getValuesetMaterial(){return materialset;}
  void setOrientation(Chromosome & ch);
  void setMaterial(Chromosome & ch);
  void setOrientationValue(int pos, int value);
  void setMaterialValue(int pos, int value);
  void show();
  ///three genetic operators
  void swapgenes(double p);
  void crossover(Individual &spouse, Individual &child, double p);
  void mutate(double p1,double p2,double p3);
  bool isSuperiorTo(Individual &in);
  bool isTheSameAs(Individual &in);
  ///whether this design is a same design for a cerntain load.
  bool safe();
};
///create a new individual,
/// length: number of genes in each chromosomes
Individual::Individual(int length, vector<int>valuesetorientation, vector<int>valuesetmaterial)
{
  orientationset = valuesetorientation;
  materialset = valuesetmaterial;
  Chromosome ch1(length,valuesetorientation);
  plyorientation = ch1;
  Chromosome ch2(length,valuesetmaterial);
  material =ch2;
  ///for empty ply, set its material as 0.
  for(int i=0;i<length;i++)
    {
      if(plyorientation.getGenevalue(i)==0)
	material.setGenevalue(i,0);
    }
}


Individual::~Individual()
{
}

double Individual::getFitnessValue()const
{
  return weightvalue;
}
void Individual::setFitnessValue(double value)
{
  weightvalue = value;
}
double Individual::getLoad()const
{
  return load;
}
void Individual::setLoad(double maxload)
{
  load = maxload;
}

int Individual::arrange()
{
  int ret=plyorientation.arrange();
  material.arrange();
  return ret;
}
void Individual::setNonEmpty(int value)
{
  plyorientation.setNonEmpty(value);
  material.setNonEmpty(value);
}
int  Individual::getNonEmpty()
{
  return plyorientation.getNonempty();
}
void Individual::setOrientation(Chromosome & ch)
{
  plyorientation = ch;
}
void Individual::setMaterial(Chromosome & ch)
{
  material = ch;
}

void Individual::setOrientationValue(int pos, int value)
{
  plyorientation.setGenevalue(pos,value);
}
void Individual::setMaterialValue(int pos, int value)
{
  material.setGenevalue(pos,value);
}

// Create an individual have the same choromsomes as this one
void Individual::createFromThis(Individual &in)
{
  in.setOrientation(plyorientation);
  in.setMaterial(material);
  in.setFitnessValue(getFitnessValue());
  in.setLoad(getLoad());
  in.setNonEmpty(getNonEmpty());
  in.setRank(getRank());
}
void Individual::show()
{
  cout<<"orientations:";
  plyorientation.show();
  cout<<"materials:";
  material.show();
  cout<<"fitness:"<<weightvalue<<endl;
}
bool Individual::isSuperiorTo(Individual &in)
{
  if (weightvalue>in.getFitnessValue())
    return true;
  else return false;
}
 
bool Individual::isTheSameAs(Individual &in)
{
  int i=0;
  for ( i=0;i<getNonEmpty();i++)
    {
      if(plyorientation.getGenevalue(i)!=in.getChromosomeOrientation().getGenevalue(i))
	{break;}
    }
  if(i!=getNonEmpty())
    {
      return false;
    }
  else
    {	
      int j=0;
      for ( j=0;j<getNonEmpty();j++)
	{
	  if(material.getGenevalue(j)!=in.getChromosomeMaterial().getGenevalue(j))
	    {break;}
	}
      if(j!=getNonEmpty())
	{
	  return false;	
	}
      else
	{
	  return true;
	}
    }
}
void Individual::crossover(Individual &spouse, Individual &child, double p)
{
  int a  = rand();
  int b = RAND_MAX;
  double possibility = ((double)a/(double)b);
  ///do a true crossover
  if(possibility<=p)
    {
      ///choose a crossover point;
      // int crossoverpoint = rand()%(plyorientation.length());
      int crossoverpoint = rand()%getNonEmpty();
      //Chromosome orientation = new Chromosome(plyorientation.length(),getValuesetOrientation());
      for(int i=0;i<crossoverpoint;i++)
        {
	  child.setOrientationValue(i,plyorientation.getAllgenes()[i].get());
	  child.setMaterialValue(i,material.getAllgenes()[i].get());
        }
      for(int j=crossoverpoint;j<child.getChromosomeOrientation().length();j++)
        {
	  child.setOrientationValue(j,spouse.getChromosomeOrientation().getGenevalue(j));
	  child.setMaterialValue(j,spouse.getChromosomeMaterial().getGenevalue(j));
        }
      int ret = child.arrange();
      child.setNonEmpty(ret);

    }
  ///else just copy this one to be the child
  else
    {
      createFromThis(child);
    }
}
///mutation contains three independent steps:
///1: adding a gene, only when there are empty genes
///2: deleting a non empty gene
///3: altering each non empty gene to other gene value except 0
void Individual::mutate(double p1,double p2,double p3)
{
  ///add a gene only when there are at least one empty genes.
  if(getNonEmpty()<plyorientation.length())
    {
      int a  = rand();
      double possibility = ((double)a/(double)RAND_MAX);
      if(possibility<=p1)
        {
	  Gene g1(OrientationsetnoEmpty);
	  Gene g2(materialset);
	  plyorientation.getAllgenes().pop_back();
	  plyorientation.getAllgenes().insert(plyorientation.getAllgenes().begin(),g1);
	  material.getAllgenes().pop_back();
	  material.getAllgenes().insert(material.getAllgenes().begin(),g2);
	  setNonEmpty(getNonEmpty()+1);
        }
    }
  ///delete non empty genes
  if(getNonEmpty()>0)
    {
      int a  = rand();
      double possibility = ((double)a/(double)RAND_MAX);
      if(possibility<=p2)
        {
	  int which = rand()%getNonEmpty();
	  plyorientation.setGenevalue(which,0);
	  material.setGenevalue(which,0);
	  int nonempty = arrange();
	  setNonEmpty(nonempty);
        }
    }
  ///alter every gene with small prob
  for(int i=0;i<getNonEmpty();i++)
    {
      int a  = rand();
      double possibility = ((double)a/(double)RAND_MAX);
      if(possibility<=p3)
        {
 	  int whichorientation = rand()%getNonEmpty();
	  int value1 = plyorientation.getGenevalue(whichorientation);
	  int which1;
	  int v1 =value1;

	  while(v1==value1)
            {
	      which1 = rand()%OrientationsetnoEmpty.size();
	      v1 = OrientationsetnoEmpty[which1];
            }
	  plyorientation.setGenevalue(whichorientation,OrientationsetnoEmpty[which1]);
        }
      int b  = rand();
      double possibility1 = ((double)b/(double)RAND_MAX);
      if(possibility1<=p3)
	{
			
	  int whichmaterial=1;
	  whichmaterial= rand()%getNonEmpty();
			
	  int which2;
	  int value2 = material.getGenevalue(whichmaterial);

	  int v2=value2;
	  while(v2==value2)
	    {
	      which2 = rand()%Materialset.size();
	      v2=Materialset[which2];
	    }
	  material.setGenevalue(whichmaterial,Materialset[which2]);
	} 
    }
}
///swap two nonempty genes
void Individual::swapgenes(double p)
{
  if(getNonEmpty()>=2)
    {
      int a  = rand();
      double possibility = ((double)a/(double)RAND_MAX);
      if(possibility<=p)
        {

	  int which1 = rand()%getNonEmpty();
	  int which2=which1;
	  while(which2==which1)
	    {
	      which2 = rand()%getNonEmpty();
	    }
 	  int value1 = plyorientation.getGenevalue(which1);
	  int value2 = material.getGenevalue(which1);
	  plyorientation.setGenevalue(which1,plyorientation.getGenevalue(which2));
	  material.setGenevalue(which1,material.getGenevalue(which2));
	  plyorientation.setGenevalue(which2,value1);
	  material.setGenevalue(which2,value2);
        }
    }
}
bool Individual::safe()
{
  double Axx=0;
  double Axy=0;
  double Ayy=0;
  double Ass=0;

  for(int i=0;i<getNonEmpty();i++)
    {
      Axx = Axx + Qxx[material.getGenevalue(i)-1][plyorientation.getGenevalue(i)] * t[material.getGenevalue(i)-1]*2;
      Axy = Axy + Qxy[material.getGenevalue(i)-1][plyorientation.getGenevalue(i)] * t[material.getGenevalue(i)-1]*2;
      Ayy = Ayy + Qyy[material.getGenevalue(i)-1][plyorientation.getGenevalue(i)] * t[material.getGenevalue(i)-1]*2;
      Ass = Ass + Qss[material.getGenevalue(i)-1][plyorientation.getGenevalue(i)] * t[material.getGenevalue(i)-1]*2;
    }
  Axx=Axx*2;
  Axy=Axy*2;
  Ayy=Ayy*2;
  Ass=Ass*2;
  double angleT[]={0,0,15,30,45,60,75,90};

  double exx = (Nx*Ayy-Ny*Axy)/(Axx*Ayy-Axy*Axy);
  double eyy = (Nx*Axy-Ny*Axx)/(Axy*Axy-Axx*Ayy);
  exx = exx*(1E-7);
  eyy = eyy*(1E-7);
  for(int i=0;i<getNonEmpty();i++)
    {
      double theta=angleT[plyorientation.getGenevalue(i)]/180*pi;
      double e11=pow(cos(theta),2)*exx+pow(sin(theta),2)*eyy;
      double e22=pow(sin(theta),2)*exx+pow(cos(theta),2)*eyy;
      double e12=-2*sin(theta)*cos(theta)*exx+2*sin(theta)*cos(theta)*eyy;

      double lam=1-max(e12/e12a[material.getGenevalue(i)-1],max(e11/e11a[material.getGenevalue(i)-1],e22/e22a[material.getGenevalue(i)-1]));
      if (lam<0)
	{
	  return false;
	}
    }

  vector<double>z;
  for(int i=0;i<=getNonEmpty();i++)
    {
      z.push_back(0.0);
    }
  for(int i=0;i<getNonEmpty();i++)
    {
      z[i+1]=z[i]+t[material.getGenevalue(i)-1]*2;
    }
  double Dxx=0;
  double Dxy=0;
  double Dyy=0;
  double Dss=0;
  for(int i=0;i<getNonEmpty();i++)
    {
      Dxx=Dxx+(1E+7)*Qxx[material.getGenevalue(i)-1][plyorientation.getGenevalue(i)]*(pow(z[i+1],3)-pow(z[i],3));
      Dxy=Dxy+(1E+7)*Qxy[material.getGenevalue(i)-1][plyorientation.getGenevalue(i)]*(pow(z[i+1],3)-pow(z[i],3));
      Dyy=Dyy+(1E+7)*Qyy[material.getGenevalue(i)-1][plyorientation.getGenevalue(i)]*(pow(z[i+1],3)-pow(z[i],3));	
      Dss=Dss+(1E+7)*Qss[material.getGenevalue(i)-1][plyorientation.getGenevalue(i)]*(pow(z[i+1],3)-pow(z[i],3));
		
    }
  Dxx=Dxx*2/3;
  Dxy=Dxy*2/3;
  Dyy=Dyy*2/3;
  Dss=Dss*2/3;
	
  // to check buckling failure
  double lam01=pow(pi,2)*(Dxx*pow(0/L1,4)+2*(Dxy+2*Dss)*pow(0/L1,2)*pow(1/L2,2)+Dyy*pow(1/L2,4))/(pow(0/L1,2)*Nx+pow(1/L2,2)*Ny);
  double lam10=pow(pi,2)*(Dxx*pow(1/L1,4)+2*(Dxy+2*Dss)*pow(1/L1,2)*pow(0/L2,2)+Dyy*pow(0/L2,4))/(pow(1/L1,2)*Nx+pow(0/L2,2)*Ny);
  double lam11=pow(pi,2)*(Dxx*pow(1/L1,4)+2*(Dxy+2*Dss)*pow(1/L1,2)*pow(1/L2,2)+Dyy*pow(1/L2,4))/(pow(1/L1,2)*Nx+pow(1/L2,2)*Ny);

  double lamcr=min(lam01,min(lam10,lam11));
  if(lamcr>1.0)
    {
      setLoad(lam11*100);
      return true;
    }
  else
    {
      setLoad(lam11*100);
      return false;
    }
	
}


//-----------------------------------------------------------------------------
//----------------------SelectionModel class definition------------------------
///the base class that represent a parent selection model
class SelectionModel{
 public :
  virtual int select()=0;
  virtual void initialize(vector<Individual> &pop)=0;
};

//-----------------------------------------------------------------------------
//----------------------SelectByRoulette class definition----------------------
class SelectByRoulette:public SelectionModel{
  vector<pair<double,double> >ranks;
  vector<Individual> population;
  bool chooseMe(pair<double,double> rank, double p);
  void setRank();
 public:
  SelectByRoulette(vector<Individual> population);
  void initialize(vector<Individual> &population);
  int select();
};
SelectByRoulette::SelectByRoulette(vector<Individual> pop)
{
  population = pop;
  pair<double,double> temp(0.0, 0.0);
  for(int i=0;i<pop.size();i++)
    {
      ranks.push_back(temp);
    }
  setRank();
}
void SelectByRoulette::initialize(vector<Individual> &pop)
{
	
  population = pop;
  if(ranks.size()>0)
    ranks.clear();
  pair<double,double> temp(0.0, 0.0);
  for(int i=0;i<population.size();i++)
    {
      ranks.push_back(temp);
    }
  setRank();
}
bool SelectByRoulette::chooseMe(pair<double,double> rank, double p)
{
  if((p>=rank.first)&&(p<rank.second))
    return true;
  else return false;
}
int SelectByRoulette::select()
{
  int a  = rand();
  int b = RAND_MAX;
  double rank;
  rank=((double)a/(double)b);
  while(rank==1.0)
    {
      a=rand();
      rank=((double)a/(double)b);
    }
  for(int i=0;i<population.size();i++)
    {
      if(chooseMe(ranks[i],rank))
	{
	  return i;
	}
    }
  cerr<<"sth wrong in SelectByRoulette::select,rank="<<rank<<endl;
}
///here we assume that the input vector has aready been sorted by its fitness value.
void  SelectByRoulette::setRank()
{
  double d1=0.0;
  double d2=0.0;
  int size = population.size();
  for(int i=0;i<size;i++)
    {
      d2=d1+2*((double)size-(double)i)/((double)size*((double)size+1));
      ranks[i].first = d1;
      ranks[i].second=d2;
      d1 = d2;
    }
}

//-----------------------------------------------------------------------------
//----------------------SelectionModel class definition------------------------
///the base class that represent a fitness evaluation model
class FitnessModel {
 public:
  virtual double evaluate(Individual &in)=0;
  virtual void show(Individual &in)=0;
};

//-----------------------------------------------------------------------------
//----------------------OptimizeWeightModel class definition-------------------
class OptimizeWeightModel:public FitnessModel
{
  vector<int> orientations;
  vector<int> materials;
 public:
  OptimizeWeightModel();
  double evaluate(Individual &in);
  void show(Individual &in);
};
OptimizeWeightModel::OptimizeWeightModel()
{
}

double OptimizeWeightModel::evaluate(Individual &in)
{
  if(orientations.size()>0)
    orientations.clear();
  if(materials.size()>0)
    materials.clear();
  double fitness = 0.0;
  for(int i=0;i<in.getChromosomeOrientation().length();i++)
    {
      orientations.push_back(in.getChromosomeOrientation().getGenevalue(i));
      materials.push_back(in.getChromosomeMaterial().getGenevalue(i));
    }
  ///now evalute fitness.
  for(int i=0;i<orientations.size();i++)
    {		
      if(orientations[i]==0)
	break;
      fitness += p[materials[i]-1]*L1*L2*t[materials[i]-1]*2;
    }
  in.setFitnessValue(fitness*2);
  return fitness*2;
}

void OptimizeWeightModel::show(Individual &in)
{

}


//-----------------------------------------------------------------------------
//------------------------GeneticAlgorithm class definition---------------------
class GeneticAlgorithm {
  vector<Individual> population;
  vector<Individual> newpopulation;
  //populationsize
  int populationsize;
  int generation;
  int chromesomesize;
  double p_crossover;
  double p_add;
  double p_delete;
  double p_alter;
  double p_swap;
  FitnessModel  * model;
  SelectionModel *selectmodel;
  int selectpower;	
  void eliteSet();

 public:
  GeneticAlgorithm(int populationsize=20,int chromosomesize=20);
  void initialize();
  //set individual's chromosome manually
  void setIndividual(int no,Individual &in);
  //set crossover probobility,  p should (0,1]
  void setCrossover(double p); 
  // set mutation rate, 
  void setMutation( double p_add,  double p_delete, double p_alter);
  //set gene swap rate
  void setSwap(double swap);
  void setGeneration(int n) ;
  //evaluation model of this GA
  void setModel(FitnessModel *m);
  void setSelectModel(SelectionModel *m);
  void setRanking();
  void setSelectPower(int s);
  vector<Individual> getPopulation();
  //sort the population based on its fitness,
  //the most superior one is at the BEGINNIG of the population vector..
  void sortpopulation();
  void sortnewpopulation();
  //evaluate the fitnesses of the population
  void evaluate();
  //evaluate an individual in the population
  void evaluate(int no);
  void evaluatenewpopulation();
  void evaluatenew(int no);
  int getPopulationSize() { return populationsize;}
  //optimazation
  void optimize();
  void swapPopulation();
  //use selectpower children to replace the bad ones in parents generation
  void genNextGeneration(int selectpower);		
  //use the N-Nk best parents and new generate Nk children
  void genNextGeneration1(int Nk);
  Individual &get(int no) {return population[no];}
  void show(int no);
  void showAll();
  void crossover(int father, int mother,Individual &child);
  void mutate();
  void showResult(int j);
};

GeneticAlgorithm::GeneticAlgorithm(int popsize,int chromsize)
{
  populationsize = popsize;
  chromesomesize = chromsize;
  p_crossover = 0.8;
  p_add = 0.2;
  p_delete=0.2;
  p_alter=0.2;
  p_swap=0.2;
  generation = 1000;
  population.reserve(popsize);
  newpopulation.reserve(popsize);
}

void GeneticAlgorithm::initialize()
{
  if(population.size()>0)
    population.clear();
  if(newpopulation.size()>0)
    newpopulation.clear();
  for(int i=0;i<populationsize;i++)
    {
    HERE:
      Individual ind(chromesomesize, OrientationsetnoEmpty,Materialset);
      Individual *p=&ind;
      while(!(p->safe()))
	{
	  goto HERE;
	}
      double value = model->evaluate(ind);
      ind.setFitnessValue(value);
      population.push_back(ind);
      newpopulation.push_back(ind);
    }
}

void GeneticAlgorithm::setCrossover(double p)
{
  p_crossover = p;
}
vector<Individual> GeneticAlgorithm::getPopulation()
{
  return population;
}
// set mutation rate, 
void  GeneticAlgorithm::setMutation(double  padd, double pdelete,double palter)
{
  p_add = padd;
  p_delete = pdelete;
  p_alter = palter;
}
void GeneticAlgorithm::setSelectPower(int s)
{
  if((s<0)||(s>populationsize))
    {
      cerr<<"wrong paramiter: the num of children used to replace the bad individuals in parent population should be <=populationsize"<<endl;
    }
  selectpower = s;
}
void  GeneticAlgorithm::setSwap(double swap)
{
  p_swap = swap;
}
void GeneticAlgorithm::setGeneration(int n) 
{
  generation = n;
}
void GeneticAlgorithm::setModel(FitnessModel *m)
{
  model =m;
}
void GeneticAlgorithm::setSelectModel(SelectionModel *m)
{
  selectmodel = m;
}

void  GeneticAlgorithm::show(int no)
{
  population[no].show();

}
void  GeneticAlgorithm::showAll()
{

  for (int i=0;i<population.size();i++)
    {	
      cout<<"individual "<<i<<":"<<endl;
      show(i);
    }
}
void  GeneticAlgorithm::sortpopulation()
{
  ::sort(population.begin(),population.end());
}
void GeneticAlgorithm::sortnewpopulation()
{
  ::sort(newpopulation.begin(),newpopulation.end());
}
//operator for sorting
bool  operator <( const Individual & x, const Individual & y)
{
  if(x.getFitnessValue()<y.getFitnessValue())
    return true;	 
  else if(x.getFitnessValue()==y.getFitnessValue())
    {
      if(x.getLoad()>y.getLoad())
	{
	  return true;
	}
      else return false;
    }	 
  else return false;
}
//evaluate the fitnesses of the population
void  GeneticAlgorithm::evaluate()
{
  double value = 0.0;
  for(int i=0;i<population.size();i++)
    {
      value =  model->evaluate(population[i]);
      population[i].setFitnessValue(value);
    }
}

void  GeneticAlgorithm::evaluatenewpopulation()
{
  double value = 0.0;
  for(int i=0;i<newpopulation.size();i++)
    {
      value =  model->evaluate(newpopulation[i]);
      newpopulation[i].setFitnessValue(value);
    }
}

//evaluate an individual in the population
void  GeneticAlgorithm::evaluate(int no)
{
  double value = model->evaluate(population[no]);
  population[no].setFitnessValue(value);
}
//evaluate an individual in the population
void  GeneticAlgorithm::evaluatenew(int no)
{
  double value = model->evaluate(newpopulation[no]);
  newpopulation[no].setFitnessValue(value);
}
///make new generation as generation
void GeneticAlgorithm::swapPopulation()
{
  for(int i=0;i<population.size();i++)
    {
      newpopulation[i].createFromThis(population[i]);
    }
}
void GeneticAlgorithm::genNextGeneration(int selectpower)
{
  if((selectpower<0)||(selectpower>populationsize))
    {
      cerr<<"wrong paramiter: the num ofchildren used to replace the bad individuals in parent population should be <=populationsize"<<endl;
    }
  int start = populationsize-selectpower;
  int k=0;
  for(int i=start;i<populationsize;i++)
    {
      newpopulation[k].createFromThis(population[i]);
      k++;
    }
}

void GeneticAlgorithm::crossover(int father, int mother,Individual &child)
{
  population[father].crossover(population[mother],child, p_crossover);
}
void GeneticAlgorithm::mutate()
{
  for(int i=0;i<newpopulation.size();i++)
    {
      newpopulation[i].mutate(p_add,p_delete,p_alter);
      newpopulation[i].swapgenes(p_swap);
    }
}
void GeneticAlgorithm::optimize()
{
  int gencount = 0;
  int individualcount=0;
  int father,mother;
  evaluate();
  sortpopulation();
  cout<<"___________________________________________________"<<endl;
  cout<<"best in generation:"<<gencount<<endl;
  cout<<"weight:"<<population[0].getFitnessValue()<<"  Load:"<<population[0].getLoad()<<endl;
  population[0].show();
  while(gencount<generation)
    {
      while(individualcount<selectpower)
	{
	  father = selectmodel->select();
	  mother=father;
	  while(mother==father)
	    {
	      mother = selectmodel->select();
	    }
	  crossover(father,mother,newpopulation[individualcount]);
	  newpopulation[individualcount].mutate(p_add,p_delete,p_alter);	
	  newpopulation[individualcount].swapgenes(p_swap);

	  if(!newpopulation[individualcount].safe())
	    {
	      continue;
	    }
	  int k=0;
	  for(k=0;k<individualcount;k++)
	    {
	      if(newpopulation[individualcount].isTheSameAs(newpopulation[k]))
		{
		  break;
		}
	    }
	  if(k!=individualcount)
	    {
	      continue;
	    }
	  for( k=0;k<populationsize;k++)
	    {
	      if(newpopulation[individualcount].isTheSameAs(population[k]))
		{
		  break;
		}
	    }	
	  if(k!=populationsize)
	    {
	      continue;
	    }
	  individualcount++;
	}
      individualcount=0;
      evaluatenewpopulation();
      //sortnewpopulation();
      genNextGeneration(selectpower);
      sortpopulation();	
      selectmodel->initialize(population);
      // double fit = population[0].getFitnessValue();
      // if(fit<1.30)
      //  {
      //	 cout<<"total generations: "<<gencount<<endl;
      //	 break;
      //  }
      gencount++;
      cout<<"___________________________________________________"<<endl;
      cout<<"best in generation:"<<gencount<<endl;
      cout<<"weight:"<<population[0].getFitnessValue()<<"  Load:"<<population[0].getLoad()<<endl;
      population[0].show();
      /*
	if(gencount>=20){
	cout<<"see all individuals? y/n"<<endl;
	char yn;
	cin>>yn;
	if(yn=='y')
	{showAll();	}
	}	
      */
    }
  // cout<<"best in generation:"<<gencount<<endl;
  // cout<<"weight:"<<population[0].getFitnessValue()<<"  Load:"<<population[0].getLoad()<<endl;
  // population[0].show();
}
#endif // COMP_LAMINATE_H_INCLUDED
