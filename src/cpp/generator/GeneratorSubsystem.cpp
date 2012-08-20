#include "generator/GeneratorSubsystem.h"

using namespace std;
using namespace Poco;

namespace Myriad {

// the initial stage ID should always be zero
I32u AbstractSequenceGenerator::Stage::NEXT_STAGE_ID = 0;

// register the valid stages for the Myriad generator extension
AbstractSequenceGenerator::StageList initList()
{
    AbstractSequenceGenerator::StageList tmp;

    tmp.push_back(AbstractSequenceGenerator::Stage("card"));

    return tmp;
}

const AbstractSequenceGenerator::StageList AbstractSequenceGenerator::STAGES(initList());

// register the record sequence generators
void GeneratorSubsystem::registerGenerators()
{
}

} // namespace Myriad
