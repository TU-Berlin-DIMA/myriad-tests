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

#ifndef MOCKRECORDA_H_
#define MOCKRECORDA_H_

#include "record/Record.h"
#include "record/mock/MockRecordAMeta.h"

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// mock record traits
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordA;

template<>
struct RecordTraits<MockRecordA>
{
	typedef MockRecordAMeta MetaType;
	typedef RecordFactory<MockRecordA> FactoryType;

	enum Field { UNKNOWN, GEN_ID, MOCK_FIELD_1, MOCK_FIELD_2, MOCK_FIELD_3 };
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// mock record
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordA : public Record
{
public:

	typedef RecordTraits<MockRecordA>::MetaType RecordMetaType;

	MockRecordA(const RecordMetaType& meta) :
		_meta(meta)
	{
	}

	const RecordMetaType& meta() const
	{
		return _meta;
	}

	inline void mockField1(const I16u& v)
	{
		_mock_field_1 = v;
	}

	inline const I16u& mockField1() const
	{
		return _mock_field_1;
	}

	inline void mockField2(const I64u& v)
	{
		_mock_field_2 = v;
	}

	inline const I64u& mockField2() const
	{
		return _mock_field_2;
	}

	inline void mockField3(const Decimal& v)
	{
		_mock_field_3 = v;
	}

	inline const Decimal& mockField3() const
	{
		return _mock_field_3;
	}

private:

	// meta reference
	const RecordMetaType& _meta;

	// fields
	I16u _mock_field_1;
	I64u _mock_field_2;
	Decimal _mock_field_3;
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// mock record field inspection structures
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// mock_field_1
template<>
struct RecordFieldTraits<RecordTraits<MockRecordA>::MOCK_FIELD_1, MockRecordA>
{
    typedef I16u FieldType;
    // record field getter / setter types
    typedef typename MethodTraits<MockRecordA, FieldType>::Setter FieldSetterType;
    typedef typename MethodTraits<MockRecordA, FieldType>::Getter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&MockRecordA::mockField1);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&MockRecordA::mockField1);
    }
};

// mock_field_2
template<>
struct RecordFieldTraits<RecordTraits<MockRecordA>::MOCK_FIELD_2, MockRecordA>
{
    typedef I64u FieldType;
    // record field getter / setter types
    typedef typename MethodTraits<MockRecordA, FieldType>::Setter FieldSetterType;
    typedef typename MethodTraits<MockRecordA, FieldType>::Getter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&MockRecordA::mockField2);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&MockRecordA::mockField2);
    }
};

// mock_field_3
template<>
struct RecordFieldTraits<RecordTraits<MockRecordA>::MOCK_FIELD_3, MockRecordA>
{
    typedef Decimal FieldType;
    // record field getter / setter types
    typedef typename MethodTraits<MockRecordA, FieldType>::Setter FieldSetterType;
    typedef typename MethodTraits<MockRecordA, FieldType>::Getter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&MockRecordA::mockField3);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&MockRecordA::mockField3);
    }
};

} // namespace Myriad

#endif /* MOCKRECORDA_H_ */