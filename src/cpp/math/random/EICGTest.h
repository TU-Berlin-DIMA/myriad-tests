/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2014 Marie Hoffmann <ozymandiaz147@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */


#ifndef EICGTEST_H_
#define EICGTEST_H_

#include "math/random/EICG.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <stdlib.h>
#include <math.h>
#include <limits>

using namespace CppUnit;

namespace Myriad {

struct A {
	static const Int32 p_size = 7;
	static const Int32 primes[7];
};

const Int32 A::primes[A::p_size] = {11, 101, 1009, 10007, 100003, 10000019, 100000007};

class EICGTest: public TestFixture
{

public:

	void testAdd()
	{
		Int32 p = A::primes[rand() % A::p_size];
		EICG eicg(p,0,0,0);

		// test ring overflow in Z_p for p = 10007
		Int32 x, y;
		x = p-1;
		y = 1;
		CPPUNIT_ASSERT(eicg.add(x, y) == 0);

		// test int32 overflow
		x = std::numeric_limits<int>::max();
		y = 1;
		CPPUNIT_ASSERT(eicg.add(x,y) == x-p+1);
	}

	void testMult()
	{
		Int32 p = A::primes[rand() % A::p_size];
		EICG eicg(p,0,0,0);
		// test multiplication below order of Z_p for p = 1009
		CPPUNIT_ASSERT(eicg.mult(p/2,2) == p-1);

		// test multiplication at boundary of Z_p for p = 1009
		CPPUNIT_ASSERT(eicg.mult(p/2+1,2) == 1);

		// test int32 overflow, args > p, and asymmetry of fast multiplication algorithm, resp.
		Int32 z_ref = (Int32) (((long int)std::numeric_limits<int>::max())*2 % ((long int)p));
		CPPUNIT_ASSERT(eicg.mult(2,std::numeric_limits<int>::max()) == z_ref);
		CPPUNIT_ASSERT(eicg.mult(std::numeric_limits<int>::max(),2) == z_ref);
		CPPUNIT_ASSERT(eicg.mult(p+1,2) == 2);
		CPPUNIT_ASSERT(eicg.mult(2,p+1) == 2);
	}

	void testInverse()
	{
		Int32 p = 11;
		EICG eicg(p,0,0,0);
		// reference list of inverse elements for 0..p-1 for p = 11
		Int32 inv[] = {0, 1, 6, 4, 3, 9, 2, 8, 7, 5, 10};
		for (unsigned int i = 0; i < p-1; ++i)
			CPPUNIT_ASSERT(eicg.inverse(i) == inv[i]);
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("EICGTest");
		suite->addTest(new TestCaller<EICGTest> ("testAdd", &EICGTest::testAdd));
		suite->addTest(new TestCaller<EICGTest> ("testMult", &EICGTest::testMult));
		suite->addTest(new TestCaller<EICGTest> ("testInverse", &EICGTest::testInverse));

		return suite;
	}

};

} // namespace Myriad

#endif /* EICGTEST_H_ */
