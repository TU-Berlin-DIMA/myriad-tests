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

#include "record/AbstractRecord.h"
#include "record/mock/MockRecordAMeta.h"
#include "record/mock/MockRecordB.h"

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// forward declarations
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordA;
class MockRecordAGenerator;
class MockRecordASetterChain;

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// mock record traits
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordA;

template<>
struct RecordTraits<MockRecordA>
{
	typedef MockRecordAMeta MetaType;
    typedef MockRecordAGenerator GeneratorType;
    typedef MockRecordASetterChain SetterChainType;
	typedef RecordFactory<MockRecordA> FactoryType;
	typedef RecordRangePredicate<MockRecordA> RangePredicateType;

	enum Field { UNKNOWN, GEN_ID, MOCK_FIELD_1, MOCK_FIELD_2, MOCK_FIELD_3, MOCK_FIELD_4, POSITION, MOCK_RECORD_B };
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// mock record
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordA : public AbstractRecord
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

	inline void mockField4(const I16u& v)
	{
		_mock_field_4 = v;
	}

	inline const I16u& mockField4() const
	{
		return _mock_field_4;
	}

	inline void position(const I32u& v)
	{
		_position = v;
	}

	inline const I32u& position() const
	{
		return _position;
	}

	inline void mockRecordB(const AutoPtr<MockRecordB>& v)
	{
		_mock_record_b = v;
	}

	inline const AutoPtr<MockRecordB>& mockRecordB() const
	{
		return _mock_record_b;
	}

private:

	// meta reference
	const RecordMetaType& _meta;

	// fields
	I16u _mock_field_1;
	I64u _mock_field_2;
	Decimal _mock_field_3;
	I16u _mock_field_4;
	I32u _position;

	// references
	AutoPtr<MockRecordB> _mock_record_b;
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
    typedef typename MethodTraits<MockRecordA, FieldType>::RefSetter FieldSetterType;
    typedef typename MethodTraits<MockRecordA, FieldType>::RefGetter FieldGetterType;

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
    typedef typename MethodTraits<MockRecordA, FieldType>::RefSetter FieldSetterType;
    typedef typename MethodTraits<MockRecordA, FieldType>::RefGetter FieldGetterType;

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
    typedef typename MethodTraits<MockRecordA, FieldType>::RefSetter FieldSetterType;
    typedef typename MethodTraits<MockRecordA, FieldType>::RefGetter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&MockRecordA::mockField3);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&MockRecordA::mockField3);
    }
};

// mock_field_4
template<>
struct RecordFieldTraits<RecordTraits<MockRecordA>::MOCK_FIELD_4, MockRecordA>
{
    typedef I16u FieldType;
    // record field getter / setter types
    typedef typename MethodTraits<MockRecordA, FieldType>::RefSetter FieldSetterType;
    typedef typename MethodTraits<MockRecordA, FieldType>::RefGetter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&MockRecordA::mockField4);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&MockRecordA::mockField4);
    }
};

// position
template<>
struct RecordFieldTraits<RecordTraits<MockRecordA>::POSITION, MockRecordA>
{
    typedef I32u FieldType;
    // record field getter / setter types
    typedef typename MethodTraits<MockRecordA, FieldType>::RefSetter FieldSetterType;
    typedef typename MethodTraits<MockRecordA, FieldType>::RefGetter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&MockRecordA::position);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&MockRecordA::position);
    }
};

// mock_record_b
template<>
struct RecordFieldTraits<RecordTraits<MockRecordA>::MOCK_RECORD_B, MockRecordA>
{
    typedef MockRecordB FieldType;
    // record field getter / setter types
    typedef typename MethodTraits<MockRecordA, AutoPtr<MockRecordB> >::RefSetter FieldSetterType;
    typedef typename MethodTraits<MockRecordA, AutoPtr<MockRecordB> >::RefGetter FieldGetterType;

    static inline FieldSetterType setter()
    {
        return static_cast<FieldSetterType>(&MockRecordA::mockRecordB);
    }

    static inline FieldGetterType getter()
    {
        return static_cast<FieldGetterType>(&MockRecordA::mockRecordB);
    }
};

} // namespace Myriad

#endif /* MOCKRECORDA_H_ */
