/**
 * GeneticAlgorithm.cpp
 */

#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(bool useMaxIterations, unsigned long maxIterations,
                                  unsigned long numPopulationElitism, unsigned long numPopulationAntiElitism,
                                  unsigned long populationSize, unsigned long numGenesPerIndividual,
                                  double desiredFitnessLevel, double mutationRate, bool doBreeding,
                                  double rand1, double rand2, bool usePDF)
: population(populationSize, numGenesPerIndividual, mutationRate, rand1, rand2, usePDF)
{
  this->useMaxIterations = useMaxIterations;
  this->maxIterations = maxIterations;

  this->numPopulationElitism = numPopulationElitism;
  this->numPopulationAntiElitism = numPopulationAntiElitism;

  this->desiredFitnessLevel = desiredFitnessLevel;
  this->doBreeding = doBreeding;

  this->currentIteration = 0;

  this->numGenesPerIndividual = numGenesPerIndividual;
  this->mutationRate = mutationRate;
  this->rand1 = rand1;
  this->rand2 = rand2;
  this->usePDF = usePDF;

  // Start MATLAB engine synchronously
  // std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

  this->startTime = std::chrono::steady_clock::now();

  population.GenerateInitialPopulation();
}

bool GeneticAlgorithm::MaxIterationsReached() const
{
  return useMaxIterations && currentIteration >= maxIterations;
}

bool GeneticAlgorithm::FitnessLevelReached() const
{
  return EvaluateFitnessFunction() >= desiredFitnessLevel;
}

void GeneticAlgorithm::NextIteration()
{
  #ifdef DEBUG
  PrintIndividualInfo();
  std::cin.ignore();
  #endif

  // Step 1 - fitness evaluation
  population.UpdateFitnessValues();
  #ifdef DEBUG
  std::cout << "After update fitness values" << std::endl;
  PrintIndividualInfo();
  std::cin.ignore();
  #endif
  
  // Step 2 - sort by fitness
  population.SortIndividualsByFitness();
  #ifdef DEBUG
  std::cout << "After sort individuals by fitness" << std::endl;
  PrintIndividualInfo();
  std::cin.ignore();
  #endif

  // Step 3 - normalize scores
  population.NormalizeFitnessValues();
  #ifdef DEBUG
  std::cout << "After normalize fitness values" << std::endl;
  PrintIndividualInfo();
  std::cin.ignore();
  #endif

  // Step 4 - extract elites and antielites from population
  population.SeparateElites(this->numPopulationElitism, this->numPopulationAntiElitism);
  #ifdef DEBUG
  std::cout << "After separate elites" << std::endl;
  PrintIndividualInfo();
  std::cin.ignore();
  #endif

  // Step 5 - match individuals in remaining population
  population.MatchRemainingIndividuals();
  #ifdef DEBUG
  std::cout << "After match remaining individuals" << std::endl;
  PrintIndividualInfo();
  std::cin.ignore();
  #endif

  // Step 6 - perform breeding or crossover with matched individuals
  if(doBreeding)
    population.Breed(this->numGenesPerIndividual, this->rand1, this->rand2, this->usePDF);
  else
    population.Crossover(this->numGenesPerIndividual, this->rand1, this->rand2, this->usePDF);
  #ifdef DEBUG
  std::cout << "After breeding or crossover" << std::endl;
  PrintIndividualInfo();
  std::cin.ignore();
  #endif

  // Step 7 - Add antielites back into population
  population.AddAntiElites();
  #ifdef DEBUG
  std::cout << "After add anti elites" << std::endl;
  PrintIndividualInfo();
  std::cin.ignore();
  #endif

  // Step 8 - Mutate population
  population.Mutate();
  #ifdef DEBUG
  std::cout << "After mutate" << std::endl;
  PrintIndividualInfo();
  std::cin.ignore();
  #endif

  // Step 9 - Add elites back into population
  population.AddElites();
  #ifdef DEBUG
  std::cout << "After add elites" << std::endl;
  PrintIndividualInfo();
  std::cin.ignore();
  #endif

  currentIteration += 1;
}

void GeneticAlgorithm::EvaluatePopulation()
{
  for(unsigned long i = 0; i < population.GetPopulationSize(); i++)
  {
    Individual individual = population.GetIndividual(i);
  }
}

double GeneticAlgorithm::EvaluateFitnessFunction() const
{
  return 0.0;
}

void GeneticAlgorithm::PrintIterationInfo()
{
  executionTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
  vector<double> fitnessValues(population.GetPopulationSize());
  for (unsigned long i = 0; i < fitnessValues.size(); i++)
  {
    fitnessValues[i] = population.GetIndividual(i).getFitnessVal();
    // std::cout << "fitnessValues[i]: " << fitnessValues[i] << std::endl;
  }
  sort(fitnessValues.begin(), fitnessValues.end());
  std::cout << "generation: " << currentIteration << ", highest fitness: " << fitnessValues[fitnessValues.size()-1]
            << ", execution time (seconds): " << executionTimeMs/1000 << std::endl;
}

unsigned long GeneticAlgorithm::GetCurrentIteration() const
{
  return currentIteration;
}

bool GeneticAlgorithm::GetIsMaxIterationsEnabled() const
{
 return useMaxIterations;
}

void GeneticAlgorithm::PrintIndividualInfo()
{
  for (unsigned long i = 0; i < population.GetPopulationSize(); i++)
  {
    Individual ind = population.GetIndividual(i);
    std::cout << "original individual " << i << ": ";
    vector<double> genes = ind.getGenome().GetGenes();
    for (unsigned long j = 0; j < genes.size(); j++)
    {
      std::cout << genes[j] << ", ";
    }
    std::cout << "" << std::endl;
  }
  for (unsigned long i = 0; i < population.GetRemainingPopulationSize(); i++)
  {
    Individual ind = population.GetRemainingIndividual(i);
    std::cout << "remaining individual " << i << ": ";
    vector<double> genes = ind.getGenome().GetGenes();
    for (unsigned long j = 0; j < genes.size(); j++)
    {
      std::cout << genes[j] << ", ";
    }
    std::cout << "" << std::endl;
  }
  if (population.GetRemainingPopulationSize() == 0)
  {
    std::cout << "remaining population size == 0" << std::endl;
  }
}