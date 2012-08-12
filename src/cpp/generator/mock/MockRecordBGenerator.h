/*
 * Copyright 2010-2011 DIMA Research Group, TU Berlin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: Alexander Alexandrov <alexander.alexandrov@tu-berlin.de>
 */

#ifndef MOCKRECORDAGENERATOR_H_
#define MOCKRECORDAGENERATOR_H_

#include "generator/RandomSetGenerator.h"
#include "generator/mock/MockRecordBHydratorChain.h"
#include "record/mock/MockRecordB.h"

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// RecordGenerator specialization
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordBGenerator: public RandomSetGenerator<MockRecordB>
{
public:

    MockRecordBGenerator(const string& name, GeneratorConfig& config, NotificationCenter& notificationCenter) :
        RandomSetGenerator<MockRecordB>(name, config, notificationCenter)
    {
    }

    void prepare(Stage stage, const GeneratorPool& pool)
    {
        // call generator implementation
        RandomSetGenerator<MockRecordB>::prepare(stage, pool);

        if (stage.name() == name())
        {
            registerTask(new RandomSetDefaultGeneratingTask< MockRecordB > (*this, _config));
        }
    }

    MockRecordBHydratorChain hydratorChain(BaseHydratorChain::OperationMode opMode, RandomStream& random)
    {
        return MockRecordBHydratorChain(opMode, random, _config);
    }
};

}  // namespace Myriad

#endif /* MOCKRECORDAGENERATOR_H_ */
