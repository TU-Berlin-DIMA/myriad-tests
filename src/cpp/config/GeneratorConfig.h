/*
 * This file is part of the myriad-demo package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
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

	virtual void configureSets(const AutoPtr<XML::Document>& doc)
	{
		bindStringSet(doc, "male-names", _maleNames);
		bindRecordSet<ProductClass> (doc, "product_classes", _productClasses);
	}

private:

	vector<String> _maleNames;
	vector<AutoPtr<ProductClass> > _productClasses;
};

} // namespace Myriad

#endif /* GENERATORCONFIG_H_ */
