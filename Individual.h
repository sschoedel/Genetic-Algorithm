/**
 * Individual.h
 *
*/

#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "Genes.h"

using namespace matlab::engine;
static std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

class Individual
{
public:
  Individual();
  Individual(unsigned long numGenes, double mutationRate, double rand1, double rand2, bool PDF);

  void randomizeGenes();

  void updateFitnessWithMATLAB();
  void mutateGenes();

  void setFitnessVal(double fitnessVal);
  void setNormalizedFitnessVal(double normalizedFitnessVal);
  void setGenes(unsigned long numGenes, double mutationRate);
  void setGenome(vector<double> genome);
  void setGeneRandomVars(double rand1, double rand2, bool usePDF);

  double getFitnessVal() const;
  Genes getGenome();


  bool operator > (const Individual& ind) const;

private:
  Genes genes;
  unsigned long numGenes;
  double fitnessVal;
  double normalizedFitnessVal;
};
