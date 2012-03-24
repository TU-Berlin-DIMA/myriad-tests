#ifndef GENERATORSUBSYSTEM_H_
#define GENERATORSUBSYSTEM_H_

#include "generator/AbstractGeneratorSubsystem.h"

using namespace std;
using namespace Poco;

namespace Myriad {

class GeneratorSubsystem: public AbstractGeneratorSubsystem
{
public:

    GeneratorSubsystem(NotificationCenter& notificationCenter, const vector<bool>& executeStages) :
        AbstractGeneratorSubsystem(notificationCenter, executeStages)
    {
    }

    virtual ~GeneratorSubsystem()
    {
    }

protected:

    virtual void registerGenerators();
};

} // namespace Myriad

#endif /* GENERATORSUBSYSTEM_H_ */
