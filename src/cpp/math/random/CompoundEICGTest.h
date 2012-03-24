/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef COMPOUNDEICGTEST_H_
#define COMPOUNDEICGTEST_H_

#include "math/random/CompoundEICG.h"

#include <Poco/RunnableAdapter.h>
#include <Poco/Stopwatch.h>
#include <Poco/ThreadPool.h>

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <cstdlib>
#include <vector>
#include <map>

using namespace CppUnit;
using namespace Poco;
using namespace std;

namespace Myriad {

class CompoundEICGTest: public TestFixture
{
public:

	void testHierarchicalInterface()
	{
		CompoundEICG random = _random;

		UInt32 s1[6] = { 0, 0, 0, 0, 0, 0 };
		CPPUNIT_ASSERT(random.seed() == CompoundEICG::Seed(s1));

		random.nextSubstream();

		for (int i = 0; i < 3; i++)
		{
			random.next();
		}

		UInt32 s2[6] = { 1073741827, 1073745247, 1073855287, 1073906047, 1074048847, 1074226927 };
		CPPUNIT_ASSERT(random.seed() == CompoundEICG::Seed(s2));

		random.nextSubstream();
		for (int i = 0; i < 5; i++)
		{
			random.next();
		}

		UInt32 s3[6] = { 6, 6864, 226986, 328514, 614130, 970304 };
		CPPUNIT_ASSERT(random.seed() == CompoundEICG::Seed(s3));

		for (int i = 0; i < 52; i++)
		{
			random.nextChunk();
		}
		for (int i = 0; i < 23; i++)
		{
			random.next();
		}

		UInt32 s4[6] = { 50, 7376, 228590, 330326, 616358, 972896 };
		CPPUNIT_ASSERT(random.seed() == CompoundEICG::Seed(s4));

		random.resetChunk();
		for (int i = 0; i < 180; i++)
		{
			random.next();
		}

		UInt32 s5[6] = { 207, 7533, 228747, 330483, 616515, 973053 };
		CPPUNIT_ASSERT(random.seed() == CompoundEICG::Seed(s5));

		for (int i = 0; i < 5321; i++)
		{
			random.nextSubstream();
		}
		for (int i = 0; i < 42; i++)
		{
			random.nextChunk();
		}
		for (int i = 0; i < 79; i++)
		{
			random.next();
		}

		UInt32 s6[6] = { 1073744585, 1091997521, 1677853085, 1948070021, 560753770, 1508711172 };
		CPPUNIT_ASSERT(random.seed() == CompoundEICG::Seed(s6));

		random.resetSubstream();
		for (int i = 0; i < 11; i++)
		{
			random.nextChunk();
		}

		UInt32 s7[6] = { 2667, 18255333, 604110267, 874327083, 1634494155, 434967784 };
		CPPUNIT_ASSERT(random.seed() == CompoundEICG::Seed(s7));
	}

	void testRandomAccess()
	{
		srand(1985850524);

		CompoundEICG r1, r2;

		r1 = _random;
		r2 = _random;

		// test random access of chunks
		for (int j = 0; j < 100; j++)
		{
			int n = rand() % 10000;
			for (int i = 0; i < n; i++)
			{
				r1.nextChunk();
			}

			double x = r1.next();
			double y = r2.atChunk(n).next();

			CPPUNIT_ASSERT(x == y);

			r1.resetSubstream();
			r2.resetSubstream();
		}

		// test random access of elements
		for (int j = 0; j < 100; j++)
		{
			int n = rand() % 10000;
			for (int i = 0; i < n; i++)
			{
				r1.next();
			}

			double x = r1.next();
			double y = r2.at(n);

			CPPUNIT_ASSERT(x == y);

			r1.resetChunk();
			r2.resetChunk();
		}
	}

	void testThreads()
	{
		ThreadPool pool;

		CompountEICGRunner t1(_random);
		CompountEICGRunner t2(_random);

		pool.start(t1);
		pool.start(t2);

		pool.joinAll();

		const vector<double>& r1 = t1.result();
		const vector<double>& r2 = t2.result();


		CPPUNIT_ASSERT(r1.size() == r2.size());


		int x = r1.size();
		for (int i = 0; i < x; i++)
		{
			CPPUNIT_ASSERT(r1[i] == r2[i]);
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("CompoundEICGTest");
		suite->addTest(new TestCaller<CompoundEICGTest> ("testHierarchicalInterface", &CompoundEICGTest::testHierarchicalInterface));
		suite->addTest(new TestCaller<CompoundEICGTest> ("testRandomAccess", &CompoundEICGTest::testRandomAccess));
//		suite->addTest(new TestCaller<CompoundEICGTest> ("testThreads", &CompoundEICGTest::testThreads));
		return suite;
	}

private:

	class CompountEICGRunner: public Runnable
	{
	public:
		CompountEICGRunner(CompoundEICG& _random) :
			_random(_random)
		{
		}

		void run()
		{
			// a couple of first elements from the same chunk
			for (int i = 0; i < 10000; i++)
			{
				for (int j = 0; j < 50; j++)
				{
					_result.push_back(_random.next());
				}
			}

			// a couple of first elements from subsequent chunks
			for (int i = 0; i < 10000; i++)
			{
				for (int j = 0; j < 50; j++)
				{
					_result.push_back(_random.next());
				}

				_random.nextChunk();
			}
		}

		const vector<double>& result()
		{
			return _result;
		}

	private:

		CompoundEICG _random;
		vector<double> _result;
	};

	CompoundEICG _random;
	CompoundEICG::Seed _seed;
};

} // namespace Myriad

#endif /* COMPOUNDEICGTEST_H_ */
