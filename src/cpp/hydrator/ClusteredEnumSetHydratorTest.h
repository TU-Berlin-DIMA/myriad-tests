/*
 * This file is part of the myriad-toolkit package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef CLUSTEREDENUMSETHYDRATORTEST_H_
#define CLUSTEREDENUMSETHYDRATORTEST_H_

#include "hydrator/ClusteredEnumSetHydrator.h"
#include "record/Card.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include <vector>

using namespace std;
using namespace CppUnit;

namespace Myriad {

class ClusteredEnumSetHydratorTest: public TestFixture
{
public:

	ClusteredEnumSetHydratorTest()
	{
	}

	void testCustomProbabilityHydrator()
	{
		CustomDiscreteProbability f("enumsethydrator.test.random");
		f.define(0, 0.05);
		f.define(1, 0.05);
		f.define(2, 0.10);
		f.define(3, 0.10);
		f.define(4, 0.15);
		f.define(5, 0.15);
		f.define(6, 0.20);
		f.define(7, 0.20);

		unsigned int M = 8;
		unsigned int N = 1000000;
		unsigned int distribution[M];

		RandomStream random;
		ClusteredEnumSetHydrator<Card, I16u> hydrator(&Card::clusterID, &Card::clusterInterval, f, N);

		for (unsigned int i = 0; i < M; i++)
		{
			distribution[i] = 0;
		}

		I16u currentID = 0;
		bool changedFlag = false;
		for (unsigned int i = 0; i < N; i++)
		{
			AutoPtr<Card> recordPtr = new Card();
			recordPtr->genID(i);

			hydrator(recordPtr);

			I16u clusterID = recordPtr->clusterID();
			distribution[clusterID]++;

			int diff = clusterID - currentID;
			if (diff > 1 || diff < 0)
			{
				CPPUNIT_FAIL(format("unclustered sequential clusterIDs %hu, %hu at position %u", clusterID, currentID, i));
			}
			else if (diff == 1)
			{
				if (changedFlag == false)
				{
					currentID = clusterID;
					changedFlag = true;
				}
				else
				{
					CPPUNIT_FAIL(format("unexpected new clusterIDs %hu at position %u", clusterID, i));
				}
			}
			else // diff = 0
			{
				changedFlag = false;
			}

			random.nextChunk();
		}

		for (unsigned int i = 0; i < M; i++)
		{
			double x = f.at(i);
			double y = distribution[i]/static_cast<double>(N);

			CPPUNIT_ASSERT_DOUBLES_EQUAL(x, y, 0.001);
		}
	}

	void testUniformProbabilityHydrator()
	{
		unsigned int M = 40;
		unsigned int N = 1000000;
		unsigned int distribution[M];

		RandomStream random;
		ClusteredEnumSetHydrator<Card, I16u> hydrator(&Card::clusterID, &Card::clusterInterval, M, N);

		for (unsigned int i = 0; i < M; i++)
		{
			distribution[i] = 0;
		}

		I16u currentID = 0;
		bool changedFlag = false;
		for (unsigned int i = 0; i < N; i++)
		{
			AutoPtr<Card> recordPtr = new Card();
			recordPtr->genID(i);

			hydrator(recordPtr);

			I16u clusterID = recordPtr->clusterID();
			distribution[clusterID]++;

			int diff = clusterID - currentID;
			if (diff > 1 || diff < 0)
			{
				CPPUNIT_FAIL(format("unclustered sequential clusterIDs %hu, %hu at position %u", clusterID, currentID, i));
			}
			else if (diff == 1)
			{
				if (changedFlag == false)
				{
					currentID = clusterID;
					changedFlag = true;
				}
				else
				{
					CPPUNIT_FAIL(format("unexpected new clusterIDs %hu at position %u", clusterID, i));
				}
			}
			else // diff = 0
			{
				changedFlag = false;
			}

			random.nextChunk();
		}

		for (unsigned int i = 0; i < M; i++)
		{
			double x = 1/static_cast<double>(M);
			double y = distribution[i]/static_cast<double>(N);

			CPPUNIT_ASSERT_DOUBLES_EQUAL(x, y, 0.001);
		}
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("ClusteredEnumSetHydratorTest");
		suite->addTest(new TestCaller<ClusteredEnumSetHydratorTest> ("testCustomProbabilityHydrator", &ClusteredEnumSetHydratorTest::testCustomProbabilityHydrator));
		suite->addTest(new TestCaller<ClusteredEnumSetHydratorTest> ("testUniformProbabilityHydrator", &ClusteredEnumSetHydratorTest::testUniformProbabilityHydrator));
		return suite;
	}

private:
};

} // namespace Myriad

#endif /* CLUSTEREDENUMSETHYDRATORTEST_H_ */
