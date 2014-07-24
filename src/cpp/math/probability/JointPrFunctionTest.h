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

#ifndef JOINTPRFUNCTIONTEST_H_
#define JOINTPRFUNCTIONTEST_H_

#include <unistd.h>
#include "math/probability/JointPrFunction.h"
#include "core/types/MyriadAbstractTuple.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <Poco/AutoPtr.h>

#include <cmath>
#include <limits>
#include <set>

using namespace CppUnit;
using namespace Poco;
using namespace std;

namespace Myriad {

template<typename T> class JointPrFunctionTest: public TestFixture
{
public:

	JointPrFunctionTest():
		_basePath(""),
		_sampleSize(8)
	{
		size_t size = 512;
		char buffer[size];
		_basePath = "/home/mho/TU-Berlin-DIMA/myriad-tests-project"; // getcwd(buffer);
		// FIXME use stream instead of local distribution file
		_path = _basePath + "/../test/protein_aminoacid.distribution";
		_path2 = _basePath + "/../test/q2d_hist_4.distribution";

	}

	~JointPrFunctionTest() {

		//delete _probability;
		//delete _binID;
		//delete _tID;
	}

	/* tests initialization also */
	/*	void setUp()
	{
		//_path = _basePath + "/../test/protein_aminoacid.distribution";
			//_binID = new I64u[8];

		_probability = new JointPrFunction<MyriadTuple<I64u, I64u> >(_path);
		vector<Decimal> _bucketProbabilities;
		_bucketProbabilities.push_back(.1); // id in [0]
		_bucketProbabilities.push_back(.2); // id in [1,2]
		_bucketProbabilities.push_back(.1); // id in [3]
		_bucketProbabilities.push_back(.6); // id in [4..9]
		// set needed class members explicitly
		JointPrFunction<MyriadTuple<I64, I64> > pr();
		_probability->_sampleSize = this->_sampleSize;
		_probability->_numberOfBuckets = 4;
		_probability->_bucketProbabilities = _bucketProbabilities;

		// solutions
		_binID = new I64u[10]; // binIDs for GenID in [0.._sampleSize-1]
		_binID[0] = 0; _binID[1] = 1; _binID[2] = 1; _binID[3] = 2; for (I64u i = 4; i < 10; ++i) _binID[i] = 3;
		_tID = new I64u[10]; // normalized tIDs for GenID in [0.._sampleSize-1]
		_tID[0] = 0; _tID[1] = 0; _tID[2] = 1; _tID[3] = 0; _tID[4] = 0; _tID[5] = 1; _tID[6] = 2; _tID[7] = 3; _tID[8] = 4; _tID[9] = 5;

	}*/

	/* Test for reading joint probability distribution */
	void testInitialize(){
		JointPrFunction<MyriadTuple<I64, I64> > pr("", _path);
		// file protein_aminoacid.distribution
		I32 numberOfBuckets = 6;
		// test number of buckets correct
		CPPUNIT_ASSERT_EQUAL(pr._numberOfBuckets, numberOfBuckets);

		Decimal _bucketProbabilities[] = {.546, 0.161, 0.09, 0.11, 0.06, 0.04};
		//MyriadTuple
		I64 min_ref[] = {1, 1, 2, 2, 3, 3, 1,4,1,4,1,4};
		I64 max_ref[] = {2,2,3,3,4,4,  4,7,4,7,4,7};
		// buckets with edges and probs correct
		for (I32u i = 0; i < numberOfBuckets; ++i){
//			cout << "i = " << i << endl;
//			cout << "\t prob = " << pr._bucketProbabilities.at(i) << endl;
//			cout << "\t min_i1 = " << pr._buckets.at(i).min().getFirst() << ", min_i2 = " << pr._buckets.at(i).min().getSecond() << endl;
//			cout << "\t max_i1 = " << pr._buckets.at(i).max().getFirst() << ", max_i2 = " << pr._buckets.at(i).max().getSecond() << endl;
//

			CPPUNIT_ASSERT_EQUAL(pr._bucketProbabilities.at(i), _bucketProbabilities[i]);
			// lower bin edge tuple
			CPPUNIT_ASSERT_EQUAL(pr._buckets.at(i).min().getFirst(), min_ref[i]);
			CPPUNIT_ASSERT_EQUAL(pr._buckets.at(i).min().getSecond(), min_ref[numberOfBuckets + i]);
			// upper bin edge tuple
			CPPUNIT_ASSERT_EQUAL(pr._buckets.at(i).max().getFirst(), max_ref[i]);
			CPPUNIT_ASSERT_EQUAL(pr._buckets.at(i).max().getSecond(), max_ref[numberOfBuckets + i]);
		}
		// test domain range
		MyriadTuple<I64, I64> t_min(1,1);
		MyriadTuple<I64, I64> t_max(4,7);
		CPPUNIT_ASSERT(pr._activeDomain.min() == t_min);
		CPPUNIT_ASSERT(pr._activeDomain.max() == t_max);
		// test cumulative distribution values
		Decimal cumulativeProbabilities[] = {.546, 0.707, 0.797, 0.907, 0.967, 1.007};

		//cout << "check cdf:" << endl;
		for (I32 i = 0; i < numberOfBuckets; ++i){
			//	cout << "\tcdf[" << i << "] = " << pr.cdf(i) << endl;
			CPPUNIT_ASSERT_DOUBLES_EQUAL(pr.cdf(i), cumulativeProbabilities[i], 0.0001);
		}
	}

	void testFindBucket(){
		JointPrFunction<MyriadTuple<I64, I64> > pr("", _path);
		I64u binID[] = {0, 0, 0, 1, 1, 1, 3, 4}; // i.e. bin0 is hit by [0..2], bin1 by [3..5], bin2/5 are empty
		for (I64u i = 0; i < this->_sampleSize; ++i){
			//cout << "\nfindBucket[" <<i<<"] = " << pr.findBucket(i) << endl;
			CPPUNIT_ASSERT_EQUAL( pr.findBucket(i), binID[i]);
		}
	}

	// test for GenID normalization, calls findBucket
	void testNormalizeTupleID(){
		JointPrFunction<MyriadTuple<I64, I64> > pr("", _path);
		I64u binID[] = {0, 0, 0, 1, 1, 1, 3, 4}; // i.e. bin0 is hit by [0..2], bin1 by [3..5], bin2/5 are empty
		I64u tID[] = {0,1,2,0,1,2,0,0}; // expected, normalized tuple IDs
		for (I64u i = 0; i < _sampleSize; ++i){
			I64u t = pr.normalizeTupleID(i, binID[i]);
			CPPUNIT_ASSERT_EQUAL(tID[i], t );
		}
	}


	// activate MultiplicativeGroupTest.h, too
	void testPermuteTupleID(){
		cout << "entered testPermuteTupleID" << endl;
		JointPrFunction<MyriadTuple<I64, I64> > pr("", _path);
		pr.setSampleSize(8);
		I64u binID[] = {0, 0, 0, 1, 1, 1, 3, 4}; // i.e. bin0 is hit by [0..2], bin1 by [3..5], bin2/5 are empty
		I64u tID[] = {0,1,2,0,1,2,0,0}; // expected, normalized tuple IDs
		set<I64u> s;
		// check binID = 0,1
		for (I64u bin = 0; bin < 2; ++bin){
			s.clear();
			for (I64u t = 0; t < 3; ++t)
				s.insert(pr.permuteTupleID(t,bin)); // tupleID, binID
			// check length
			CPPUNIT_ASSERT_EQUAL( s.size(), 3UL);
			// check for content [0,2]
			for (I64u elem = 0; elem < 3; ++elem)
				CPPUNIT_ASSERT(s.find(elem) != s.end());

			cout << "set elements are " << endl;
			for (set<I64u>::iterator it = s.begin(); it != s.end(); ++it)
				cout << *it << endl;
		}
		// check binID = 3
		cout << "pr.permuteTupleID(0,3) = " << pr.permuteTupleID(0,3) << endl;
		s.clear();
		s.insert(0);
		s.insert(1);
		s.insert(2);
		CPPUNIT_ASSERT(s.find(pr.permuteTupleID(0,3)) != s.end());
		// check binID = 4
		CPPUNIT_ASSERT(s.find(pr.permuteTupleID(0,4)) != s.end());
	}

	void testScalar2Tuple(){
		JointPrFunction<MyriadTuple<I64, I64> > pr("", this->_path);
		I64 b, t;
		for (I64u binID = 0; binID < 6; ++binID){
			for (I64u tID = 0; tID < 3; ++tID){
				MyriadTuple<I64, I64> m(pr.scalar2Tuple(tID, binID));
				b = binID/2;
				t = tID+(binID %2)*3;
				cout << "binID = " << binID << ", tID = " << tID << ", t = (" << m.getFirst() << ", " << m.getSecond() << ")" << endl;
				//CPPUNIT_ASSERT_EQUAL(b, m.getFirst());
				//CPPUNIT_ASSERT_EQUAL(t, m.getSecond());
			}
		}
	}

	void testSDSS_initialize(){
		JointPrFunction<MyriadTuple<I64u, I64u> > pr("", _path2);
		// file q2d_hist_4.distribution
		I32 numberOfBuckets = 16;
		// test number of buckets correct
		cout << "pr.numBin = " << pr._numberOfBuckets << ", expected " << numberOfBuckets << endl;
		CPPUNIT_ASSERT_EQUAL(numberOfBuckets, pr._numberOfBuckets);
		Decimal _bucketProbabilities[] = {0.0623501, 0.0626747, 0.0623501, 0.0623501, 0.0626747, 0.063001, 0.0626747, 0.0626747, 0.0623501, 0.0626747, 0.0623501, 0.0623501, 0.0623501, 0.0626747, 0.0623501, 0.0623501};
		I64 min_ref[] = {14534925868, 14534925868, 14545516954,14545516954, 14545516954, 14545516954, 14555496890, 14555496890, 14555496890, 14555496890, 14560464574, 14560464574, 14560464574, 14560464574, 17019745, 26712797, 34320382, 42401806, 17019745, 26712797, 34320382, 42401806, 17019745, 26712797, 34320382, 42401806, 17019745, 26712797, 34320382, 42401806};
		I64 max_ref[] = {14545516954, 14545516954, 14545516954, 14545516954, 14555496890, 14555496890, 14555496890, 14555496890, 14560464574, 14560464574, 14560464574, 14560464574, 14568025934, 14568025934, 14568025934, 14568025934};

//		I64 min_ref[] = {1, 1, 2, 2, 3, 3, 1,4,1,4,1,4};
//		I64 max_ref[] = {2,2,3,3,4,4,  4,7,4,7,4,7};
//										, ), [, 26712797)]}
//		p(X) = 0.0626747 for X = { x in [[, ), [, 34320382)]}
//		p(X) = 0.0623501 for X = { x in [[, ), [, 42401806)]}
//		p(X) = 0.0623501 for X = { x in [[, ), [, 49913354)]}
//		p(X) = 0.0626747 for X = { x in [[, ), [, 26712797)]}
//		p(X) = 0.063001 for X = { x in [[, ), [, 34320382)]}
//		p(X) = 0.0626747 for X = { x in [[, ), [, 42401806)]}
//		p(X) = 0.0626747 for X = { x in [[, ), [, 49913354)]}
//		p(X) = 0.0623501 for X = { x in [[, ), [, 26712797)]}
//		p(X) = 0.0626747 for X = { x in [[, ), [, 34320382)]}
//		p(X) = 0.0623501 for X = { x in [[, ), [, 42401806)]}
//		p(X) = 0.0623501 for X = { x in [[, ), [, 49913354)]}
//		p(X) = 0.0623501 for X = { x in [[, ), [, 26712797)]}
//		p(X) = 0.0626747 for X = { x in [[, ), [, 34320382)]}
//		p(X) = 0.0623501 for X = { x in [[, ), [, 42401806)]}
//		p(X) = 0.0623501 for X = { x in [[, ), [, 49913354)]}
	}

	void testSDSS_findBucket(){
		JointPrFunction<MyriadTuple<I64u, I64u> > pr("", _path2);
		I64u sampleSize =  304089172;
		I64u genID[] = {87099048, 283843523, 238752542, 161246917, 194191055, 133212761, 120149163, 111707042, 129314632, 292427477, 277373905, 112934846, 134133339, 175190346, 190252543, 220362731, 142978894, 148823852, 19937566, 0, 87099048};
		I64u binID[] = {4, 14, 12,  8, 10, 6, 6, 5, 6, 15, 14, 5, 7,  9,   9,  11,   7,   7,   1,   0,   4};
		pr.setSampleSize(sampleSize);
		I64u rangeMap_304089172[] = {18959990, 38018688, 56978678, 75938668, 94997366, 114155288, 133213986, 152272683, 171232674, 190291371, 209251361, 228211352, 247171342, 266230040, 285190030, 304150020};
		for (unsigned int i = 0; i < sizeof(genID)/sizeof(I64u); ++i)
			CPPUNIT_ASSERT_EQUAL(binID[i], pr.findBucket(genID[i]));
	}

	// test for GenID normalization, calls findBucket
	void testSDSS_normalizeTupleID(){
		JointPrFunction<MyriadTuple<I64u, I64u> > pr("", _path2);
		I64u sampleSize =  304089172;
		I64u genID[] = {87099048, 283843523, 238752542, 161246917, 194191055, 133212761, 120149163, 111707042, 129314632, 292427477, 277373905, 112934846, 134133339, 175190346, 190252543, 220362731, 142978894, 148823852, 19937566, 0, 87099048};
		I64u binID[] = {4, 14, 12,  8, 10, 6, 6, 5, 6, 15, 14, 5, 7,  9,   9,  11,   7,   7,   1,   0,   4};
		pr.setSampleSize(sampleSize);
		I64u rangeMap_304089172[] = {18959990, 38018688, 56978678, 75938668, 94997366, 114155288, 133213986, 152272683, 171232674, 190291371, 209251361, 228211352, 247171342, 266230040, 285190030, 304150020};
		I64u t, t_ref;
		for (unsigned int i = 0; i < sizeof(genID)/sizeof(I64u); ++i){
			t = pr.normalizeTupleID(genID[i], binID[i]);
			cout << "hier" << endl;
			t_ref = (binID[i] == 0) ? genID[i]: genID[i] - rangeMap_304089172[binID[i]-1];
			cout << "normalized t = " << t << ", t_ref = " << t_ref << endl;
			CPPUNIT_ASSERT_EQUAL(t_ref, t);
		}
	}

	void testSDSS_permuteTupleID(){
		JointPrFunction<MyriadTuple<I64u, I64u> > pr("", _path2);
		I64u sampleSize =  304089172;
		I64u genID[] = {87099048, 283843523, 238752542, 161246917, 194191055, 133212761, 120149163, 111707042, 129314632, 292427477, 277373905, 112934846, 134133339, 175190346, 190252543, 220362731, 142978894, 148823852, 19937566, 0, 87099048};
		I64u binID[] = {4, 14, 12,  8, 10, 6, 6, 5, 6, 15, 14, 5, 7,  9,   9,  11,   7,   7,   1,   0,   4};
		I64u rangeMap_304089172[] = {18959990, 38018688, 56978678, 75938668, 94997366, 114155288, 133213986, 152272683, 171232674, 190291371, 209251361, 228211352, 247171342, 266230040, 285190030, 304150020};

		vector<I64u> tupleID; // normalized genIDs
		for (unsigned int i = 0; i < sizeof(genID)/sizeof(I64u); ++i)
			tupleID.push_back((binID[i] == 0) ? genID[i]: genID[i] - rangeMap_304089172[binID[i]-1]);
		pr.setSampleSize(sampleSize);
		set<I64u> s;
		for (unsigned int i = 0; i < sizeof(genID)/sizeof(I64u); ++i)
			pr.permuteTupleID(tupleID.at(i), binID[i]);
	}

	void testSDSS_scalar2Tuple(){
		JointPrFunction<MyriadTuple<I64, I64> > pr("", this->_path2);
		pr.setSampleSize(1000);
		I64 b, t;
		for (I64u binID = 0; binID < 6; ++binID){
			for (I64u tID = 0; tID < 3; ++tID){
				MyriadTuple<I64, I64> m(pr.scalar2Tuple(tID, binID));
				cout << "binID = " << binID << ", tID = " << tID << ", tuple = (" << m.getFirst() << ", " << m.getSecond() << ")" << endl;
			}
		}
	}

	void testSDSS(){
		JointPrFunction<MyriadTuple<I64u, I64u> > pr("", _path2);
		I64u binCard[] = {4967684, 7511548, 7561360, 7607585, 8081424, 9693052, 9979936, 10591086};
		I64u sampleSize =  304089172;
		I64u genID = 87099048;
		// test findBin
		I64u rangeMap_304089172[] = {60459458, 78658129, 97330639, 115433273, 135106261, 152693782, 170755142, 188246626, 202907362, 215482631, 228531739, 241010971, 258265383,   273434328,   289077112,   304150020};
		// test sample
		cout << "randmax = " << RAND_MAX << endl;
		// RAND_MAX = 2147483647, max_card of q2d: 1088780628878194
		for (unsigned int i = 0; i < 100; ++i){
			sampleSize = rand();
			genID = rand() % sampleSize;
			cout << "sample with genID = " << genID << ", sampleSize = " << sampleSize << endl;
			MyriadTuple<I64u, I64u> t = pr.sample( genID, sampleSize);
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("JointPrFunctionTest");
//		suite->addTest(new TestCaller<JointPrFunctionTest> ("testInitialize", &JointPrFunctionTest<T>::testInitialize));
//		suite->addTest(new TestCaller<JointPrFunctionTest> ("testFindBucket", &JointPrFunctionTest<T>::testFindBucket));
//		suite->addTest(new TestCaller<JointPrFunctionTest> ("testNormalizeTupleID", &JointPrFunctionTest<T>::testNormalizeTupleID));
		suite->addTest(new TestCaller<JointPrFunctionTest> ("testPermuteTupleID", &JointPrFunctionTest<T>::testPermuteTupleID));
//		suite->addTest(new TestCaller<JointPrFunctionTest> ("testScalar2Tuple", &JointPrFunctionTest<T>::testScalar2Tuple));
	//	suite->addTest(new TestCaller<JointPrFunctionTest> ("testSDSS_initialize", &JointPrFunctionTest<T>::testSDSS_initialize));
		//	suite->addTest(new TestCaller<JointPrFunctionTest> ("testSDSS_findBucket", &JointPrFunctionTest<T>::testSDSS_findBucket));
		//	suite->addTest(new TestCaller<JointPrFunctionTest> ("testSDSS_normalizeTupleID", &JointPrFunctionTest<T>::testSDSS_normalizeTupleID));
		suite->addTest(new TestCaller<JointPrFunctionTest> ("testSDSS_permuteTupleID", &JointPrFunctionTest<T>::testSDSS_permuteTupleID));
//		suite->addTest(new TestCaller<JointPrFunctionTest> ("testSDSS_scalar2Tuple", &JointPrFunctionTest<T>::testSDSS_scalar2Tuple));

//		suite->addTest(new TestCaller<JointPrFunctionTest> ("testSDSS", &JointPrFunctionTest<T>::testSDSS));

		return suite;
	}

private:

	string _basePath;
	string _path;
	string _path2;

    I64u _sampleSize;
	//I64u* _binID;
	//I64u* _tID;
	//JointPrFunction<MyriadTuple<I64u, I64u> >* _pr;
};

} // namespace Myriad

#endif /* JOINTPRFUNCTIONTEST_H_ */
