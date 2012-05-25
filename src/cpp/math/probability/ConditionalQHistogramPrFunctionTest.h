/*
 * QHistogramPrFunctionTest.h
 *
 *  Created on: May 2, 2012
 *      Author: alexander
 */

#ifndef CONDITIONALQHISTOGRAMPRFUNCTIONTEST_H_
#define CONDITIONALQHISTOGRAMPRFUNCTIONTEST_H_

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

class ConditionalQHistogramPrFunctionTest: public TestFixture
{
public:
	void setUp()
	{
		srand(241231591);

	    size_t size = 512;
	    char path[size];

		_probability = new ConditionalQHistogramPrFunction("sample_cond_qhist", string(getcwd(path, size)) + "/build/config/sample_cond_qhist.distribution");
	}

	void tearDown()
	{
		delete _probability;
	}

	void testConditionalQHistogramPDF()
	{
		for (int i = 0; i < 100000; i++)
		{
			I64u x1 = rand() % 100;
			I64u x2 = rand() % 10;

			Decimal y = _probability->pdf(x1, x2);

			if (x2 < 1)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
			else if (x2 < 2)
			{
				if (x1 < 10)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
				}
				else if (x1 < 30)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, y, 0.00001);
				}
				else if (x1 < 35)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.04, y, 0.00001);
				}
				else if (x1 < 45)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.02, y, 0.00001);
				}
				else if (x1 < 60)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0133333333333, y, 0.00001);
				}
				else if (x1 < 90)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00666666666667, y, 0.00001);
				}
				else
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
				}
			}
			else if (x2 < 6)
			{
				if (x1 < 10)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
				}
				else if (x1 < 20)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.025, y, 0.00001);
				}
				else if (x1 < 40)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0125, y, 0.00001);
				}
				else if (x1 < 60)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0125, y, 0.00001);
				}
				else if (x1 < 70)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.025, y, 0.0);
				}
				else
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
				}
			}
			else
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
		}
	}

	void testConditionalQHistogramCDF()
	{
		for (int i = 0; i < 100000; i++)
		{
			I64u x1 = rand() % 100;
			I64u x2 = rand() % 10;

			Decimal y = _probability->cdf(x1, x2);

			if (x2 < 1)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
			else if (x2 < 2)
			{
				if (x1 < 10)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
				}
				else if (x1 < 30)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01*(x1-10), y, 0.00001);
				}
				else if (x1 < 35)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.20 + 0.04*(x1-30), y, 0.00001);
				}
				else if (x1 < 45)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.40 + 0.02*(x1-35), y, 0.00001);
				}
				else if (x1 < 60)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.60 + 0.0133333333333*(x1-45), y, 0.00001);
				}
				else if (x1 < 90)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.80 + 0.00666666666667*(x1-60), y, 0.00001);
				}
				else
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, y, 0.0);
				}
			}
			else if (x2 < 6)
			{
				if (x1 < 10)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
				}
				else if (x1 < 20)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.025*(x1-10), y, 0.00001);
				}
				else if (x1 < 40)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25 + 0.0125*(x1-20), y, 0.00001);
				}
				else if (x1 < 60)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.50 + 0.0125*(x1-40), y, 0.00001);
				}
				else if (x1 < 70)
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(0.75 + 0.025*(x1-60), y, 0.0);
				}
				else
				{
					CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, y, 0.0);
				}
			}
			else
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, y, 0.0);
			}
		}
	}

	void testConditionalQHistogramSampling()
	{
		I32u f[10][100];
		I32u C[10];

		// initialize the value frequency counters
		for (int x2 = 0; x2 < 10; x2++)
		{
			C[x2] = 0;

			for (int x1 = 0; x1 < 100; x1++)
			{
				f[x2][x1] = 0;
			}
		}

		// sample 10000 values from the histogram
		for (int i = 0; i < 100000; i++)
		{
			Decimal y = (rand() % 10000) / 10000.0;

			I64u x2 = rand() % 5 + 1;
			I64u x1 = _probability->invcdf(y, x2);

			f[x2][x1]++;
			C[x2]++;
		}

		// verify the sampled value frequencies
		for (int x2 = 0; x2 < 10; x2++)
		{
			for (int x1 = 0; x1 < 100; x1++)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(_probability->pdf(x1, x2), C[x2] == 0 ? 0 : f[x2][x1]/static_cast<Decimal>(C[x2]), 0.01);
			}
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("ConditionalQHistogramPrFunctionTest");
		suite->addTest(new TestCaller<ConditionalQHistogramPrFunctionTest> ("testConditionalQHistogramPDF", &ConditionalQHistogramPrFunctionTest::testConditionalQHistogramPDF));
		suite->addTest(new TestCaller<ConditionalQHistogramPrFunctionTest> ("testConditionalQHistogramCDF", &ConditionalQHistogramPrFunctionTest::testConditionalQHistogramCDF));
		suite->addTest(new TestCaller<ConditionalQHistogramPrFunctionTest> ("testConditionalQHistogramSampling", &ConditionalQHistogramPrFunctionTest::testConditionalQHistogramSampling));
		return suite;
	}

private:

	/**
	 * Random seed for the stream.
	 */
	ConditionalQHistogramPrFunction* _probability;
};

} // namespace Myriad

#endif /* CONDITIONALQHISTOGRAMPRFUNCTIONTEST_H_ */
