#include <iostream>

#include "comp_laminate.h"

using namespace std;


int main(int argc, const char * argv[])
{
  int popsize=40;
  int selectpower=28;
  int generation=1000;
  double p_crossover=0.8;
  double p_add=0.05;
  double p_del=0.3;
  double p_alter=0.3;
  double p_swap=0.3;
  if((argc!=1)&&(argc!=9))
    {
      cout<<"Usage: program [<popsize> <selectpower> <generation> <p_crossover> <p_add> <p_del> <p_alter> <p_swap>]"<<endl;
	exit(-1);
    }
  if(argc==1)
    {
      srand(time(NULL));
    }
  else if(argc==9)
    {
      srand(time(NULL));
      popsize=atoi(argv[1]);
      selectpower=atoi(argv[2]);
      generation=atoi(argv[3]);
      p_crossover=atof(argv[4]);
      p_add=atof(argv[5]);
      p_del=atof(argv[6]);
      p_alter=atof(argv[7]);
      p_swap=atof(argv[8]);
    }
  Materialset.push_back(1);
  Materialset.push_back(2);
  Orientationset.push_back(0);
  int maxorietation = 7;
  for (int i=1;i<=maxorietation;i++)
    {
      Orientationset.push_back(i);
      OrientationsetnoEmpty.push_back(i);
    }
  GeneticAlgorithm GA(popsize,20);
  cout<<"___________________________________"<<endl;
  cout<<"population size:"<<GA.getPopulationSize()<<endl;
  OptimizeWeightModel m;
  OptimizeWeightModel *model = &m;
  GA.setModel(model);   
  GA.setGeneration(generation);
  GA.setSelectPower(selectpower);
  GA.setCrossover(p_crossover);
  GA.setMutation(p_add,p_del,p_alter);
  GA.setSwap(p_swap);
  GA.initialize();
  //GA.showAll();
  SelectByRoulette  sm(GA.getPopulation());
  SelectByRoulette *smodel=&sm;
  GA.setSelectModel(smodel);
  GA.optimize();
  getchar();
  return 0;
}
 

void optimize(int popsize,int selectpower)
{
  GeneticAlgorithm GA(popsize,20);
  cout<<"___________________________________"<<endl;
  cout<<"population size:"<<GA.getPopulationSize()<<endl;
		
  //TODO:now we need to call setModel before initialzie,because we need to call evaluation in initialzie
  OptimizeWeightModel m;
  OptimizeWeightModel *model = &m;
  GA.setModel(model);   
  GA.setGeneration(10000);
  GA.setSelectPower(selectpower);
  GA.setCrossover(0.80);
  GA.setMutation(0.05,0.7,0.3);
  GA.setSwap(0.3);
  int cycle =1;
  for(int i=0;i<cycle;i++)
    {
      cout<<endl<<endl<<"cycle "<<i<<endl;
      GA.initialize();
      //GA.showAll();
      SelectByRoulette  sm(GA.getPopulation());
      SelectByRoulette *smodel=&sm;
      GA.setSelectModel(smodel);
      GA.optimize();
    }
}

void optimizepop()
{
  for(int i = 5; i<=100;i=i+2)
    { 
      GeneticAlgorithm GA(i,15);
      cout<<"___________________________________"<<endl;
      cout<<"population size:"<<GA.getPopulationSize()<<endl;
      int select = (i*0.8);
		
      //TODO:now we need to call setModel before initialzie,because we need to call evaluation in initialzie
      OptimizeWeightModel m;
      OptimizeWeightModel *model = &m;
      GA.setModel(model);
		    
      GA.setGeneration(2000);
      GA.setSelectPower(select);
      GA.setCrossover(0.9);
      GA.setMutation(0.05,0.5,0.3);
      GA.setSwap(0.75);
      int cycle =1;
      for(int i=0;i<cycle;i++)
	{
	  cout<<endl<<endl<<"cycle "<<i<<endl;
	  GA.initialize();
	  //GA.showAll();
	  SelectByRoulette  sm(GA.getPopulation());
	  SelectByRoulette *smodel=&sm;
	  GA.setSelectModel(smodel);
	  GA.optimize();
	}
    }
}

void optimizeselect(int popsize)
{
  cout<<"selectpower optimization under population size "<<popsize<<endl;
  for(int i = 5; i<=100;i=i+5)
    { 
      GeneticAlgorithm GA(popsize,15);
      cout<<"___________________________________"<<endl;
      cout<<"selectpower:"<<i<<endl;
      double s=(double)i/100;
      int select = (popsize*s);
		
      //TODO:now we need to call setModel before initialzie,because we need to call evaluation in initialzie
      OptimizeWeightModel m;
      OptimizeWeightModel *model = &m;
      GA.setModel(model);
		    
      GA.setGeneration(2000);
      GA.setSelectPower(select);
      GA.setCrossover(0.9);
      GA.setMutation(0.05,0.5,0.3);
      GA.setSwap(0.75);
      int cycle =1;
      for(int i=0;i<cycle;i++)
	{
	  cout<<endl<<endl<<"cycle "<<i<<endl;
	  GA.initialize();
	  //GA.showAll();
	  SelectByRoulette  sm(GA.getPopulation());
	  SelectByRoulette *smodel=&sm;
	  GA.setSelectModel(smodel);
	  GA.optimize();
	}
    }
}


