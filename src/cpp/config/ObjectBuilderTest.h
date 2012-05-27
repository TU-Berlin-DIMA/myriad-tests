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

#ifndef SANDBOXTEST_H_
#define SANDBOXTEST_H_

#include "core/types.h"
#include "config/ObjectBuilder.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

using namespace CppUnit;
using namespace Poco;

namespace Myriad {

class ObjectBuilderTest: public TestFixture
{
public:

	class ConfigurableObject
	{
	public:
		ConfigurableObject(map<string, Any>& params)
		{
			x = AnyCast<Decimal>(params["x"]);
			y = AnyCast<I16u>(params["y"]);
			z = AnyCast<String>(params["z"]);
		}

		ConfigurableObject(const String& name, map<string, Any>& params) :
			name(name)
		{
			x = AnyCast<Decimal>(params["x"]);
			y = AnyCast<I16u>(params["y"]);
			z = AnyCast<String>(params["z"]);
		}

		String name;
		Decimal x;
		I16u y;
		String z;
	};

	void setUp()
	{
	}

	void tearDown()
	{
	}

	void testObjectBuilder()
	{
		builder
			.clear()
			.addParameter("name", string("myConfigurable"))
			.addParameter("x", 3.1415926535)
			.addParameter("y", static_cast<I16u>(42))
			.addParameter("z", string("Hello, World!"));

		ConfigurableObject* o = builder.createObject<ConfigurableObject>();

		CPPUNIT_ASSERT(o->name == "myConfigurable");
		CPPUNIT_ASSERT(o->x == 3.1415926535);
		CPPUNIT_ASSERT(o->y == 42);
		CPPUNIT_ASSERT(o->z == "Hello, World!");
	}

	static Test *suite()
	{
		TestSuite* suite = new TestSuite("ObjectBuilderTest");
		suite->addTest(new TestCaller<ObjectBuilderTest> ("testObjectBuilder", &ObjectBuilderTest::testObjectBuilder));
		return suite;
	}

private:

	ObjectBuilder builder;
};

} // namespace Myriad

#endif /* SANDBOXTEST_H_ */
