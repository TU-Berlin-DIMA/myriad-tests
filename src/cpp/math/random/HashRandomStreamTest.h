/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef HASHRANDOMSTREAMTEST_H_
#define HASHRANDOMSTREAMTEST_H_

#include "math/random/HashRandomStream.h"

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

class HashRandomStreamTest: public TestFixture
{
public:

	void testHierarchicalInterface()
	{
		HashRandomStream random = _random;

		UInt64 s1[1] = { 0ULL };
		CPPUNIT_ASSERT(random.seed() == HashRandomStream::Seed(s1));

		random.nextSubstream();

		for (int i = 0; i < 3; i++)
		{
			random.next();
		}

		UInt64 s2[1] = { 281474976710659ULL };
		CPPUNIT_ASSERT(random.seed() == HashRandomStream::Seed(s2));

		random.nextSubstream();
		for (int i = 0; i < 5; i++)
		{
			random.next();
		}

		UInt64 s3[1] = { 562949953421317ULL };
		CPPUNIT_ASSERT(random.seed() == HashRandomStream::Seed(s3));

		for (int i = 0; i < 52; i++)
		{
			random.nextChunk();
		}
		for (int i = 0; i < 23; i++)
		{
			random.next();
		}

		UInt64 s4[1] = { 562949953434647ULL };
		CPPUNIT_ASSERT(random.seed() == HashRandomStream::Seed(s4));

		random.resetChunk();
		for (int i = 0; i < 180; i++)
		{
			random.next();
		}

		UInt64 s5[1] = { 562949953434804ULL };
		CPPUNIT_ASSERT(random.seed() == HashRandomStream::Seed(s5));

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

		UInt64 s6[1] = { 1498291301030832719ULL };
		CPPUNIT_ASSERT(random.seed() == HashRandomStream::Seed(s6));

		random.resetSubstream();
		for (int i = 0; i < 11; i++)
		{
			random.nextChunk();
		}

		UInt64 s7[1] = { 1498291301030824704ULL };
		CPPUNIT_ASSERT(random.seed() == HashRandomStream::Seed(s7));
	}

	void testRandomAccess()
	{
		srand(1985850524);

		HashRandomStream r1, r2;

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

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("HashRandomStreamTest");
		suite->addTest(new TestCaller<HashRandomStreamTest> ("testHierarchicalInterface", &HashRandomStreamTest::testHierarchicalInterface));
		suite->addTest(new TestCaller<HashRandomStreamTest> ("testRandomAccess", &HashRandomStreamTest::testRandomAccess));
		return suite;
	}

private:

	HashRandomStream _random;
	HashRandomStream::Seed _seed;
};

} // namespace Myriad

#endif /* HASHRANDOMSTREAMTEST_H_ */
