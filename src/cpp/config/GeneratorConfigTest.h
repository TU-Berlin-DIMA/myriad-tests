/*
 * This file is part of the myriad-dgen package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef GENERATORCONFIGTEST_H_
#define GENERATORCONFIGTEST_H_

#include "core/types.h"
#include "config/GeneratorConfig.h"
#include "generator/GeneratorPool.h"
#include "math/DiscreteMap.h"
#include "math/probability/CustomDiscreteProbability.h"
#include "math/probability/ParetoPrFunction.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <Poco/Util/LoggingConfigurator.h>
#include <Poco/Util/MapConfiguration.h>

using namespace CppUnit;
using namespace Poco;
using namespace Poco::Util;

namespace Myriad {

class GeneratorConfigTest: public TestFixture
{
public:

	GeneratorConfigTest()
		: _config(_generatorPool)
	{
	}

	void setUp()
	{
		_appConfig.assign(new MapConfiguration());

		// logging
		_appConfig->setString("logging.loggers.root.channel", "c1");
		_appConfig->setString("logging.loggers.root.level", "debug");
		_appConfig->setString("logging.channels.c1.class", "ConsoleChannel");
		_appConfig->setString("logging.channels.c1.formatter.class", "PatternFormatter");
		_appConfig->setString("logging.channels.c1.formatter.pattern", "%t");
		// application
		_appConfig->setString("application.baseName", "myriad-tests");
		_appConfig->setString("application.config-dir", Path("build/config").makeAbsolute().toString());
		_appConfig->setString("application.output-base", Path("build/data").makeAbsolute().toString());
		_appConfig->setString("application.job-id", "default-job");
		// common
		_appConfig->setString("common.config.master-file", "test-config.xml");

		LoggingConfigurator loggingConfigurator;
		loggingConfigurator.configure(_appConfig);
	}

	void tearDown()
	{
	}

	void testGeneratorConfig()
	{
		_config.initialize(*_appConfig);

		CustomDiscreteProbability& f0 = _config.func<CustomDiscreteProbability>("t-product-class-probability");
		CPPUNIT_ASSERT(f0(0) == 0.24);
		CPPUNIT_ASSERT(f0(1) == 0.26);
		CPPUNIT_ASSERT(f0(2) == 0.17);
		CPPUNIT_ASSERT(f0(3) == 0.12);
		CPPUNIT_ASSERT(f0(4) == 0.21);

		CustomDiscreteProbability& f1 = _config.func<CustomDiscreteProbability>("t-male-name-probability");
		CPPUNIT_ASSERT(f1(0) == 0.4);
		CPPUNIT_ASSERT(f1(1) == 0.3);
		CPPUNIT_ASSERT(f1(2) == 0.2);
		CPPUNIT_ASSERT(f1(3) == 0.1);

		DiscreteMap<ID, Interval<ID> >& f2 = _config.func<DiscreteMap<ID, Interval<ID> > >("t-id-range-map");
		CPPUNIT_ASSERT(f2.name() == "t-id-range-map");

		IntervalMap<ID, ID>& f3 = _config.func<IntervalMap<ID, ID> >("t-interval-map");
		CPPUNIT_ASSERT(f3.name() == "t-interval-map");

		ParetoPrFunction& f4 = _config.func<ParetoPrFunction>("t-pareto");
		ParetoPrFunction h4(7.0, 1);
		CPPUNIT_ASSERT(f4(0.4) == h4(0.4));
		CPPUNIT_ASSERT(f4.sample(0.42) - h4.sample(0.42) < 0.00000000001);
		CPPUNIT_ASSERT(f4.sample(0.98) - h4.sample(0.98) < 0.00000000001);

		const vector<String>& maleNames = _config.maleNames();
		CPPUNIT_ASSERT(maleNames.size() == 4);
		CPPUNIT_ASSERT(maleNames[0] == "John");
		CPPUNIT_ASSERT(maleNames[1] == "Jake");
		CPPUNIT_ASSERT(maleNames[2] == "Paul");
		CPPUNIT_ASSERT(maleNames[3] == "Don");

		const vector<AutoPtr<ProductClass> >& productClasses = _config.productClasses();
		CPPUNIT_ASSERT(productClasses.size() == 5);
		CPPUNIT_ASSERT(productClasses[0]->classSK() == 10000);
		CPPUNIT_ASSERT(productClasses[1]->classSK() == 20000);
		CPPUNIT_ASSERT(productClasses[2]->classSK() == 30000);
		CPPUNIT_ASSERT(productClasses[3]->classSK() == 40000);
		CPPUNIT_ASSERT(productClasses[4]->classSK() == 50000);
		CPPUNIT_ASSERT(productClasses[0]->className() == "Automotive");
		CPPUNIT_ASSERT(productClasses[1]->className() == "Baby");
		CPPUNIT_ASSERT(productClasses[2]->className() == "Beauty");
		CPPUNIT_ASSERT(productClasses[3]->className() == "Books");
		CPPUNIT_ASSERT(productClasses[4]->className() == "Cell Phones & Service");
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("GeneratorConfigTest");
		suite->addTest(new TestCaller<GeneratorConfigTest> ("testGeneratorConfig", &GeneratorConfigTest::testGeneratorConfig));
		return suite;
	}

private:

	/**
	 * Application configuration.
	 */
	AutoPtr<MapConfiguration> _appConfig;

	/**
	 * A pool for the registered generators.
	 */
	GeneratorPool _generatorPool;

	/**
	 * An application wide generator config instance.
	 */
	GeneratorConfig _config;
};

} // namespace Myriad

#endif /* GENERATORCONFIGTEST_H_ */
