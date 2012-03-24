/*
 * This file is part of the myriad-dgen package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef MATHUTILITIESTEST_H_
#define MATHUTILITIESTEST_H_

#include "math/util.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;

namespace Myriad {

class MathUtilitiesTest: public TestFixture
{
public:
	void setUp()
	{
	}

	void tearDown()
	{
	}

	void testModmult()
	{
		I64 N1 = 8832000017LL;
		Decimal InvN1 = 1.0/(Decimal) N1;

		I64 N2 = 2208000007LL;
		Decimal InvN2 = 1.0/(Decimal) N2;

		CPPUNIT_ASSERT(modmult(2717447886LL, 2717447886LL, N1, InvN1) == 8832000016LL);
		CPPUNIT_ASSERT(modmult(550867102LL, 1010693226LL, N2, InvN2) == 604567935LL);
	}

	void testModexp()
	{
		CPPUNIT_ASSERT(modexp(266905, 980448, 980449) == 1);
		CPPUNIT_ASSERT(modexp(1804289385ULL, 0, 8832000017ULL) == 1);
		CPPUNIT_ASSERT(modexp(1804289385ULL, 552000001ULL, 8832000017ULL) == 7296544323ULL);
		CPPUNIT_ASSERT(modexp(1804289385ULL, 8832000016ULL, 8832000017ULL) == 1);
		CPPUNIT_ASSERT(modexp(1804289385ULL, 1104000003ULL, 2208000007ULL) == 1);
	}

	void testIsPrime()
	{
		CPPUNIT_ASSERT(isPrime(963ULL)        == false);
		CPPUNIT_ASSERT(isPrime(574ULL)        == false);
		CPPUNIT_ASSERT(isPrime(423ULL)        == false);
		CPPUNIT_ASSERT(isPrime(388ULL)        == false);
		CPPUNIT_ASSERT(isPrime(303ULL)        == false);
		CPPUNIT_ASSERT(isPrime(471ULL)        == false);
		CPPUNIT_ASSERT(isPrime(251ULL)        == true);
		CPPUNIT_ASSERT(isPrime(907ULL)        == true);
		CPPUNIT_ASSERT(isPrime(573ULL)        == false);
		CPPUNIT_ASSERT(isPrime(626ULL)        == false);
		CPPUNIT_ASSERT(isPrime(554ULL)        == false);
		CPPUNIT_ASSERT(isPrime(196ULL)        == false);
		CPPUNIT_ASSERT(isPrime(176ULL)        == false);
		CPPUNIT_ASSERT(isPrime(316ULL)        == false);
		CPPUNIT_ASSERT(isPrime(174ULL)        == false);
		CPPUNIT_ASSERT(isPrime(968ULL)        == false);
		CPPUNIT_ASSERT(isPrime(271ULL)        == true);
		CPPUNIT_ASSERT(isPrime(493ULL)        == false);
		CPPUNIT_ASSERT(isPrime(890ULL)        == false);
		CPPUNIT_ASSERT(isPrime(886ULL)        == false);
		CPPUNIT_ASSERT(isPrime(973ULL)        == false);
		CPPUNIT_ASSERT(isPrime(234ULL)        == false);
		CPPUNIT_ASSERT(isPrime(414ULL)        == false);
		CPPUNIT_ASSERT(isPrime(986ULL)        == false);
		CPPUNIT_ASSERT(isPrime(688ULL)        == false);
		CPPUNIT_ASSERT(isPrime(319ULL)        == false);
		CPPUNIT_ASSERT(isPrime(482ULL)        == false);
		CPPUNIT_ASSERT(isPrime(719ULL)        == true);
		CPPUNIT_ASSERT(isPrime(137ULL)        == true);
		CPPUNIT_ASSERT(isPrime(281ULL)        == true);
		CPPUNIT_ASSERT(isPrime(774ULL)        == false);
		CPPUNIT_ASSERT(isPrime(487ULL)        == true);
		CPPUNIT_ASSERT(isPrime(739ULL)        == true);
		CPPUNIT_ASSERT(isPrime(714ULL)        == false);
		CPPUNIT_ASSERT(isPrime(610ULL)        == false);
		CPPUNIT_ASSERT(isPrime(887ULL)        == true);
		CPPUNIT_ASSERT(isPrime(657ULL)        == false);
		CPPUNIT_ASSERT(isPrime(380ULL)        == false);
		CPPUNIT_ASSERT(isPrime(37)            == true);
		CPPUNIT_ASSERT(isPrime(159ULL)        == false);
		CPPUNIT_ASSERT(isPrime(18)            == false);
		CPPUNIT_ASSERT(isPrime(80)            == false);
		CPPUNIT_ASSERT(isPrime(356ULL)        == false);
		CPPUNIT_ASSERT(isPrime(1)             == false);
		CPPUNIT_ASSERT(isPrime(88)            == false);
		CPPUNIT_ASSERT(isPrime(53)            == true);
		CPPUNIT_ASSERT(isPrime(751ULL)        == true);
		CPPUNIT_ASSERT(isPrime(391ULL)        == false);
		CPPUNIT_ASSERT(isPrime(503ULL)        == true);
		CPPUNIT_ASSERT(isPrime(29ULL)         == true);
		CPPUNIT_ASSERT(isPrime(0ULL)          == false);
		CPPUNIT_ASSERT(isPrime(980449ULL)     == true);
		CPPUNIT_ASSERT(isPrime(88320001ULL)   == false);
		CPPUNIT_ASSERT(isPrime(88320007ULL)   == true);
		CPPUNIT_ASSERT(isPrime(220800007ULL)  == false);
		CPPUNIT_ASSERT(isPrime(8832000000ULL) == false);
		CPPUNIT_ASSERT(isPrime(8832000017ULL) == true);
		CPPUNIT_ASSERT(isPrime(2208000005ULL) == false);
		CPPUNIT_ASSERT(isPrime(2208000007ULL) == true);
	}

	void testNextPrime()
	{
		CPPUNIT_ASSERT(nextPrime(1000ULL)       == 1009ULL);
		CPPUNIT_ASSERT(nextPrime(4322ULL)       == 4327ULL);
		CPPUNIT_ASSERT(nextPrime(53121ULL)      == 53129ULL);
		CPPUNIT_ASSERT(nextPrime(43224ULL)      == 43237ULL);
		CPPUNIT_ASSERT(nextPrime(10324ULL)      == 10331ULL);
		CPPUNIT_ASSERT(nextPrime(17000ULL)      == 17011ULL);
		CPPUNIT_ASSERT(nextPrime(18050ULL)      == 18059ULL);
		CPPUNIT_ASSERT(nextPrime(980432ULL)     == 980449ULL);
		CPPUNIT_ASSERT(nextPrime(980449ULL)     == 980449ULL);
		CPPUNIT_ASSERT(nextPrime(342891ULL)     == 342899ULL);
		CPPUNIT_ASSERT(nextPrime(342899ULL)     == 342899ULL);
		CPPUNIT_ASSERT(nextPrime(8832000000ULL) == 8832000017ULL);
		CPPUNIT_ASSERT(nextPrime(2208000000ULL) == 2208000007ULL);
	}

	void testPower()
	{
		CPPUNIT_ASSERT(power(0, 0)   == 0);
		CPPUNIT_ASSERT(power(0, 123) == 0);
		CPPUNIT_ASSERT(power(1, 0)   == 1);
		CPPUNIT_ASSERT(power(1, 42)  == 1);
		CPPUNIT_ASSERT(power(2, 6)   == 64);
		CPPUNIT_ASSERT(power(2, 17)  == 131072);
		CPPUNIT_ASSERT(power(7, 7)   == 823543);
		CPPUNIT_ASSERT(power(11, 5)  == 161051);
	}

	void testNearestPow2()
	{
		CPPUNIT_ASSERT(nearestPow2(32) == 32);
		CPPUNIT_ASSERT(nearestPow2(33) == 32);
		CPPUNIT_ASSERT(nearestPow2(47) == 32);
		CPPUNIT_ASSERT(nearestPow2(48) == 64);
		CPPUNIT_ASSERT(nearestPow2(4294967296ULL) == 4294967296ULL);
		CPPUNIT_ASSERT(nearestPow2(4294999437ULL) == 4294967296ULL);
		CPPUNIT_ASSERT(nearestPow2(6442450947ULL) == 8589934592ULL);
		CPPUNIT_ASSERT(nearestPow2(13510798882111487ULL) == 9007199254740992ULL);
		CPPUNIT_ASSERT(nearestPow2(13510798882111489ULL) == 18014398509481984ULL);
		CPPUNIT_ASSERT(nearestPow2(18446744073709551615ULL) == 9223372036854775808ULL);
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("MathUtilitiesTest");
		suite->addTest(new TestCaller<MathUtilitiesTest> ("testModmult", &MathUtilitiesTest::testModmult));
		suite->addTest(new TestCaller<MathUtilitiesTest> ("testModexp", &MathUtilitiesTest::testModexp));
		suite->addTest(new TestCaller<MathUtilitiesTest> ("testIsPrime", &MathUtilitiesTest::testIsPrime));
		suite->addTest(new TestCaller<MathUtilitiesTest> ("testNextPrime", &MathUtilitiesTest::testNextPrime));
//		suite->addTest(new TestCaller<MathUtilitiesTest> ("testPower", &MathUtilitiesTest::testPower));
//		suite->addTest(new TestCaller<MathUtilitiesTest> ("testNearestPow2", &MathUtilitiesTest::testNearestPow2));
		return suite;
	}
};

} // namespace Myriad

#endif /* MATHUTILITIESTEST_H_ */
