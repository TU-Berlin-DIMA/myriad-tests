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

#ifndef GENERATORCONFIGTEST_H_
#define GENERATORCONFIGTEST_H_

#include "core/types.h"
#include "config/GeneratorConfig.h"
#include "generator/GeneratorPool.h"
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
		// TODO: implement a range of generator config unit tests
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
