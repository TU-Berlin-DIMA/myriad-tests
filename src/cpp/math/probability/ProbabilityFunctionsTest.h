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
#include <unistd.h>
#include <limits>

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
		_numberOfSamples = 809; //8095855;
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
		// FIXME use stream instead of local distribution file
		String _path = "../test/q2d_hist_4.distribution";
		String _outpath = "../tmp/q2d_hist_4_result.distribution";
		JointPrFunction<MyriadTuple<I64, I64> > pr("", _path);
		pr.setSampleSize(this->_numberOfSamples);

		// bin edges for each dimension
		const I32u numBins_joint = pr.numberOfBuckets();
		I32u numBins = sqrt(numBins_joint);
		// [a,b,c] <=> [[a,b), [b, c)]
		vector<IntervalTuple<MyriadTuple<I64, I64> > > bins_joint = pr.getBuckets();
		vector<double> freq_joint = pr.getBucketProbabilities();
		// collect edges 1D
		map<I64, double> bins_d1;
		map<I64, double> bins_d2;
		I64 maxBin1 = numeric_limits<int>::lowest();
		I64 maxBin2 = numeric_limits<int>::lowest(); // right most bin edge needed for short representation
		for (unsigned int b = 0; b < bins_joint.size(); ++b){
			IntervalTuple<MyriadTuple<I64, I64> > t = bins_joint.at(b);
			update(t.min().getFirst(), &bins_d1, &freq_joint, b);
			update(t.min().getSecond(), &bins_d2, &freq_joint, b);
			maxBin1 = (maxBin1 < t.max().getFirst()) ? t.max().getFirst(): maxBin1;
			maxBin2 = (maxBin2 < t.max().getSecond()) ? t.max().getSecond(): maxBin2;
		}

		vector<pair<double, double> > freq_ref, freq_res;
		vector<pair<I64,I64> > bins;

		// insert edge of 1st dim in sorted, unique manner
		for (auto keyval : bins_d1){
			bins.push_back(pair<I64, I64>(keyval.first, 0)); // collect key from map
			freq_ref.push_back(make_pair((double)keyval.second, 0.0)); // collect val from map
		}
		// insert 2nd dim
		unsigned int idx = 0;
		for (auto keyval : bins_d2){
			bins.at(idx).second = (double) keyval.first;
			freq_ref.at(idx).second = keyval.second;
			if (idx > 0){
				bins.at(idx).first++;
				bins.at(idx).second++;
			}
			idx++;
		}
		// add max edges
		bins.push_back(pair<I64,I64>(maxBin1+1, maxBin2+1));

		clock_t t_start = clock();
		cout << "create histogram ..." << endl;
		createHistogram2(pr, "../tmp/pr_test_joint_q2d.dat", &bins, numBins, &freq_res);
		cout << "... done (create histogram)" << endl;

		clock_t t_end = clock();
		double elapsed_secs = double(t_end - t_start) / CLOCKS_PER_SEC;
		cout << "sample size = " << this->_numberOfSamples << ", elapsed time [s] = "<< elapsed_secs << endl;

		// print relative frequency and error into file
		double err = 0.0, f1, f2;
		FileOutputStream out(_outpath, std::ios::trunc | std::ios::binary);
		out << "1st, 2nd" << endl << endl;
		for (I32u k = 0; k < numBins; ++k){
			err += (freq_ref.at(k).first - freq_res.at(k).first) * (freq_ref.at(k).first - freq_res.at(k).first);
			err += (freq_ref.at(k).second - freq_res.at(k).second) * (freq_ref.at(k).second - freq_res.at(k).second);
			cout << freq_ref.at(k).first << ", " << freq_res.at(k).first << "; " << freq_ref.at(k).second << ", " << freq_res.at(k).second << endl;
			out << format("%f,\t%f", f1, f2) << endl;
		}
		err /= 2*numBins;
		out << format("error: %f", err) << endl;
		out.close();
		cout << "err = " << err << endl;
	}

	// update frequencies to reconstruct one-dimensional histograms
	void update(I64 key, map<I64, double> *bins, vector<double> *freq_joint, unsigned int i){
		auto it = bins->find(key);
		if (it != bins->end()){
			double freq = bins->at(key);
			bins->erase(key);
			bins->insert(make_pair(key, freq + freq_joint->at(i)));
		}
		else
			bins->insert(make_pair(key, freq_joint->at(i)));
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
	template<typename Pr> void createHistogram2(Pr& pr, const string& filename, vector<pair<I64,I64> > *bins, const I32u numBins,
			vector<pair<double, double> > *freq_res)
	{
		//cout << "createHisto 2" << endl;
		I64 b_fst, b_snd;
		vector<pair<I64, I64> > blocks;
		cout << "init blocks" << endl;
		for (I32u j = 0; j < numBins; ++j)
			blocks.push_back(make_pair(0, 0));
		// sample N random points from the underlying d
		for (I64u i = 0; i < this->_numberOfSamples; i++)
		{
			//const I64u GenID, const I64u sampleSize
			MyriadTuple<I64, I64> sample = pr.sample(i, (I64u)this->_numberOfSamples);
			b_fst = b_snd = 0;
			for (I32u j = 1; j < numBins; ++j){
				if (bins->at(j).first <= sample.getFirst() && sample.getFirst() < bins->at(j+1).first)
					b_fst = j;
				if (bins->at(j).second <= sample.getSecond() && sample.getSecond() < bins->at(j+1).second)
					b_snd = j;
			}
			//cout << "sample[0] = " << sample.getFirst() << "in bin " << b_fst << ", sample[1] = " << sample.getSecond() << " in bin " << b_snd<< endl;

			blocks.at(b_fst).first++;
			blocks.at(b_snd).second++;
		}
		cout << "finished sampling" << endl;
		double err = 0, f1, f2;
		for (I32u k = 0; k < numBins; ++k){
			f1 = (double)blocks.at(k).first/ (double)this->_numberOfSamples;
			f2 = (double)blocks.at(k).second/(double)this->_numberOfSamples;
			freq_res->push_back(make_pair(f1, f2));
		}
		cout << "updated freq_res" << endl;
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
