/*
 * This file is part of the myriad-dgen package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef PROBABILITYFUNCTIONSTEST_H_
#define PROBABILITYFUNCTIONSTEST_H_

#include "math/random/RandomStream.h"
#include "math/probability/Probability.h"
#include "math/probability/DiscreteDistribution.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <Poco/FileStream.h>
#include <Poco/Format.h>

#include <cmath>
#include <map>

using namespace CppUnit;
using namespace Poco;
using namespace std;

namespace Myriad {

class ProbabilityFunctionsTest: public TestFixture
{
public:
	void setUp()
	{
		_seed.v[0] = 0;
		_seed.v[1] = 0;
		_seed.v[2] = 0;
		_seed.v[3] = 0;
		_seed.v[4] = 0;
		_seed.v[5] = 0;

		_random.seed(_seed);

		_numberOfSamples = 1000000;
	}

	void tearDown()
	{
	}

	void testParetoSampling()
	{
		ParetoPrFunction pr(10, 0.0625);
		createHistogram(pr, "pr_test_pareto.dat", _numberOfSamples, 1);
	}

	void testNormalSampling()
	{
		NormalPrFunction pr(50, 50);
		createHistogram(pr, "pr_test_normal.dat", _numberOfSamples, 1);
	}

	void testBoundedNormalSampling()
	{
		BoundedNormalPrFunction pr(50, 50, 0, 200);
		createHistogram(pr, "pr_test_boundednormal.dat", _numberOfSamples, 1);
	}

	void testNormalGeneration()
	{
		I16u N = 100;
		I16u M = 1500;

		Decimal mean = N / 2.0;
		Decimal stddev = N / 6.0;

		NormalPrFunction d(mean, stddev);

		Decimal S = 0;
		Decimal weights[N];

		for (I16u i = 0; i < N; i++)
		{
			weights[i] = M * (d.cdf(i + 1) - d.cdf(i));
			S += weights[i];
		}

		Decimal scale = M / S;

		S = 0;
		Decimal balance = 0;

		for (I16u i = 0; i < N; i++)
		{
			Decimal dWeight = scale * weights[i] + balance;
			I32u iWeight = static_cast<I32u> (dWeight + 0.5);

			weights[i] = iWeight;
			S += iWeight;

			//			std::cout << "d[" << i << "] = " << iWeight << ";" << endl;

			// compute balance for the next step
			balance = dWeight - iWeight;
		}

		//		std::cout << "sum of weights from 0 to " << N << " is " << S << endl;
	}

//	void testParetoGeneration()
//	{
//		I64u N = 2000000; // number of products
//		I64u M = 5 * N; // number of product offers
//
//		Decimal xMin = 1;
//		Decimal alpha = 0.01;
//
//		ParetoPrFunction d(xMin, alpha);
//
//		Decimal xMax = d.invpdf(1 / static_cast<Decimal> (M));
//		Decimal step = (xMax - xMin) / static_cast<Decimal> (N);
//
//		std::cout << "xMax is " << xMax << std::endl;
//
//		Decimal S = 0;
//		I64u zeroes = 0;
//
//		Decimal scale = M / d.cdf(xMax);
//
//		for (I16u i = 0; i < N; i++)
//		{
//			Decimal weight = static_cast<I64u> (scale * d.cdf(xMin + (i + 1) * step) + 0.5) - static_cast<I64u> (scale * d.cdf(xMin + i * step) + 0.5);
//
//			S += weight;
//
//			if (weight == 0.0)
//			{
//				zeroes++;
//			}
//
//			std::cout << "weight at position " << i << " is " << weight << endl;
//		}
//
//		std::cout << "number of zeroes is " << (zeroes/static_cast<Decimal>(N)) << std::endl;
//		std::cout << "sum of weights from 0 to " << N << " is " << S << endl;
//	}

	void testParetoGeneration()
	{
		ParetoPrFunction f1(1, 0.5);
//		ParetoPrFunction f2(1, 2.0);
//		NormalPrFunction f3(0, 1.0);

		for (int m = 0; m < 1; m++)
		{
			FileOutputStream out(format("discreteprob%02d.dat", m+1), std::ios::trunc | std::ios::binary);

			I64u N = _random(100, 1000);
			I64u scale = _random(5, 25);
			I64u M = scale*N;
			I64u min = _random(0, 3);

			N = 5001;
			M = 25002;
			min = 1;

//			std::cout << "starting run " << m << " with parameters (" << N << "," << scale*N << "," << min << ")" << std::endl;

			DiscreteDistribution<ParetoPrFunction> d1(f1, N, M, min);
//			DiscreteDistribution<ParetoPrFunction> d2(f2, N, M, min);
//			DiscreteDistribution<NormalPrFunction> d3(f3, N, M, min);

			I64u S1 = 0;
//			I64u S2 = 0;
//			I64u S3 = 0;

			for (I64u i = 0; i < N; i++)
			{
				S1 += d1(i);
//				S2 += d2(i);
//				S3 += d3(i);

				out << format("%Lu, %Lu", i, d1(i)) << std::endl;
//				out << format("%Lu, %Lu, %Lu, %Lu", i, d1(i), d2(i), d3(i)) << std::endl;
			}

			CPPUNIT_ASSERT(M == S1);
//			CPPUNIT_ASSERT(M == S2);
//			CPPUNIT_ASSERT(M == S3);

			out.close();

			_random.nextChunk();
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("ProbabilityFunctionsTest");
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testParetoSampling", &ProbabilityFunctionsTest::testParetoSampling));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testNormalSampling", &ProbabilityFunctionsTest::testNormalSampling));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testBoundedNormalSampling", &ProbabilityFunctionsTest::testBoundedNormalSampling));
//		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testNormalGeneration", &ProbabilityFunctionsTest::testNormalGeneration));
//		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testParetoGeneration", &ProbabilityFunctionsTest::testParetoGeneration));
		return suite;
	}

private:

	template<typename Pr> void createHistogram(Pr& pr, const string& filename, const I32u numberOfSamples,  const Decimal histogramBlockWidth)
	{
		FileOutputStream out(filename, std::ios::trunc | std::ios::binary);
		map<I32, I32u> blocks;

		// sample N random points from the underlying d
		for (I32u i = 0; i < numberOfSamples; i++)
		{
			Decimal sample = pr.sample(_random());

			I32 b = floor(sample / histogramBlockWidth); // determine bucket

			if (blocks.find(b) == blocks.end())
			{
				blocks.insert(pair<I32, I32u> (b, 0));
			}

			blocks[b]++;
			_random.nextChunk();
		}

		Decimal pdfExp, pdfAct;
		for (map<I32, I32u>::iterator it = blocks.begin(); it != blocks.end(); ++it)
		{
			pdfAct = it->second / (histogramBlockWidth * numberOfSamples);
			pdfExp = pr.pdf(it->first * histogramBlockWidth);
			out << format("%f %f %f", it->first * histogramBlockWidth, pdfExp, pdfAct) << endl;
		}

		out.close();
	}


	/**
	 * Random seed for the stream.
	 */
	RandomStream::Seed _seed;

	/**
	 * RandomStream used in the sampling process.
	 */
	RandomStream _random;

	/**
	 * Number of samples per histogram.
	 */
	I32u _numberOfSamples;
};

} // namespace Myriad

#endif /* RANDOMSTREAMTEST_H_ */
