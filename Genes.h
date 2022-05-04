/**
 * Genes.h
 */

#include <vector>
#include <random>
#include <iostream>
#include <chrono>

using std::vector;

class Genes
{
public:
  Genes();
  Genes(unsigned long numGenes, double mutationRate, double rand1, double rand2, bool usePDF);
  // Genes(const Genes& src);

  void CreateGenes(unsigned long numGenes, double mutationRate);
  void ResetGenes(unsigned long numGenes, double mutationRate);
  void RandomizeGenes();
  void MutateGenes();

  void SetGenes(vector<double> genes);
  void SetRandomValues(double rand1, double rand2, bool usePDF);
  vector<double> GetGenes() const;

private:
  vector<double> genes;
  bool genesCreated;
  double mutationRate;

  std::default_random_engine rand_generator;

  bool usePDF;
  double lowerLimit;
  double upperLimit;
  double mean;
  double stdDev;
};
