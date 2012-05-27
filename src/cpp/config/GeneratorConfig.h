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

#ifndef GENERATORCONFIG_H_
#define GENERATORCONFIG_H_

#include "config/AbstractGeneratorConfig.h"
#include "record/ProductClass.h"

namespace Myriad {

class GeneratorConfig: public AbstractGeneratorConfig
{
public:

    GeneratorConfig(GeneratorPool& generatorPool) : AbstractGeneratorConfig(generatorPool)
    {
    }

	const vector<String>& maleNames()
	{
		return _maleNames;
	}

	const vector<AutoPtr<ProductClass> >& productClasses()
	{
		return _productClasses;
	}

protected:

	virtual void configureSets()
	{
		// FIXME
//		bindStringSet(doc, "male-names", _maleNames);
//		bindRecordSet<ProductClass> (doc, "product_classes", _productClasses);
	}

private:

	vector<String> _maleNames;
	vector<AutoPtr<ProductClass> > _productClasses;
};

} // namespace Myriad

#endif /* GENERATORCONFIG_H_ */
