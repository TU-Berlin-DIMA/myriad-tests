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
#include "math/probability/util/CombinedPrFunctionInput.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <Poco/AutoPtr.h>

#include <cmath>

using namespace CppUnit;
using namespace Poco;
using namespace std;

namespace Myriad {

template<typename T> class CombinedPrFunctionTest: public TestFixture
{
public:

	CombinedPrFunctionTest() :
		_basePath(""),
		_input(NULL),
		_probability(NULL)
	{
	    size_t size = 512;
	    char buffer[size];

	    _basePath = string(getcwd(buffer, size));
	}

	void setUp()
	{
		srand(241231591);

		try
		{
			CombinedPrFunctionInputFactory<T> functionInputFactory;
			_input = functionInputFactory.template getFunction<0>();
			_probability = new CombinedPrFunction<T>("sample.<T>.combined", _input->serialize());
		}
		catch(const DataException& e)
		{
			std::cout << e.message() << std::endl;
			throw e;
		}
	}

	void tearDown()
	{
		if (_input != NULL)
		{
			delete _input;
			_input = NULL;
		}

		if (_probability != NULL)
		{
			delete _probability;
			_probability = NULL;
		}
	}

	void testPDF()
	{
		T testRangeMin = _input->testRangeMin();
		T testRangeMax = _input->testRangeMax();
		int N = testRangeMax - testRangeMin;

		for (int i = 0; i < N; i++)
		{
			T x = testRangeMin + i;
			Decimal yExp = _input->pdf(x);
			Decimal yAct = _probability->pdf(x);

			CPPUNIT_ASSERT_DOUBLES_EQUAL(yExp, yAct, 0.0);
		}
	}

	void testCDF()
	{
		T testRangeMin = _input->testRangeMin();
		T testRangeMax = _input->testRangeMax();
		int N = testRangeMax - testRangeMin;

		for (int i = 0; i < N; i++)
		{
			T x = testRangeMin + i;
			Decimal yExp = _input->cdf(x);
			Decimal yAct = _probability->cdf(x);

			CPPUNIT_ASSERT_DOUBLES_EQUAL(yExp, yAct, 0.00001);
		}
	}

	void testSampling()
	{
		T testRangeMin = _input->testRangeMin();
		T testRangeMax = _input->testRangeMax();
		int N = testRangeMax - testRangeMin;
		I32u f[N], fNull = 0;

		// initialize the value frequency counters
		for (int x = 0; x < N; x++)
		{
			f[x] = 0;
		}

		// sample 10000 values from the histogram
		for (int i = 0; i < 10000; i++)
		{
			Decimal y = (rand() % 10000) / 10000.0;
			T x = _probability->invcdf(y);

			if (x != nullValue<T>())
			{
				f[x - testRangeMin]++;
			}
			else
			{
				fNull++;
			}
		}

		// verify the sampled value frequencies
		for (int i = 0; i < N; i++)
		{
			T x = testRangeMin + i;
			CPPUNIT_ASSERT_DOUBLES_EQUAL(_input->pdf(x), f[i]/10000.0, 0.01);
		}

		// verify sampled null values frequency
		CPPUNIT_ASSERT_DOUBLES_EQUAL(_input->pdf(nullValue<T>()), fNull/10000.0, 0.01);
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("CombinedPrFunctionTest");
		suite->addTest(new TestCaller<CombinedPrFunctionTest> ("testPDF", &CombinedPrFunctionTest<T>::testPDF));
		suite->addTest(new TestCaller<CombinedPrFunctionTest> ("testCDF", &CombinedPrFunctionTest<T>::testCDF));
		suite->addTest(new TestCaller<CombinedPrFunctionTest> ("testSampling", &CombinedPrFunctionTest<T>::testSampling));
		return suite;
	}

private:

	string _basePath;

	CombinedPrFunctionInput<T>* _input;
	CombinedPrFunction<T>* _probability;
};

} // namespace Myriad

#endif /* COMBINEDPRFUNCTIONTEST_H_ */
