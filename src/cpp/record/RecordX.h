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
 * @author: Alexander Alexandrov <alexander@alex.androv.eu>
 */

#ifndef RECORDX_H_
#define RECORDX_H_

#include "record/Record.h"

namespace Myriad {

class RecordX: public Record
{
public:

	void a(const I64u& v)
	{
		_a = v;
	}

	const I64u& a() const
	{
		return _a;
	}

	void b(const String& v)
	{
		_b = v;
	}

	const String& b() const
	{
		return _b;
	}

private:

	I64u _a;
	String _b;
};

} // namespace Myriad

#endif /* RECORDX_H_ */
