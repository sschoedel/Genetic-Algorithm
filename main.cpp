/**
 * main.cpp
 */

#include <iostream>
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "GeneticAlgorithm.h"

using namespace std;

int main()
{

  // matlab::data::ArrayFactory factory;
  // std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

  bool useMaxIterations = true;
  unsigned long maxIterations = 30;
  unsigned long numPopulationElitism = 1;
  unsigned long numPopulationAntiElitism = 5;
  unsigned long populationSize = 30;
  unsigned long numGenesPerIndividual = 9;
  unsigned long desiredFitnessLevel = 40;
  double mutationRate = 0.1;
  bool doBreeding = true;
  double lowerLimit = -3.0;
  double upperLimit = 3.0;
  bool usePDF = false;
  GeneticAlgorithm genAlg(useMaxIterations, maxIterations, numPopulationElitism, numPopulationAntiElitism,
                          populationSize, numGenesPerIndividual, desiredFitnessLevel, mutationRate, doBreeding,
                          lowerLimit, upperLimit, usePDF);
  if (genAlg.GetIsMaxIterationsEnabled())
  {
    while (!genAlg.MaxIterationsReached() && !genAlg.FitnessLevelReached())
    {
      genAlg.NextIteration();
      genAlg.PrintIterationInfo();
    }
  }

  return 0;
}
