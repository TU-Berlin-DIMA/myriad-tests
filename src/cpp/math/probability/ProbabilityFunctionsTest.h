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

#include <ctime>

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

		_random.seed(_seed);
//## compile prototype with different sample sizes
//#	100 MB 	->	8095855 rows
//#	1 GB	->	80958549 rows
//#	10 GB 	-> 	809585492 rows
//#	100 GB	->	8095854922
//#	1 TB	->	80958549222
//
		_numberOfSamples = 10000; //8095855;
	}

	void tearDown()
	{
	}

	void testParetoSampling()
	{
		ParetoPrFunction pr(10, 0.0625);
		createHistogram(pr, "/tmp/pr_test_pareto.dat", _numberOfSamples, 1);
	}

	void testNormalSampling()
	{
		NormalPrFunction pr(50, 50);
		createHistogram(pr, "/tmp/pr_test_normal.dat", _numberOfSamples, 1);
	}

	void testBoundedNormalSampling()
	{
		BoundedNormalPrFunction pr(50, 50, 0, 200);
		createHistogram(pr, "/tmp/pr_test_boundednormal.dat", _numberOfSamples, 1);
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

	void testJointProbabilitySampling(){
		String _basePath = "/home/mho/TU-Berlin-DIMA/myriad-tests-project"; // getcwd(buffer);
		// FIXME use stream instead of local distribution file
		String _path = _basePath + "/../test/q2d_hist_4.distribution";
		JointPrFunction<MyriadTuple<I64u, I64u> > pr("", _path);
		pr.setSampleSize(this->_numberOfSamples);

		// bin edges for each dimension
		const I32u numBins = 4;
		// [a,b,c] <=> [[a,b), [b, c)]
		const I32 bins[][5] = {{14534925, 14545517, 14555497, 14560465, 14568027}, {17019, 26713, 34321, 42402, 49915}};
		Decimal freq_ref[][4] = {{0.2497,0.2510,0.2497,0.2497}, {0.2497,0.2510,0.2497,0.2497}};
		Decimal freq_res[2][4] = {0};

		clock_t t_start = clock();
		createHistogram2(pr, "/tmp/pr_test_joint_q2d.dat", _numberOfSamples, bins, numBins, freq_res);
		clock_t t_end = clock();

		double elapsed_secs = double(t_end - t_start) / CLOCKS_PER_SEC;



		double err = 0;
		for (I32u i = 0; i < numBins; ++i){
			err += (freq_ref[0][i] - freq_res[0][i]) * (freq_ref[0][i] - freq_res[0][i]);
			err += (freq_ref[1][i] - freq_res[1][i]) * (freq_ref[1][i] - freq_res[1][i]);
		}
		err /= 2*numBins;
		cout << "err = " << err << " with sample size = " << this->_numberOfSamples << ", elapsed time [s] = "<< elapsed_secs << endl;

	}

//	void testParetoGeneration()
//	{
//		ParetoPrFunction f1(1, 0.5);
////		ParetoPrFunction f2(1, 2.0);
////		NormalPrFunction f3(0, 1.0);
//
//		for (int m = 0; m < 1; m++)
//		{
//			FileOutputStream out(format("discreteprob%02d.dat", m+1), std::ios::trunc | std::ios::binary);
//
//			I64u N = _random(100, 1000);
//			I64u scale = _random(5, 25);
//			I64u M = scale*N;
//			I64u min = _random(0, 3);
//
//			N = 5001;
//			M = 25002;
//			min = 1;
//
////			std::cout << "starting run " << m << " with parameters (" << N << "," << scale*N << "," << min << ")" << std::endl;
//
////			DiscreteDistribution<ParetoPrFunction> d1(f1, N, M, min);
////			DiscreteDistribution<ParetoPrFunction> d2(f2, N, M, min);
////			DiscreteDistribution<NormalPrFunction> d3(f3, N, M, min);
//
//			I64u S1 = 0;
////			I64u S2 = 0;
////			I64u S3 = 0;
//
//			for (I64u i = 0; i < N; i++)
//			{
//				S1 += d1(i);
////				S2 += d2(i);
////				S3 += d3(i);
//
////				out << format("%Lu, %Lu", i, d1(i)) << std::endl;
////				out << format("%Lu, %Lu, %Lu, %Lu", i, d1(i), d2(i), d3(i)) << std::endl;
//			}
//
//			CPPUNIT_ASSERT(M == S1);
////			CPPUNIT_ASSERT(M == S2);
////			CPPUNIT_ASSERT(M == S3);
//
//			out.close();
//
//			_random.nextChunk();
//		}
//	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("ProbabilityFunctionsTest");
//		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testParetoSampling", &ProbabilityFunctionsTest::testParetoSampling));
//		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testNormalSampling", &ProbabilityFunctionsTest::testNormalSampling));
//		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testBoundedNormalSampling", &ProbabilityFunctionsTest::testBoundedNormalSampling));
//		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testNormalGeneration", &ProbabilityFunctionsTest::testNormalGeneration));
//		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testParetoGeneration", &ProbabilityFunctionsTest::testParetoGeneration));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling", &ProbabilityFunctionsTest::testJointProbabilitySampling));
		return suite;
	}

private:

	template<typename Pr> void createHistogram(Pr& pr, const string& filename, const I32u numberOfSamples,  const Decimal histogramBlockWidth)
	{
		FileOutputStream out(filename, std::ios::trunc | std::ios::binary);
		map<I32, I32u> blocks;

		// sample N random points from the underlying distribution
		for (I32u i = 0; i < numberOfSamples; i++)
		{
			//const I64u GenID, const I64u sampleSize
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

	// histograms dimension-wise (2D) for joint probability function
	template<typename Pr> void createHistogram2(Pr& pr, const string& filename, const I32u numberOfSamples,  const I32 bins[][5], const I32u numBins, double freq[][4])
		{
			cout << "createHisto 1" << endl;
			FileOutputStream out(filename, std::ios::trunc | std::ios::binary);

			cout << "createHisto 2" << endl;
			I32u b_fst, b_snd;;

			I32u blocks[2][4] = {};
			// sample N random points from the underlying d
			for (I32u i = 0; i < numberOfSamples; i++)
			{
				//const I64u GenID, const I64u sampleSize
				MyriadTuple<I64u, I64u> sample = pr.sample(i, this->_numberOfSamples);
				b_fst = b_snd = 0;
				for (I32u j = 1; j < numBins; ++j){
					if (bins[0][j] <= sample.getFirst() && sample.getFirst() < bins[0][j+1])
						b_fst = j;
					if (bins[1][j] <= sample.getSecond() && sample.getSecond() < bins[1][j+1])
						b_snd = j;
				}
				//cout << "sample[0] = " << sample.getFirst() << "in bin " << b_fst << ", sample[1] = " << sample.getSecond() << " in bin " << b_snd<< endl;

				blocks[0][b_fst]++;
				blocks[1][b_snd]++;
			}
			cout << "createHisto 3" << endl;
			// print relative frequency into file
			out << "1st, 2nd" << endl << endl;
			for (I32u k = 0; k < numBins; ++k){
				double f1 = (double)blocks[0][k]/(double)numberOfSamples;
				double f2 = (double)blocks[1][k]/(double)numberOfSamples;
				freq[0][k] = f1;
				freq[1][k] = f2;

				cout << "write f1 = " << f1 << ", f2 = " << f2 << endl;
				out << format("%f,\t%f", f1, f2) << endl;
			}
			out.close();
			cout << "createHisto 4" << endl;
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
