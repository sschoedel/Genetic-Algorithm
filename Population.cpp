/**
 * Population.cpp
 *
*/

#include "Population.h"

Population::Population(unsigned long populationSize, unsigned long numGenesPerIndividual, double mutationRate,
                       double rand1, double rand2, bool usePDF)
{
  this->mutationRate = mutationRate;

  // request the size of the vector
  individuals.resize(populationSize);
  for (unsigned long i = 0; i < populationSize; i++)
  {
    individuals[i].setGenes(numGenesPerIndividual, mutationRate);
    individuals[i].setGeneRandomVars(rand1, rand2, usePDF);
  }
}

void Population::GenerateInitialPopulation()
{
  for (unsigned long i = 0; i < individuals.size(); i++)
  {
    individuals[i].randomizeGenes();
  }
}

void Population::NormalizeFitnessValues()
{
  // Sum fitness values from each individual
  double totalFitness = 0;
  for (unsigned long i = 0; i < individuals.size(); i++)
  {
    totalFitness += individuals[i].getFitnessVal();
  }

  normalizedFitnessValues.clear();
  // Normalize fitness value for each individual
  for (unsigned long i = 0; i < individuals.size(); i++)
  {
    if (totalFitness != 0)
    {
      double normalizedVal = individuals[i].getFitnessVal()/totalFitness;
      individuals[i].setNormalizedFitnessVal(normalizedVal);
      normalizedFitnessValues.push_back(normalizedVal);
    }
    else
    {
      individuals[i].setNormalizedFitnessVal(0);
      normalizedFitnessValues.push_back(0);
      normalizedFitnessValues.push_back(0);
    }
  }
}

void Population::Breed(unsigned long numGenes, double rand1, double rand2, bool usePDF)
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed);
  std::uniform_int_distribution<int> chooseGene{ 1, 2 };

  // Loop through all matched individuals and randomly choose which parent to make gene from
  for (unsigned long i = 0; i < remainingIndividuals.size(); i++)
  {
    Genes parent1Genes;
    parent1Genes.SetGenes(matchedIndividuals[i].first.getGenome().GetGenes());
    Genes parent2Genes;
    parent2Genes.SetGenes(matchedIndividuals[i].second.getGenome().GetGenes());
    Individual child(parent1Genes.GetGenes().size(), mutationRate, rand1, rand2, usePDF);
    vector<double> childGenome;
    for (unsigned long j = 0; j < parent1Genes.GetGenes().size(); j++)
    {
      // Randomly choose which parent from which to take gene
      int chooseParent1 = chooseGene(gen);
      if (chooseParent1 == 1) {
        childGenome.push_back(parent1Genes.GetGenes()[j]);
      }
      else {
        childGenome.push_back(parent2Genes.GetGenes()[j]);
      }

      // std::cout << "childgenome " << i << ": " << childGenome[j] << std::endl;
    }

//    if (i == 0) {
//    std::cout << "Child number " << i << std::endl;
//    for(unsigned int j = 0; j < childGenome.size(); j++)
//      std::cout << childGenome[j] << std::endl << std::endl;
//    }
    // Set child's genome
    child.setGenome(childGenome);
    // std::cout << "childGenome: ";
    // for (int i = 0; i < childGenome.size(); i++)
    // {
    //   std::cout << childGenome[i] << ", ";
    // }
    // std::cout << "" << std::endl;
    // std::cout << "GENOME FROM CHILD: ";
    // for (int i = 0; i < child.getGenome().GetGenes().size(); i++)
    // {
    //   std::cout << child.getGenome().GetGenes()[i] << ", ";
    // }
    // std::cout << "" << std::endl;

    // // Overwrite existing individuals with children
    // remainingIndividuals[i] = child;
  }
}

void Population::Crossover(unsigned long numGenesPerIndividual, double rand1, double rand2, bool usePDF)
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed);
  // Loop through all matched individuals and randomly choose which parent to make gene from
  for (unsigned long i = 0; i < remainingIndividuals.size(); i++)
  {
    Genes parent1Genes = matchedIndividuals[i].first.getGenome();
    Genes parent2Genes = matchedIndividuals[i].second.getGenome();
    Individual child(parent1Genes.GetGenes().size(), mutationRate, rand1, rand2, usePDF);
    vector<double> childGenome;
    childGenome.resize(parent1Genes.GetGenes().size());
    bool takeFromParent1 = true;
    for (unsigned long i = 0; i < parent1Genes.GetGenes().size(); i++)
    {
      // Randomly choose if we should swap between taking from parent 1 or parent 2
      std::uniform_int_distribution<int> chooseGene{ 1, 2 };
      int swap = chooseGene(gen);
      if (swap == 1) {
        takeFromParent1 = !takeFromParent1;
      }
      if (takeFromParent1) {
        childGenome[i] = parent1Genes.GetGenes()[i];
      }
      else {
        childGenome[i] = parent2Genes.GetGenes()[i];
      }
    }

    // Set child's genome
    child.getGenome().SetGenes(childGenome);

    // Overwrite existing individuals with children
    remainingIndividuals[i] = child;
  }
}