void optimizecrossover(int popsize,int selectpower)
{
  cout<<"crossover optimization under population size "<<popsize<<", selector power "<<selectpower<<endl;
  for(int i = 0; i<=100;i=i+10)
    { 
      double prob = (double)i/100;
      GeneticAlgorithm GA(popsize,15);
      cout<<"___________________________________"<<endl;
      cout<<"crossover probability(%):"<<i<<endl;
		
      //TODO:now we need to call setModel before initialzie,because we need to call evaluation in initialzie
      OptimizeWeightModel m;
      OptimizeWeightModel *model = &m;
      GA.setModel(model);
		    
      GA.setGeneration(2000);
      GA.setSelectPower(selectpower);
      GA.setCrossover(prob);
      GA.setMutation(0.05,0.5,0.3);
      GA.setSwap(0.75);
      int cycle =1;
      for(int i=0;i<cycle;i++)
	{
	  cout<<endl<<endl<<"cycle "<<i<<endl;
	  GA.initialize();
	  //GA.showAll();
	  SelectByRoulette  sm(GA.getPopulation());
	  SelectByRoulette *smodel=&sm;
	  GA.setSelectModel(smodel);
	  GA.optimize();
	}
    }
}
void optimizeadd(int popsize,int selectpower)
{
  cout<<"add probability optimization under population size "<<popsize<<", selector power "<<selectpower<<endl;
  for(int i = 0; i<=40;i=i+5)
    { 
      double prob = (double)i/100;
      GeneticAlgorithm GA(popsize,15);
      cout<<"___________________________________"<<endl;
      cout<<"add probability(%):"<<i<<endl;
		
      //TODO:now we need to call setModel before initialzie,because we need to call evaluation in initialzie
      OptimizeWeightModel m;
      OptimizeWeightModel *model = &m;
      GA.setModel(model);
		    
      GA.setGeneration(2000);
      GA.setSelectPower(selectpower);
      GA.setCrossover(0.9);
      GA.setMutation(prob,0.5,0.3);
      GA.setSwap(0.75);
      int cycle =1;
      for(int i=0;i<cycle;i++)
	{
	  cout<<endl<<endl<<"cycle "<<i<<endl;
	  GA.initialize();
	  //GA.showAll();
	  SelectByRoulette  sm(GA.getPopulation());
	  SelectByRoulette *smodel=&sm;
	  GA.setSelectModel(smodel);
	  GA.optimize();
	}
    }
}

void optimizedelete(int popsize,int selectpower)
{
  cout<<"delete probability optimization under population size "<<popsize<<", selector power "<<selectpower<<endl;
  for(int i = 10; i<=80;i=i+5)
    { 
      double prob = (double)i/100;
      GeneticAlgorithm GA(popsize,15);
      cout<<"___________________________________"<<endl;
      cout<<"delete probability(%):"<<i<<endl;
		
      //TODO:now we need to call setModel before initialzie,because we need to call evaluation in initialzie
      OptimizeWeightModel m;
      OptimizeWeightModel *model = &m;
      GA.setModel(model);
		    
      GA.setGeneration(2000);
      GA.setSelectPower(selectpower);
      GA.setCrossover(0.9);
      GA.setMutation(0.05,prob,0.3);
      GA.setSwap(0.75);
      int cycle =1;
      for(int i=0;i<cycle;i++)
	{
	  cout<<endl<<endl<<"cycle "<<i<<endl;
	  GA.initialize();
	  //GA.showAll();
	  SelectByRoulette  sm(GA.getPopulation());
	  SelectByRoulette *smodel=&sm;
	  GA.setSelectModel(smodel);
	  GA.optimize();
	}
    }
}

void optimizealter(int popsize,int selectpower)
{
  cout<<"alter probability optimization under population size "<<popsize<<", selector power "<<selectpower<<endl;
  for(int i = 10; i<=90;i=i+5)
    { 
      double prob = (double)i/100;
      GeneticAlgorithm GA(popsize,15);
      cout<<"___________________________________"<<endl;
      cout<<"alter probability(%):"<<i<<endl;
		
      //TODO:now we need to call setModel before initialzie,because we need to call evaluation in initialzie
      OptimizeWeightModel m;
      OptimizeWeightModel *model = &m;
      GA.setModel(model);
		    
      GA.setGeneration(2000);
      GA.setSelectPower(selectpower);
      GA.setCrossover(0.9);
      GA.setMutation(0.05,0.5,prob);
      GA.setSwap(0.75);
      int cycle =1;
      for(int i=0;i<cycle;i++)
	{
	  cout<<endl<<endl<<"cycle "<<i<<endl;
	  GA.initialize();
	  //GA.showAll();
	  SelectByRoulette  sm(GA.getPopulation());
	  SelectByRoulette *smodel=&sm;
	  GA.setSelectModel(smodel);
	  GA.optimize();
	}
    }
}

void optimizeswap(int popsize,int selectpower)
{
  cout<<"swap probability optimization under population size "<<popsize<<", selector power "<<selectpower<<endl;
  for(int i = 10; i<=90;i=i+10)
    { 
      double prob = (double)i/100;
      GeneticAlgorithm GA(popsize,15);
      cout<<"___________________________________"<<endl;
      cout<<"swap probability(%):"<<i<<endl;
		
      //TODO:now we need to call setModel before initialzie,because we need to call evaluation in initialzie
      OptimizeWeightModel m;
      OptimizeWeightModel *model = &m;
      GA.setModel(model);
		    
      GA.setGeneration(2000);
      GA.setSelectPower(selectpower);
      GA.setCrossover(0.9);
      GA.setMutation(0.05,0.5,0.3);
      GA.setSwap(prob);
      int cycle =1;
      for(int i=0;i<cycle;i++)
	{
	  cout<<endl<<endl<<"cycle "<<i<<endl;
	  GA.initialize();
	  //GA.showAll();
	  SelectByRoulette  sm(GA.getPopulation());
	  SelectByRoulette *smodel=&sm;
	  GA.setSelectModel(smodel);
	  GA.optimize();
	}
    }
}
