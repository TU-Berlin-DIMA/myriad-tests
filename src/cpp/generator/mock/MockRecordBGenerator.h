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

#include "generator/RandomSequenceGenerator.h"
#include "generator/mock/MockRecordBSetterChain.h"
#include "record/mock/MockRecordB.h"

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// RecordGenerator specialization
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordBGenerator: public RandomSequenceGenerator<MockRecordB>
{
public:

    MockRecordBGenerator(const string& name, GeneratorConfig& config, NotificationCenter& notificationCenter) :
        RandomSequenceGenerator<MockRecordB>(name, config, notificationCenter)
    {
    }

    void prepare(Stage stage, const GeneratorPool& pool)
    {
        // call generator implementation
        RandomSequenceGenerator<MockRecordB>::prepare(stage, pool);

        if (stage.name() == name())
        {
            registerTask(new PartitionedSequenceIteratorTask< MockRecordB > (*this, _config));
        }
    }

    MockRecordBSetterChain setterChain(BaseSetterChain::OperationMode opMode, RandomStream& random)
    {
        return MockRecordBSetterChain(opMode, random, _config);
    }
};

}  // namespace Myriad

#endif /* MOCKRECORDAGENERATOR_H_ */