void Population::UpdateFitnessValues()
{
  for (unsigned long i = 0; i < individuals.size(); i++)
  {
    individuals[i].updateFitnessWithMATLAB();
    // Send genes from individual to MATLAB to compute fitness
  }
}

void Population::SortIndividualsByFitness()
{
  sort(individuals.begin(), individuals.end(), std::greater<Individual>());
}

void Population::SeparateElites(unsigned long numElites, unsigned long numAntiElites)
{
  elites.clear();
  antielites.clear();
  remainingIndividuals.clear();
  matchedIndividuals.clear();
  if (numElites > 0 && numElites <= individuals.size())
  {
    // Extract top numElites from population
    for (unsigned long i = 0; i < numElites; i++)
    {
      elites.push_back(individuals[i]);
    }
  }

  if (numAntiElites > 0 && numAntiElites <= (individuals.size() - numElites))
  {
    // Extract bottom numAntiElites from population
    for (unsigned long i = 0; i < numAntiElites; i++)
    {
      antielites.push_back(individuals[individuals.size() - i - 1]);
    }
  }

  // Populate new vector to track and operate on remaining individuals
  for (unsigned long i = numElites; i < individuals.size() - numAntiElites; i++)
  {
    remainingIndividuals.push_back(individuals[i]);
  }
}

void Population::MatchRemainingIndividuals()
{
  matchedIndividuals.clear();
  // std::cout << "normalizedFitnessValues.size(): " << normalizedFitnessValues.size() << std::endl;
  // std::cout << "normalinf fitneess values:" << std::endl;
  // for (double normVal : normalizedFitnessValues)
  // {
  //   std::cout << normVal << ", ";
  // }
  // std::cout << " " << std::endl;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine gen(seed);
  std::discrete_distribution<int> normalizedProbabilities(normalizedFitnessValues.begin(), normalizedFitnessValues.end());
  for (unsigned long i = 0; i < remainingIndividuals.size(); i++)
  {
    std::pair<Individual, Individual> mates;
    mates.first = remainingIndividuals[i];
    // Choose mate from normalized fitness value based distribution function
    // TODO: get new index if mateIndex equals current index. Shouldn't break algorithm this way but is not ideal for individuals to mate with themselves.
    int mateIndex = normalizedProbabilities(gen);
    mates.second = individuals[mateIndex]; // Should remaining individuals be able to mate with elites or antielites?
    matchedIndividuals.push_back(mates);
  }
}

void Population::AddAntiElites()
{
  individuals.clear();
  for (unsigned long i = 0; i < antielites.size(); i++)
  {
    remainingIndividuals.push_back(antielites[i]);
  }
}

void Population::AddElites()
{
  // std::cout << "ADD ELITES!" << std::endl;
  // std::cout << "remainingindividuals.size: " << remainingIndividuals.size() << ", elites.size: " << elites.size() << std::endl;
  individuals.clear();
  for (unsigned long i = 0; i < elites.size(); i++)
  {
    individuals.push_back(elites[i]);
  }
  for (unsigned long i = 0; i < remainingIndividuals.size(); i++)
  {
    individuals.push_back(remainingIndividuals[i]);
  }
  // std::cout << "individuals.size: " << individuals.size() << ", elites.size(): " << elites.size() << ", antielites.size() : " << antielites.size() << ", remaining.size: " << remainingIndividuals.size() << ", matchedINdividiauals.zize()" << matchedIndividuals.size() << std::endl;
}

void Population::Mutate()
{
  // Mutate all genes in existing population
  for (unsigned long i = 0; i < remainingIndividuals.size(); i++)
  {
    remainingIndividuals[i].mutateGenes();
  }
}

size_t Population::GetPopulationSize()
{
  return individuals.size();
}

size_t Population::GetRemainingPopulationSize()
{
  return remainingIndividuals.size();
}

Individual Population::GetIndividual(unsigned long index)
{
  return individuals[index];
}

Individual Population::GetRemainingIndividual(unsigned long index)
{
  return remainingIndividuals[index];
}
