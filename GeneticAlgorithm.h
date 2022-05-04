/**
 * GeneticAlgorithm.h
 *
 */

#include "Population.h"



class GeneticAlgorithm
{
public:
  GeneticAlgorithm(bool useMaxIterations, unsigned long maxIterations,
                    unsigned long numPopulationElitism, unsigned long numPopulationAntiElitism,
                    unsigned long populationSize, unsigned long numGenesPerIndividual,
                    double desiredFitnessLevel, double mutationRate, bool doBreeding,
                    double rand1, double rand2, bool usePDF);

  bool MaxIterationsReached() const;
  bool FitnessLevelReached() const;

  void NextIteration();

  void EvaluatePopulation();
  double EvaluateFitnessFunction() const;

  void PrintIterationInfo();
  void PrintIndividualInfo();

  bool GetIsMaxIterationsEnabled() const;
  unsigned long GetCurrentIteration() const;

private:

  bool doBreeding;  // true for breeding, false for Crossover

  bool useMaxIterations;
  unsigned long currentIteration;
  unsigned long maxIterations;

  unsigned long numPopulationElitism;
  unsigned long numPopulationAntiElitism;

  double desiredFitnessLevel;

  Population population;

  std::chrono::steady_clock::time_point startTime;
  double executionTimeMs;

  unsigned long numGenesPerIndividual;
  double mutationRate;
  double rand1;
  double rand2;
  bool usePDF;

};
