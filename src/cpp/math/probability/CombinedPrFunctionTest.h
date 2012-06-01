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


template<typename T> class CombinedPrFunctionInput
{
public:

	CombinedPrFunctionInput(double nullProbability) :
		_nullProbability(nullProbability)
	{
		_interval.set(numeric_limits<T>::max(), numeric_limits<T>::min());
	}

	CombinedPrFunctionInput(const CombinedPrFunctionInput& o) :
		_nullProbability(o._nullProbability),
		_buckets(o._buckets),
		_exactValueBuckets(o._exactValueBuckets),
		_multiValueBuckets(o._multiValueBuckets),
		_pdf(o._pdf),
		_cdf(o._cdf),
		_interval(o._interval)
	{
	}

	CombinedPrFunctionInput<T>& addExactValue(double probability, const T& x)
	{
		size_t i = _buckets.size();

		_buckets.push_back(Interval<T>(x, x+1));
		_pdf.push_back(probability);
		_cdf.push_back(i > 0 ? _cdf[i-1] + probability : probability);

		_exactValueBuckets.push_back(i);

		_interval.set(min(_interval.min(), x), max(_interval.max(), x+1));

		return *this;
	}

	CombinedPrFunctionInput<T>& addBucket(double probability, const T& l, const T& u)
	{
		size_t i = _buckets.size();

		_buckets.push_back(Interval<T>(l, u));
		_pdf.push_back(probability);
		_cdf.push_back(i > 0 ? _cdf[i-1] + probability : probability);

		_multiValueBuckets.push_back(i);

		_interval.set(min(_interval.min(), l), max(_interval.max(), u));

		return *this;
	}

	std::istream& serialize()
	{
		_in.clear();

		_in << "# numberofexactvals: " << _exactValueBuckets.size() << "\n"
			<< "# numberofbins: "      << _multiValueBuckets.size()  << "\n"
			<< "# nullprobability: "   << _nullProbability           << "\n";

		for (size_t j = 0; j < _exactValueBuckets.size(); j++)
		{
			size_t i = _exactValueBuckets[j];
			_in << _pdf[i] << "\t" << _buckets[i].min() << "\n";
		}

		for (size_t j = 0; j < _multiValueBuckets.size(); j++)
		{
			size_t i = _multiValueBuckets[j];
			_in << _pdf[i] << "\t" << _buckets[i].min() << "\t" << _buckets[i].max() << "\n";
		}

		return _in;
	}

private:

	typedef Interval<T> TInterval;

	double _nullProbability;

	std::vector<TInterval> _buckets;

	std::vector<size_t> _exactValueBuckets;

	std::vector<size_t> _multiValueBuckets;

	std::vector<double> _pdf;

	std::vector<double> _cdf;

	TInterval _interval;

	std::stringstream _in;
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
		CombinedPrFunctionInput<I64u> prFunctionInput(0.04000);
		prFunctionInput.addExactValue(0.0700, 29);
		prFunctionInput.addExactValue(0.0100, 34);
		prFunctionInput.addExactValue(0.0500, 43);
		prFunctionInput.addExactValue(0.0500, 44);
		prFunctionInput.addExactValue(0.0300, 59);
		prFunctionInput.addBucket(0.1900, 10, 29);
		prFunctionInput.addBucket(0.0400, 30, 34);
		prFunctionInput.addBucket(0.0800, 35, 43);
		prFunctionInput.addBucket(0.1400, 45, 59);
		prFunctionInput.addBucket(0.3000, 60, 90);

		_probability = new CombinedPrFunction("sample.I64u.combined", prFunctionInput.serialize());

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

	template <typename T> CombinedPrFunctionInput<T> prFunctionInputFactory()
	{
		throw std::exception("Unsupported base parameter type T");
	}

	double _defaultDelta;
	string _basePath;

	CombinedPrFunction* _probability;
};

template<> CombinedPrFunctionInput<I64u> CombinedPrFunctionTest::prFunctionInputFactory<I64u>()
{
	CombinedPrFunctionInput<I64u> prFunctionInput(0.04000);

	prFunctionInput.addExactValue(0.0700, 29);
	prFunctionInput.addExactValue(0.0100, 34);
	prFunctionInput.addExactValue(0.0500, 43);
	prFunctionInput.addExactValue(0.0500, 44);
	prFunctionInput.addExactValue(0.0300, 59);
	prFunctionInput.addBucket(0.1900, 10, 29);
	prFunctionInput.addBucket(0.0400, 30, 34);
	prFunctionInput.addBucket(0.0800, 35, 43);
	prFunctionInput.addBucket(0.1400, 45, 59);
	prFunctionInput.addBucket(0.3000, 60, 90);

	return prFunctionInput;
}

} // namespace Myriad

#endif /* COMBINEDPRFUNCTIONTEST_H_ */
