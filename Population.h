/**
 * Population.h
 *
*/
#pragma once

#include <vector>
#include <functional>
#include "Individual.h"

class Population
{
public:
  Population(unsigned long populationSize, unsigned long numGenesPerIndividual, double mutationRate,
                       double rand1, double rand2, bool usePDF);

  void GenerateInitialPopulation();

  void Breed(unsigned long numGenesPerIndividual, double rand1, double rand2, bool usePDF);
  void Crossover(unsigned long numGenesPerIndividual, double rand1, double rand2, bool usePDF);

  void UpdateFitnessValues();
  void SortIndividualsByFitness();
  void NormalizeFitnessValues();
  void SeparateElites(unsigned long numElites, unsigned long numAntiElites);
  void MatchRemainingIndividuals();

  void AddAntiElites();
  void AddElites();
  void Mutate();

  size_t GetPopulationSize();
  size_t GetRemainingPopulationSize();

  Individual GetIndividual(unsigned long index);
  Individual GetRemainingIndividual(unsigned long index);

private:
  double mutationRate;

  vector<double> normalizedFitnessValues;

  vector<Individual> individuals;
  vector<Individual> elites;
  vector<Individual> antielites;
  vector<Individual> remainingIndividuals;
  vector<std::pair<Individual, Individual>> matchedIndividuals;

  std::default_random_engine generator;
};
