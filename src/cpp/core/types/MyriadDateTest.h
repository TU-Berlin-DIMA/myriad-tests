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
		dates.push_back("2011-04-04");
		dates.push_back("2011-06-07");
		dates.push_back("2011-02-17");
		dates.push_back("2011-08-10");
		dates.push_back("2011-12-18");
		dates.push_back("2012-02-29");

		for(vector<string>::const_iterator it = dates.begin(); it != dates.end(); ++it)
		{
			MyriadDate date = fromString<MyriadDate>(*it);
			string actDate = toString<MyriadDate>(date);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("DATES DON'T MATCH", *it, actDate);
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("MyriadDateTest");
		suite->addTest(new TestCaller<MyriadDateTest> ("testConstructorsAndSerDeMethods", &MyriadDateTest::testConstructorsAndSerDeMethods));
		return suite;
	}

private:
};

} // namespace Myriad

#endif /* MYRIADDATETEST_H_ */
