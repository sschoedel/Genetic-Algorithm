/**
 * Individual.cpp
 */

#include "Individual.h"

Individual::Individual()
{
  fitnessVal = 0.0;
}

Individual::Individual(unsigned long numGenes, double mutationRate,
                        double rand1, double rand2, bool usePDF)
: genes(numGenes, mutationRate, rand1, rand2, usePDF)
{
  fitnessVal = 0.0;
  this->numGenes = numGenes;
}

void Individual::randomizeGenes()
{
  this->genes.RandomizeGenes();
}

void Individual::mutateGenes()
{
  this->genes.MutateGenes();
}

void Individual::updateFitnessWithMATLAB()
{

    // Start MATLAB engine synchronously

    // Create MATLAB data array factory
    matlab::data::ArrayFactory factory;

    vector<double> genome = genes.GetGenes();

    std::vector<matlab::data::Array> args({
        factory.createScalar<double>(genome[0]),	// k(1)
        factory.createScalar<double>(genome[1]),	// k(2)
        factory.createScalar<double>(genome[2]),	// k(3)
        factory.createScalar<double>(genome[3]),	// k(4)
        factory.createScalar<double>(genome[4]),	// k(5)
        factory.createScalar<double>(genome[5]),	// k(6)
        factory.createScalar<double>(genome[6]),	// k(7)
        factory.createScalar<double>(genome[7]),	// k(8)
        factory.createScalar<double>(genome[8])	// k(9)
        });
    // for (int i=0; i<genome.size(); i++)
    // {
    //   std::cout << "genome[" << i << "]: " << genome[i] << std::endl;
    // }

    // Call MATLAB function and return result, the matlab script must be in the
    // same directory as the executable
    matlab::data::TypedArray<double> result = matlabPtr->feval(u"systemSim", args);
    this->fitnessVal = result[0];
    
    // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // std::default_random_engine gen(seed);
    // std::uniform_real_distribution<double> randomGeneValue(.1, .5);
    // this->fitnessVal = randomGeneValue(gen);
    // // std::cout << "Fitness value result: " << this->fitnessVal << std::endl;

}

void Individual::setGenes(unsigned long numGenes, double mutationRate)
{
  this->genes.ResetGenes(numGenes, mutationRate);
}

void Individual::setGenome(vector<double> genome)
{
  this->genes.SetGenes(genome);
}

void Individual::setGeneRandomVars(double rand1, double rand2, bool usePDF)
{
  this->genes.SetRandomValues(rand1, rand2, usePDF);
}

void Individual::setFitnessVal(double fitnessVal)
{
  this->fitnessVal = fitnessVal;
}

void Individual::setNormalizedFitnessVal(double normalizedFitnessVal)
{
  this->normalizedFitnessVal = normalizedFitnessVal;
}

double Individual::getFitnessVal() const
{
  return this->fitnessVal;
}

Genes Individual::getGenome()
{
  return this->genes;
}

bool Individual::operator > (const Individual& ind) const
{
    return (fitnessVal > ind.fitnessVal);
}
