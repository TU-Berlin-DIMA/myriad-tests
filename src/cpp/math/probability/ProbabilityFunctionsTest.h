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

#include "core/constants.h"
#include "math/random/RandomStream.h"
#include "math/probability/Probability.h"

#include <chrono>
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

	struct ErrorTime{
		int time;
		double error;

	};

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
		_numberOfSamples = 809;
		_numberOfNodes = 1;
		_nodeID = 1; // from [1.._numberOfNodes]
		_rounds = 1;
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

	// KNUTH_B, MINSTD_RAND=DEFAULT_RANDOM_ENGINE, MINSTD_RAND0, MT19937, MT19937_64, RANLUX24, RANLUX48
	void testGenerator_rand(){
		jointProbabilitySampling_caller(RAND, 0);
	}

	void testGenerator_knuth_b(){
		jointProbabilitySampling_caller(KNUTH_B, 0);
	}

	void testGenerator_minstd_rand(){
		jointProbabilitySampling_caller(MINSTD_RAND, 0);
	}

	void testGenerator_minstd_rand0(){
		jointProbabilitySampling_caller(MINSTD_RAND0, 0);
	}

	void testGenerator_mt19937(){
		jointProbabilitySampling_caller(MT19937, 0);
	}

	void testGenerator_mt19937_64(){
		jointProbabilitySampling_caller(MT19937_64, 0);
	}

	void testGenerator_ranlux24(){
		jointProbabilitySampling_caller(RANLUX24, 0);
	}

	void testGenerator_ranlux48(){
		jointProbabilitySampling_caller(RANLUX48, 0);
	}

	void testGenerator_id(){

		this->_numberOfSamples = 8095854922;
		this->_numberOfNodes = 8;
		this->_nodeID = 1; // from [1.._numberOfNodes]
		this->_rounds = 1;
		jointProbabilitySampling_caller(IDENT, 0);
//		this->_numberOfSamples = 8095855;
//		jointProbabilitySampling_caller(IDENT);
//		this->_numberOfSamples = 80958549;
//		jointProbabilitySampling_caller(IDENT);
//		this->_numberOfSamples = 809585492;
//		jointProbabilitySampling_caller(IDENT);
//		this->_numberOfSamples = 8095854922;
//		jointProbabilitySampling_caller(IDENT);
//		this->_numberOfSamples = 80958549222;
//		jointProbabilitySampling_caller(IDENT);

	}

	void testGenerator_partitioned(){
		jointProbabilitySampling_caller(MT19937, 0);
	}

	// compute error within bins between samples and ideal uniform distribution
	void testUniformDistribution(){
		this->_numberOfSamples = 80958549;
		this->_numberOfNodes = 1;
		this->_nodeID = 1;
		this->_rounds = 10;
		jointProbabilitySampling_caller(IDENT, 1);
		jointProbabilitySampling_caller(RAND, 1);
		jointProbabilitySampling_caller(MINSTD_RAND, 1);
		jointProbabilitySampling_caller(MINSTD_RAND0, 1);
		jointProbabilitySampling_caller(KNUTH_B, 1);
		jointProbabilitySampling_caller(MT19937, 1);
		jointProbabilitySampling_caller(MT19937_64, 1);
		jointProbabilitySampling_caller(RANLUX24, 1);
		jointProbabilitySampling_caller(RANLUX48, 1);
	}

	// test std::default_random_engine
	void jointProbabilitySampling_caller(GENERATOR generator, int flag){
		cout << "start test " << generator << ", n = " << this->_numberOfSamples << ", Node = " << this->_nodeID << endl;
		ErrorTime acc;
		acc.error = 0;
		acc.time = 0;
		for (unsigned int i = 0; i < this->_rounds; ++i)
		{
			ErrorTime aux = jointProbabilitySampling(generator, flag);
			acc.error += aux.error;
			acc.time += aux.time;
		}
		acc.error /= this->_rounds;
		acc.time /= this->_rounds;
		std::cout << "Avg error = " << acc.error << "\t in " << acc.time << " microseconds = " << acc.time/1000 << " seconds" <<  endl;
	}

	// flag = 0: compute error on reference and result histogram, flag = 1: compute error of uniform distribution with granularity 10

	struct ErrorTime jointProbabilitySampling(GENERATOR generator, int flag){
		// FIXME use stream instead of local distribution file
		String _path = (!flag) ? "../test/q2e_hist_100_dec_10.distribution" : "../test/q2e_hist_10_dec_10.distribution";
		JointPrFunction<MyriadTuple<I64, I64> > pr("", _path);
		// adjust sample size
		pr.setSampleSize(this->_numberOfSamples);
		// set C++11 pseudo-random number generator for index shuffling
		pr.setGenerator(generator);
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

		vector<pair<double, double> > freq_ref, freq_res0;
		vector<pair<vector<double>, vector<double> > > freq_res1;
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
		const I32u numSubBins = 10; // resolution for each bin
		int elapsed_micro = 0;
		if (!flag)
			createHistogram2(pr, &bins, numBins, &freq_res0, &elapsed_micro);
		else
			createHistogram3(pr, &bins, numBins, numSubBins, &freq_res1, &elapsed_micro);

		// print relative frequency and error into file
		double err = 0.0;

		for (I32u k = 0; k < numBins; ++k){
			if (!flag){
				err += (freq_ref.at(k).first - freq_res0.at(k).first) * (freq_ref.at(k).first - freq_res0.at(k).first);
				err += (freq_ref.at(k).second - freq_res0.at(k).second) * (freq_ref.at(k).second - freq_res0.at(k).second);
			}
			else{// compute error to uniform distribution bin-wise
				for (I32u l = 0; l < numSubBins; ++l){
					err += (freq_ref.at(k).first/(double)numSubBins - freq_res1.at(k).first.at(l)) * (freq_ref.at(k).first/(double)numSubBins - freq_res1.at(k).first.at(l));
					err += (freq_ref.at(k).second/(double)numSubBins - freq_res1.at(k).second.at(l)) * (freq_ref.at(k).second/(double)numSubBins - freq_res1.at(k).second.at(l));
				}
			}
		}
		err /= 2*numBins;

		struct ErrorTime et;
		et.error = err;
		et.time = elapsed_micro;
		return et;
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
/*
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_rand", &ProbabilityFunctionsTest::testGenerator_rand));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_knuth_b", &ProbabilityFunctionsTest::testGenerator_knuth_b));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_minstd_rand", &ProbabilityFunctionsTest::testGenerator_minstd_rand));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_minstd_rand0", &ProbabilityFunctionsTest::testGenerator_minstd_rand0));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_mt19937", &ProbabilityFunctionsTest::testGenerator_mt19937));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_mt19937_64", &ProbabilityFunctionsTest::testGenerator_mt19937_64));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_ranlux24", &ProbabilityFunctionsTest::testGenerator_ranlux24));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_ranlux48", &ProbabilityFunctionsTest::testGenerator_ranlux48));
*/
//		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_partitioned", &ProbabilityFunctionsTest::testGenerator_partitioned));
		suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testUniformDistribution", &ProbabilityFunctionsTest::testUniformDistribution));
	//	suite->addTest(new TestCaller<ProbabilityFunctionsTest> ("testJointProbabilitySampling_id", &ProbabilityFunctionsTest::testGenerator_id));

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
	template<typename Pr> void createHistogram2(Pr& pr, vector<pair<I64,I64> > *bins, const I32u numBins, vector<pair<double, double> > *freq_res, int *time)
	{
		I64 b_fst, b_snd;
		vector<pair<I64, I64> > blocks;
		for (I32u j = 0; j < numBins; ++j)
			blocks.push_back(make_pair(0, 0));
		// sample N random points from the underlying distribution

		I64u partSize = floor(this->_numberOfSamples/this->_numberOfNodes);
		//cout << "ID_start = " << ((this->_nodeID-1) * partSize) << ", ID_end = " << min(this->_numberOfSamples, (I64u)floor(this->_nodeID*partSize)) << endl;

		for (I64u i = ((this->_nodeID-1)* partSize); i < min(this->_numberOfSamples, (I64u)floor(this->_nodeID*partSize))-1; i++)
		{
			//const I64u GenID, const I64u sampleSize
			//clock_t t_start = clock();

			auto start = std::chrono::steady_clock::now();

			MyriadTuple<I64, I64> sample = pr.sample(i, (I64u)this->_numberOfSamples);

			//clock_t t_end = clock();
			auto end = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			//std::cout << "It took me " << elapsed.count() << " microseconds." << std::endl;

			*time += (int)elapsed;
			//cout << "t_end - t_start = " << double(t_end-t_start) << endl;

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
		double err = 0, f1, f2;
		for (I32u k = 0; k < numBins; ++k){
			f1 = (double)blocks.at(k).first/ (double)this->_numberOfSamples;
			f2 = (double)blocks.at(k).second/(double)this->_numberOfSamples;
			freq_res->push_back(make_pair(f1, f2));
		}
	}

	// histograms dimension-wise (2D) with subdivided buckets for joint probability function
	template<typename Pr> void createHistogram3(Pr& pr, vector<pair<I64,I64> > *bins, const I32u numBins, const I32 numSubBins, vector<pair<vector<double>, vector<double> > > *freq_res, int *time)
	{
			I64 b_fst, b_snd, bs_fst, bs_snd;
			// initialize histogram
			vector<pair<vector<I64>, vector<I64> > > blocks;
			for (I32u j = 0; j < numBins; ++j){
				vector<I64> aux1, aux2;
				for (I32u k = 0; k < numSubBins; ++k){
					aux1.push_back(0);
					aux2.push_back(0);
				}

				blocks.push_back(make_pair(aux1, aux2));
				aux1.clear(); aux2.clear();
			}
			// sample N random points from the underlying distribution

			I64u partSize = floor(this->_numberOfSamples/this->_numberOfNodes);
			//cout << "ID_start = " << ((this->_nodeID-1) * partSize) << ", ID_end = " << min(this->_numberOfSamples, (I64u)floor(this->_nodeID*partSize)) << endl;

			for (I64u i = ((this->_nodeID-1)* partSize); i < min(this->_numberOfSamples, (I64u)floor(this->_nodeID*partSize))-1; i++)
			{
				auto start = std::chrono::steady_clock::now();

				MyriadTuple<I64, I64> sample = pr.sample(i, (I64u)this->_numberOfSamples);

				auto end = std::chrono::steady_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
				*time += (int)elapsed;

				b_fst = b_snd = 0;
				for (I32u j = 1; j < numBins; ++j){
					I64 width = ceil((bins->at(j+1).first - bins->at(j).first)/numSubBins);
					if (bins->at(j).first <= sample.getFirst() && sample.getFirst() < bins->at(j+1).first){
						b_fst = j; // found coarse-grained bin
						bs_fst = 0;
						for (I32u k = 1; k < numSubBins; ++k){
							if ((bins->at(j).first + k*width) <= sample.getFirst() && sample.getFirst() < (bins->at(j).first + (k+1)*width)){
								bs_fst = k; break; // found fine-grained bin
							}
						}
					}
					if (bins->at(j).second <= sample.getSecond() && sample.getSecond() < bins->at(j+1).second){
						b_snd = j;
						bs_snd = 0;
						for (I32u k = 1; k < numSubBins; ++k){
							if ((bins->at(j).second + k*width) <= sample.getSecond() && sample.getSecond() < (bins->at(j).second + (k+1)*width)){
								bs_snd = k; break; // found fine-grained bin
							}
						}
					}
				}

				blocks.at(b_fst).first.at(bs_fst)++;
				blocks.at(b_snd).second.at(bs_fst)++;
			}
			double f1, f2;
			for (I32u k = 0; k < numBins; ++k){
				vector<double> aux1, aux2;
				for (I32u l = 0; l < numSubBins; ++l){
					f1 = (double)blocks.at(k).first.at(l) / (double)this->_numberOfSamples;
					f2 = (double)blocks.at(k).second.at(l) / (double)this->_numberOfSamples;
					aux1.push_back(f1);
					aux2.push_back(f2);
				}
				freq_res->push_back(make_pair(aux1, aux2));
				aux1.clear(); aux2.clear();
			}
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
	I64u _numberOfSamples;

	/**
	 * Number of nodes generating data set.
	 */
	I64u _numberOfNodes;

	/**
	 * Partition number of current instance.
	 */
	I64u _nodeID;

	/**
	 * Number of repetitions.
	 */
	I32u _rounds;
};

} // namespace Myriad

#endif /* RANDOMSTREAMTEST_H_ */
