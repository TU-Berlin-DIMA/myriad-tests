/*
 * QHistogramPrFunctionTest.h
 *
 *  Created on: May 2, 2012
 *      Author: alexander
 */

#ifndef QHISTOGRAMPRFUNCTIONTEST_H_
#define QHISTOGRAMPRFUNCTIONTEST_H_

#include "math/probability/QHistogramPrFunction.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <Poco/AutoPtr.h>

#include <cmath>

using namespace CppUnit;
using namespace Poco;
using namespace std;

namespace Myriad {

class QHistogramPrFunctionTest: public TestFixture
{
public:
	void setUp()
	{
		srand(241231591);

	    size_t size = 512;
	    char path[size];

		_probability = new QHistogramPrFunction("sample_qhist", string(getcwd(path, size)) + "/build/config/sample_qhist.qhistogram");
	}

	void tearDown()
	{
		delete _probability;
	}

	void testQHistogramPDF()
	{

		for (int i = 0; i < 10000; i++)
		{
			I64u x = rand() % 100;
			Decimal y = _probability->pdf(x);

			if (x < 10)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
			else if (x < 30)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, 0.00001);
			}
			else if (x < 35)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.04, y, 0.00001);
			}
			else if (x < 45)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.02, y, 0.00001);
			}
			else if (x < 60)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0133333333333, y, 0.00001);
			}
			else if (x < 90)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00666666666667, y, 0.00001);
			}
			else
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
		}
	}

	void testQHistogramCDF()
	{
		for (int i = 0; i < 10000; i++)
		{
			I64u x = rand() % 100;
			Decimal y = _probability->cdf(x);

			if (x < 10)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
			else if (x < 30)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01*(x-10), y, 0.00001);
			}
			else if (x < 35)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.20 + 0.04*(x-30), y, 0.00001);
			}
			else if (x < 45)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.40 + 0.02*(x-35), y, 0.00001);
			}
			else if (x < 60)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.60 + 0.0133333333333*(x-45), y, 0.00001);
			}
			else if (x < 90)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.80 + 0.00666666666667*(x-60), y, 0.00001);
			}
			else
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, y, 0.0);
			}
		}
	}

	void testQHistogramSampling()
	{
		I32u f[100];

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

			f[x]++;
		}

		// verify the sampled value frequencies
		for (int x = 0; x < 100; x++)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(_probability->pdf(x), f[x]/10000.0, 0.01);
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("QHistogramPrFunctionTest");
		suite->addTest(new TestCaller<QHistogramPrFunctionTest> ("testQHistogramPDF", &QHistogramPrFunctionTest::testQHistogramPDF));
		suite->addTest(new TestCaller<QHistogramPrFunctionTest> ("testQHistogramCDF", &QHistogramPrFunctionTest::testQHistogramCDF));
		suite->addTest(new TestCaller<QHistogramPrFunctionTest> ("testQHistogramSampling", &QHistogramPrFunctionTest::testQHistogramSampling));
		return suite;
	}

private:

	/**
	 * Random seed for the stream.
	 */
	QHistogramPrFunction* _probability;
};

} // namespace Myriad

#endif /* QHISTOGRAMPRFUNCTIONTEST_H_ */
