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

#ifndef COMBINEDPRFUNCTIONTEST_H_
#define COMBINEDPRFUNCTIONTEST_H_

#include "math/probability/CombinedPrFunction.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <Poco/AutoPtr.h>

#include <cmath>

using namespace CppUnit;
using namespace Poco;
using namespace std;

namespace Myriad {


class CombinedPrFunctionInput
{
};

class CombinedPrFunctionTest: public TestFixture
{
public:

	CombinedPrFunctionTest() :
		_defaultDelta(0.00001),
		_basePath(""),
		_probability(NULL)
	{
	    size_t size = 512;
	    char buffer[size];

	    _basePath = string(getcwd(buffer, size));
	}

	void setUp()
	{
		srand(241231591);
		_probability = NULL;
	}

	void tearDown()
	{
		if (_probability != NULL)
		{
			delete _probability;
			_probability = NULL;
		}
	}

	void testCombinedPrFunctionPDF()
	{
		_probability = new CombinedPrFunction("sample.I64u.combined", _basePath + "/build/config/sample.I64u.combined.distribution");

		for (int i = 0; i < 100000; i++)
		{
			I64u x = rand() % 100;
			Decimal y = _probability->pdf(x);

			if (x < 10)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
			else if (x == 29)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.07, y, _defaultDelta);
			}
			else if (x == 34)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _defaultDelta);
			}
			else if (x == 43)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.05, y, _defaultDelta);
			}
			else if (x == 44)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.05, y, _defaultDelta);
			}
			else if (x == 59)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.03, y, _defaultDelta);
			}
			else if (x < 29)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _defaultDelta);
			}
			else if (x < 34)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _defaultDelta);
			}
			else if (x < 43)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _defaultDelta);
			}
			else if (x < 59)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _defaultDelta);
			}
			else if (x < 90)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _defaultDelta);
			}
			else
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
		}
	}

	void testCombinedPrFunctionCDF()
	{
		_probability = new CombinedPrFunction("sample.I64u.combined", _basePath + "/build/config/sample.I64u.combined.distribution");

		for (int i = 0; i < 100000; i++)
		{
			I64u x = rand() % 100;
			Decimal y = _probability->cdf(x);

			if (x < 10)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
			else if (x < 29)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01*(x-9), y, _defaultDelta);
			}
			else if (x == 29)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.26, y, _defaultDelta);
			}
			else if (x < 34)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.26 + 0.01*(x-29), y, _defaultDelta);
			}
			else if (x == 34)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.31, y, _defaultDelta);
			}
			else if (x < 43)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.31 + 0.01*(x-34), y, _defaultDelta);
			}
			else if (x == 43)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.44, y, _defaultDelta);
			}
			else if (x == 44)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.49, y, _defaultDelta);
			}
			else if (x < 59)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.49 + 0.01*(x-44), y, _defaultDelta);
			}
			else if (x == 59)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.66, y, _defaultDelta);
			}
			else if (x < 90)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.66 + 0.01*(x-59), y, _defaultDelta);
			}
			else
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.96, y, 0.0);
			}
		}
	}

	void testCombinedPrFunctionSampling()
	{
		_probability = new CombinedPrFunction("sample.I64u.combined", _basePath + "/build/config/sample.I64u.combined.distribution");

		I32u f[100], fNull = 0;

		// initialize the value frequency counters
		for (int x = 0; x < 100; x++)
		{
			f[x] = 0;
		}

		// sample 10000 values from the histogram
		for (int i = 0; i < 10000; i++)
		{
			Decimal y = (rand() % 10000) / 10000.0;
			I64u x = _probability->invcdf(y);

			if (x != nullValue<I64u>())
			{
				f[x]++;
			}
			else
			{
				fNull++;
			}
		}

		// verify the sampled value frequencies
		for (int x = 0; x < 100; x++)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(_probability->pdf(x), f[x]/10000.0, 0.01);
		}

		// verify sampled null values frequency
		CPPUNIT_ASSERT_DOUBLES_EQUAL(_probability->pdf(nullValue<I64u>()), fNull/10000.0, 0.01);
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("CombinedPrFunctionTest");
		suite->addTest(new TestCaller<CombinedPrFunctionTest> ("testCombinedPrFunctionPDF", &CombinedPrFunctionTest::testCombinedPrFunctionPDF));
		suite->addTest(new TestCaller<CombinedPrFunctionTest> ("testCombinedPrFunctionCDF", &CombinedPrFunctionTest::testCombinedPrFunctionCDF));
		suite->addTest(new TestCaller<CombinedPrFunctionTest> ("testCombinedPrFunctionSampling", &CombinedPrFunctionTest::testCombinedPrFunctionSampling));
		return suite;
	}

private:

	double _defaultDelta;
	string _basePath;

	CombinedPrFunction* _probability;
};

} // namespace Myriad

#endif /* COMBINEDPRFUNCTIONTEST_H_ */
