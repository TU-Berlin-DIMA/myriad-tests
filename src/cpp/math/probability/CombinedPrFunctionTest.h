/*
 * CombinedPrFunctionTest.h
 *
 *  Created on: May 2, 2012
 *      Author: alexander
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

class CombinedPrFunctionTest: public TestFixture
{
public:
	void setUp()
	{
		srand(241231591);

	    size_t size = 512;
	    char path[size];

		_probability = new CombinedPrFunction("sample_chist", string(getcwd(path, size)) + "/build/config/sample_chist.distribution");

		_delta = 0.00001;
	}

	void tearDown()
	{
		delete _probability;
	}

	void testQHistogramPDF()
	{
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
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.07, y, _delta);
			}
			else if (x == 34)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _delta);
			}
			else if (x == 43)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.05, y, _delta);
			}
			else if (x == 44)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.05, y, _delta);
			}
			else if (x == 59)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.03, y, _delta);
			}
			else if (x < 29)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _delta);
			}
			else if (x < 34)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _delta);
			}
			else if (x < 43)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _delta);
			}
			else if (x < 59)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _delta);
			}
			else if (x < 90)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, _delta);
			}
			else
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
		}
	}

//	void testQHistogramCDF()
//	{
//		for (int i = 0; i < 100000; i++)
//		{
//			I64u x = rand() % 100;
//			Decimal y = _probability->cdf(x);
//
//			if (x < 10)
//			{
//				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
//			}
//			else if (x < 30)
//			{
//				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01*(x-10), y, _delta);
//			}
//			else if (x < 35)
//			{
//				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.20 + 0.04*(x-30), y, _delta);
//			}
//			else if (x < 45)
//			{
//				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.40 + 0.02*(x-35), y, _delta);
//			}
//			else if (x < 60)
//			{
//				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.60 + 0.0133333333333*(x-45), y, _delta);
//			}
//			else if (x < 90)
//			{
//				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.80 + 0.00666666666667*(x-60), y, _delta);
//			}
//			else
//			{
//				CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, y, 0.0);
//			}
//		}
//	}

	void testQHistogramSampling()
	{
//		std::cout << "sample for 0.210 is " << _probability->invcdf(0.210) << std::endl;
//		std::cout << "sample for 0.220 is " << _probability->invcdf(0.220) << std::endl;
//		std::cout << "sample for 0.280 is " << _probability->invcdf(0.280) << std::endl;

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

//			if (x == 0)
//			{
//				std::cout << "BAD SAMPLE 0 for y = " << y << std::endl;
//			}

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
		suite->addTest(new TestCaller<CombinedPrFunctionTest> ("testQHistogramPDF", &CombinedPrFunctionTest::testQHistogramPDF));
//		suite->addTest(new TestCaller<CombinedPrFunctionTest> ("testQHistogramCDF", &CombinedPrFunctionTest::testQHistogramCDF));
		suite->addTest(new TestCaller<CombinedPrFunctionTest> ("testQHistogramSampling", &CombinedPrFunctionTest::testQHistogramSampling));
		return suite;
	}

private:

	double _delta;

	/**
	 * Random seed for the stream.
	 */
	CombinedPrFunction* _probability;
};

} // namespace Myriad

#endif /* COMBINEDPRFUNCTIONTEST_H_ */
