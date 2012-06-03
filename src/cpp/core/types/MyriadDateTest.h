/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef MYRIADDATETEST_H_
#define MYRIADDATETEST_H_

#include "core/types/MyriadDate.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <sstream>

using namespace std;
using namespace CppUnit;

namespace Myriad {

class MyriadDateTest: public TestFixture
{
public:

	MyriadDateTest()
	{
	}

	void testConstructorsAndSerDeMethods()
	{
		stringstream ss;

		vector<string> dates;
		dates.push_back("2011-04-04 07:04:17");
		dates.push_back("2011-06-07 08:30:21");
		dates.push_back("2011-02-17 11:45:40");
		dates.push_back("2011-08-10 16:16:30");
		dates.push_back("2011-12-18 20:04:01");
		dates.push_back("2012-02-29 20:08:31");

		for(vector<string>::const_iterator it = dates.begin(); it != dates.end(); ++it)
		{
			MyriadDate myriadDate = fromString<MyriadDate>(*it);

			CPPUNIT_ASSERT_EQUAL_MESSAGE("Date strings don't match", (*it).substr(0, 10), toString<MyriadDate>(myriadDate));
		}
	}

	void testComparisonOperators()
	{
		MyriadDate d1("2011-02-03");
		MyriadDate d2("2011-02-04");
		MyriadDate d3("2011-02-12");
		MyriadDate d4("2012-02-12");
		MyriadDate d5("2012-03-12");
		MyriadDate d6("2012-04-02");
		MyriadDate d7("2012-03-29");
		MyriadDate d8("2011-02-12");

		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d1 < d2);
		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d2 < d3);
		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d6 > d7);

		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d1 <= d2);
		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d2 <= d3);
		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d6 >= d7);

		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d8 <= d3);
		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d3 <= d8);

		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d2 != d3);
		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d8 == d3);
	}

	void testArithmeticOperators()
	{
		MyriadDate d1("2011-02-03");
		MyriadDate d2("2011-02-04");
		MyriadDate d3("2011-02-12");
		MyriadDate d4("2012-02-12");
		MyriadDate d5("2012-03-12");
		MyriadDate d6("2012-04-02");
		MyriadDate d7("2012-03-29");


		MyriadDate t1 = d1 + 1;
		MyriadDate t2 = t1 + 8;
		MyriadDate t3 = t2 + 365;
		MyriadDate t4 = t3 + 29;
		MyriadDate t5 = t4 + 21;
		MyriadDate t6 = t5 - 4;

		int u1 = d2 - d1;
		int u2 = d3 - d2;
		int u3 = d4 - d3;
		int u6 = d7 - d6;

		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d1 + 1 == t1 && t1 == d2);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d1 + 9 == t2 && t2 == d3);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d1 + 374 == t3 && t3 == d4);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d1 + 403 == t4 && t4 == d5);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d1 + 424 == t5 && t5 == d6);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d1 + 420 == t6 && t6 == d7);

		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d2 - d1 == u1 && u1 == 1);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d3 - d2 == u2 && u2 == 8);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d4 - d3 == u3 && u3 == 365);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d4 - d3 == u3 && u3 == 365);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d7 - d6 == u6 && u6 == -4);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", ((t6++)--) == t6);
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", (--(++t6)) == t6);

		t6 += 1;
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d1 + 421 == t6);
		t6 -= 1;
		CPPUNIT_ASSERT_MESSAGE("Wrong arithmetic expression", d1 + 420 == t6);
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("MyriadDateTest");
		suite->addTest(new TestCaller<MyriadDateTest> ("testConstructorsAndSerDeMethods", &MyriadDateTest::testConstructorsAndSerDeMethods));
		suite->addTest(new TestCaller<MyriadDateTest> ("testComparisonOperators", &MyriadDateTest::testComparisonOperators));
		suite->addTest(new TestCaller<MyriadDateTest> ("testArithmeticOperators", &MyriadDateTest::testArithmeticOperators));
		return suite;
	}

private:
};

} // namespace Myriad

#endif /* MYRIADDATETEST_H_ */
