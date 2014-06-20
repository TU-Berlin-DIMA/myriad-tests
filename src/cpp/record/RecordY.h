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

#ifndef RECORDY_H_
#define RECORDY_H_

#include "record/AbstractRecord.h"
#include "record/RecordX.h"

namespace Myriad {

class RecordY: public Record
{
public:

	void x(const AutoPtr<RecordX>& v)
	{
		_x = v;
	}

	const AutoPtr<RecordX>& x() const
	{
		return _x;
	}

	void a(const I64u& v)
	{
		_a = v;
	}

	const I64u& a() const
	{
		return _a;
	}

private:

	AutoPtr<RecordX> _x;
	I64u _a;
};

} // namespace Myriad

#endif /* RECORDY_H_ */
