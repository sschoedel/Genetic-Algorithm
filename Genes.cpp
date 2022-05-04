/**
 * Genes.cpp
 */

#include "Genes.h"

Genes::Genes()
{
  this->genesCreated = false;
  this->usePDF = false;
}

Genes::Genes(unsigned long numGenes, double mutationRate, double random1, double random2, bool usePDF)
{
  this->mean = random1;
  this->stdDev = random2;
  this->lowerLimit = random1;
  this->upperLimit = random2;
  this->usePDF = usePDF;
  CreateGenes(numGenes, mutationRate);
}

/*
Genes::Genes(const Genes& src)
{
  this->genesCreated = src.genesCreated;
  this->mutationRate = src.mutationRate;
  this->usePDF = src.usePDF;
  this->lowerLimit = src.lowerLimit;
  this->upperLimit = src.upperLimit;
  this->mean = src.mean;
  this->stdDev = src.stdDev;

  this->genes.clear();
  for(unsigned long i  = 0; i < src.genes.size(); i++)
    this->genes.push_back(src.genes[i]);
}
*/

void Genes::ResetGenes(unsigned long numGenes, double mutationRate)
{
  // delete all genes
  genes.clear();

  // create genes again
  CreateGenes(numGenes, mutationRate);
}

void Genes::CreateGenes(unsigned long numGenes, double mutationRate)
{
  // set gene length
  genes.reserve(numGenes);

  for(unsigned long i = 0; i < numGenes; i++)
    genes.push_back(0.0);

  this->genesCreated = true;
}

void Genes::RandomizeGenes()
{
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());
  for (unsigned long i = 0; i < genes.size(); i++)
  {
    if (usePDF)
    {
      std::cout << "using pdf placeholder" << std::endl;
    }
    else
    {
      // std::cout << "lower limit: " << this->lowerLimit << ", upper limit: " << this->upperLimit << std::endl;
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine gen(seed);
      std::uniform_real_distribution<double> randomGeneValue(this->lowerLimit, this->upperLimit);
      genes[i] = randomGeneValue(gen);
      // genes[i] = 0.1;
      std::cout << "gene " << i << ": " << genes[i] << std::endl;
    }
  }
}

void Genes::MutateGenes()
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed);
  std::uniform_real_distribution<double> mutationDist {0, 1};
  std::uniform_real_distribution<double> randomGeneValue(this->lowerLimit, this->upperLimit);
  // TODO: FIX DIST
  for (unsigned long i = 0; i < genes.size(); i++)
  {
    if (mutationDist(gen) < mutationRate)
    {
      // Choose new gene value
      genes[i] = randomGeneValue(gen);
    }
  }
}

void Genes::SetGenes(vector<double> genes)
{
  // TODO: make more efficient!!!
  this->genes = genes;
}

void Genes::SetRandomValues(double rand1, double rand2, bool usePDF)
{
  this->mean = rand1;
  this->stdDev = rand2;
  this->lowerLimit = rand1;
  this->upperLimit = rand2;
  this->usePDF = usePDF;
}

vector<double> Genes::GetGenes() const
{
  // TODO: make more efficient
  return genes;
}
