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

	CombinedPrFunctionInput(double nullProbability, T testRangeMin, T testRangeMax) :
		_nullProbability(nullProbability),
		_testRange(testRangeMin, testRangeMax)
	{
		_domain.set(numericLimits<T>::max(), numericLimits<T>::min());
	}

	CombinedPrFunctionInput(const CombinedPrFunctionInput& o) :
		_nullProbability(o._nullProbability),
		_buckets(o._buckets),
		_exactValueBuckets(o._exactValueBuckets),
		_multiValueBuckets(o._multiValueBuckets),
		_pdf(o._pdf),
		_cdf(o._cdf),
		_domain(o._domain)
	{
	}

	T testRangeMin()
	{
		return _testRange.min();
	}

	T testRangeMax()
	{
		return _testRange.max();
	}

	CombinedPrFunctionInput<T>& add(double probability, const T& x)
	{
		size_t i = _buckets.size();

		_buckets.push_back(Interval<T>(x, x+1));
		_pdf.push_back(probability);
		_cdf.push_back(i > 0 ? _cdf[i-1] + probability : probability);

		_exactValueBuckets.push_back(i);

		_domain.set(min(_domain.min(), x), max(_domain.max(), x+1));

		return *this;
	}

	CombinedPrFunctionInput<T>& add(double probability, const T& l, const T& u)
	{
		size_t i = _buckets.size();

		_buckets.push_back(Interval<T>(l, u));
		_pdf.push_back(probability);
		_cdf.push_back(i > 0 ? _cdf[i-1] + probability : probability);

		_multiValueBuckets.push_back(i);

		_domain.set(min(_domain.min(), l), max(_domain.max(), u));

		return *this;
	}

	double pdf(const T& x)
	{
		if (nullValue<T>() == x)
		{
			return _nullProbability;
		}
		else if (_domain.contains(x))
		{
			size_t i = lookup(x);
			return _pdf[i]/static_cast<double>(_buckets[i].length());
		}
		else
		{
			return 0.0;
		}
	}

	double cdf(const T& x)
	{
		if (_domain.min() > x)
		{
			return 0.0;
		}
		else if (_domain.max() <= x)
		{
			return 1.0 - _nullProbability;
		}
		else
		{
			size_t i = lookup(x);
			if (i > 0)
			{
				return _cdf[i-1] + _pdf[i] * (x + 1 - _buckets[i].min())/static_cast<double>(_buckets[i].length());
			}
			else
			{
				return _pdf[i] * (x + 1 - _buckets[i].min())/static_cast<double>(_buckets[i].length());
			}
		}
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

	static CombinedPrFunctionInput<T>* factory()
	{
		// Unsupported base parameter type T
		throw std::exception();
	}

private:

	size_t lookup(const T& x)
	{
		// we assert that the value x is in the [_min, _max] range
		int min = 0;
		int max = _buckets.size() - 1;
		int mid = 0;

		// continue searching while [min, max] is not empty
		while (max >= min)
		{
			// calculate the midpoint for roughly equal partition //
			mid = (min + max) / 2;

			// determine which subarray to search
			if (_buckets[mid].max() <=  x)
			{
				// change min index to search upper subarray
				min = mid + 1;
			}
			else if (_buckets[mid].min() > x)
			{
				// change max index to search lower subarray
				max = mid - 1;
			}
			else
			{
				// key found at index mid
				return mid;
			}
		}

		return mid;
	}

	typedef Interval<T> TInterval;

	double _nullProbability;

	std::vector<TInterval> _buckets;

	std::vector<size_t> _exactValueBuckets;

	std::vector<size_t> _multiValueBuckets;

	std::vector<double> _pdf;

	std::vector<double> _cdf;

	TInterval _domain;

	TInterval _testRange;

	std::stringstream _in;
};

template<> CombinedPrFunctionInput<I64u>* CombinedPrFunctionInput<I64u>::factory()
{
	CombinedPrFunctionInput<I64u>* input = new CombinedPrFunctionInput<I64u>(0.04000, 0, 100);

	input->add(0.1900, 10, 29);
	input->add(0.0700, 29);
	input->add(0.0400, 30, 34);
	input->add(0.0100, 34);
	input->add(0.0800, 35, 43);
	input->add(0.0500, 43);
	input->add(0.0500, 44);
	input->add(0.1400, 45, 59);
	input->add(0.0300, 59);
	input->add(0.3000, 60, 90);

	return input;
}

template<> CombinedPrFunctionInput<Date>* CombinedPrFunctionInput<Date>::factory()
{
	CombinedPrFunctionInput<Date>* input = new CombinedPrFunctionInput<Date>(0.04000, Date("1992-04-01"), Date("1992-07-15"));

	input->add(0.1900, "1992-04-10", "1992-04-29");
	input->add(0.0700, "1992-04-29");
	input->add(0.0400, "1992-04-30", "1992-05-04");
	input->add(0.0100, "1992-05-04");
	input->add(0.0800, "1992-05-05", "1992-05-13");
	input->add(0.0500, "1992-05-13");
	input->add(0.0500, "1992-05-14");
	input->add(0.1400, "1992-05-15", "1992-05-29");
	input->add(0.0300, "1992-05-29");
	input->add(0.3000, "1992-05-30", "1992-06-29");

	return input;
}

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

		_input = CombinedPrFunctionInput<T>::factory();
		_probability = new CombinedPrFunction<T>("sample.<T>.combined", _input->serialize());
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
