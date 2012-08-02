#include "generator/GeneratorSubsystem.h"

using namespace std;
using namespace Poco;

namespace Myriad {

// the initial stage ID should always be zero
I32u RecordGenerator::Stage::NEXT_STAGE_ID = 0;

// register the valid stages for the Myriad generator extension
RecordGenerator::StageList initList()
{
    RecordGenerator::StageList tmp;

    tmp.push_back(RecordGenerator::Stage("card"));

    return tmp;
}

const RecordGenerator::StageList RecordGenerator::STAGES(initList());

// register the record sequence generators
void GeneratorSubsystem::registerGenerators()
{
}

} // namespace Myriad
