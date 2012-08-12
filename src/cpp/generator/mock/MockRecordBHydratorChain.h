/*
 * This file is part of the myriad-tests package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef MOCKRECORDBHYDRATORCHAIN_H_
#define MOCKRECORDBHYDRATORCHAIN_H_

#include "generator/RandomSetGenerator.h"
#include "generator/mock/MockRecordBHydratorChain.h"
#include "record/mock/MockRecordB.h"

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// HydratorChain specialization
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

/**
 * Hydrator specialization for User.
 */
class MockRecordBHydratorChain : public HydratorChain<MockRecordB>
{
public:

    // value provider typedefs

	// field setter typedefs

    MockRecordBHydratorChain(OperationMode& opMode, RandomStream& random, GeneratorConfig& config) :
        HydratorChain<MockRecordB>(opMode, random),
        _logger(Logger::get("mock_record_b.hydrator"))
    {
    }

    virtual ~MockRecordBHydratorChain()
    {
    }

    /**
     * Object hydrating function.
     */
    void operator()(AutoPtr<MockRecordB> recordPtr) const
    {
        ensurePosition(recordPtr->genID());

        RandomStream& random = const_cast<MockRecordBHydratorChain*>(this)->_random;

        recordPtr->catalogSize(random(10, 45));
    }

protected:

    // value provider members

	// field setter members

    /**
     * Logger instance.
     */
    Logger& _logger;
};

}  // namespace Myriad

#endif /* MOCKRECORDBHYDRATORCHAIN_H_ */
