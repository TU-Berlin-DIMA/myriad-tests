/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef RECORDRANGEPREDICATEST_H_
#define RECORDRANGEPREDICATEST_H_

#include "record/mock/MockRecordB.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <sstream>

using namespace std;
using namespace CppUnit;
using namespace MyriadTestMock;

namespace Myriad {

class RecordRangePredicateTest: public TestFixture
{
public:

	typedef RecordTraits<MockRecordB>::MetaType MockRecordBMetaType;
	typedef RecordTraits<MockRecordB>::FactoryType MockRecordBFactoryType;

	RecordRangePredicateTest()
		: _merchantFactory(MockRecordBMetaType())
	{
	}

	void setUp()
	{
		srand(64372234);
	}

//	void testComparisonOperators()
//	{
//		RecordRangePredicate d1("2011-02-03");
//		RecordRangePredicate d2("2011-02-04");
//		RecordRangePredicate d3("2011-02-12");
//		RecordRangePredicate d4("2012-02-12");
//		RecordRangePredicate d5("2012-03-12");
//		RecordRangePredicate d6("2012-04-02");
//		RecordRangePredicate d7("2012-03-29");
//		RecordRangePredicate d8("2011-02-12");
//
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d1 < d2);
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d2 < d3);
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d6 > d7);
//
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d1 <= d2);
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d2 <= d3);
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d6 >= d7);
//
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d8 <= d3);
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d3 <= d8);
//
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d2 != d3);
//		CPPUNIT_ASSERT_MESSAGE("Wrong comparison", d8 == d3);
//	}

	void testGetters()
	{
//		for (I64u i = 0; i < 10000; i++)
//		{
//			AutoPtr<MockRecordB> merchantPtr = _merchantFactory();
//
//			merchantPtr->riskRating(rand() % 10000000);
//			I16u genIDAct = applySpecificGetter<RecordTraits<MockRecordB>::GEN_ID>(merchantPtr);
//			I16u genIDExp = merchantPtr->genIDRef();
//
//			CPPUNIT_ASSERT_MESSAGE("Wrong comparison", genIDExp == genIDAct);
//		}
//
//		for (I64u i = 0; i < 10000; i++)
//		{
//			AutoPtr<MockRecordB> merchantPtr = _merchantFactory();
//
//			merchantPtr->riskRating(rand() % 100);
//			I16u riskRatingAct = applySpecificGetter<RecordTraits<MockRecordB>::RISK_RATING>(merchantPtr);
//			I16u riskRatingExp = merchantPtr->riskRating();
//
//			CPPUNIT_ASSERT_MESSAGE("Wrong comparison", riskRatingExp == riskRatingAct);
//		}
	}

//	template<int fid> const typename RecordFieldTraits<fid, MockRecordB>::FieldType& applySpecificGetter(const AutoPtr<MockRecordB>& merchantPtr)
//	{
//		typedef typename RecordFieldTraits<fid, MockRecordB>::FieldType FieldType;
//		typedef typename RecordFieldTraits<fid, MockRecordB>::GetterType GetterType;
//
//		const GetterType getter = RecordFieldTraits<fid, MockRecordB>::getter();
//
//		return (merchantPtr->*getter)();
//	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("RecordRangePredicateTest");
		suite->addTest(new TestCaller<RecordRangePredicateTest> ("testGetters", &RecordRangePredicateTest::testGetters));
		return suite;
	}

private:

	MockRecordBFactoryType _merchantFactory;
};

} // namespace Myriad

#endif /* RECORDRANGEPREDICATEST_H_ */
