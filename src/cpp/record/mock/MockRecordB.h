// auto-generatad C++ file for `merchant`

#ifndef MOCKRECORDB_H_
#define MOCKRECORDB_H_

#include "record/Record.h"
#include "record/mock/MockRecordBMeta.h"

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// forward declarations
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordB;
class MockRecordBGenerator;
class MockRecordBSetterChain;

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// record traits specialization
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// record traits specialization
template<> struct
RecordTraits<MockRecordB>
{
    typedef MockRecordBMeta MetaType;
    typedef MockRecordBGenerator GeneratorType;
    typedef MockRecordBSetterChain SetterChainType;
    typedef RecordFactory<MockRecordB> FactoryType;
	typedef RecordRangePredicate<MockRecordB> RangePredicateType;

	enum Field { UNKNOWN, GEN_ID, CATALOG_SIZE, RISK_RATING, CATEGORY, TYPE };
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// record type
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordB: public Record
{
public:

    typedef RecordTraits<MockRecordB>::MetaType RecordMetaType;

    MockRecordB(const MockRecordBMeta& meta) :
        _meta(meta)
    {
    }

    const RecordMetaType& meta() const
    {
        return _meta;
    }

    inline void catalogSize(const I32u& v)
    {
        _catalog_size = v;
    }

    inline const I32u& catalogSize() const
    {
        return _catalog_size;
    }

    inline void riskRating(const I16u& v)
    {
        _risk_rating = v;
    }

    inline const I16u& riskRating() const
    {
        return _risk_rating;
    }

    inline void category(const Enum& v)
    {
        _category = v;
    }

    inline const Enum& category() const
    {
        return _category;
    }

    inline void type(const Enum& v)
    {
        _type = v;
    }

    inline const Enum& type() const
    {
        return _type;
    }

protected:

    // meta
    const MockRecordBMeta& _meta;

    // fields
    I32u _catalog_size;
    I16u _risk_rating;
    Enum _category;
    Enum _type;
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// record field inspection structures
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// catalog size
template<>
struct RecordFieldTraits<RecordTraits<MockRecordB>::CATALOG_SIZE, MockRecordB>
{
	typedef I32u FieldType;
	typedef typename MethodTraits<MockRecordB, FieldType>::Getter FieldGetterType;
	typedef typename MethodTraits<MockRecordB, FieldType>::Setter FieldSetterType;

	static FieldSetterType setter()
	{
		return static_cast<FieldSetterType>(&MockRecordB::catalogSize);
	}

	static FieldGetterType getter()
	{
		return static_cast<FieldGetterType>(&MockRecordB::catalogSize);
	}
};

// risk_rating
template<>
struct RecordFieldTraits<RecordTraits<MockRecordB>::RISK_RATING, MockRecordB>
{
	typedef I16u FieldType;
	typedef typename MethodTraits<MockRecordB, FieldType>::Getter FieldGetterType;
	typedef typename MethodTraits<MockRecordB, FieldType>::Setter FieldSetterType;

	static FieldSetterType setter()
	{
		return static_cast<FieldSetterType>(&MockRecordB::riskRating);
	}

	static FieldGetterType getter()
	{
		return static_cast<FieldGetterType>(&MockRecordB::riskRating);
	}
};

// category
template<>
struct RecordFieldTraits<RecordTraits<MockRecordB>::CATEGORY, MockRecordB>
{
	typedef Enum FieldType;
	typedef typename MethodTraits<MockRecordB, FieldType>::Getter FieldGetterType;
	typedef typename MethodTraits<MockRecordB, FieldType>::Setter FieldSetterType;

	static FieldSetterType setter()
	{
		return static_cast<FieldSetterType>(&MockRecordB::category);
	}

	static FieldGetterType getter()
	{
		return static_cast<FieldGetterType>(&MockRecordB::category);
	}
};

// type
template<>
struct RecordFieldTraits<RecordTraits<MockRecordB>::TYPE, MockRecordB>
{
	typedef Enum FieldType;
	typedef typename MethodTraits<MockRecordB, FieldType>::Getter FieldGetterType;
	typedef typename MethodTraits<MockRecordB, FieldType>::Setter FieldSetterType;

	static FieldSetterType setter()
	{
		return static_cast<FieldSetterType>(&MockRecordB::type);
	}

	static FieldGetterType getter()
	{
		return static_cast<FieldGetterType>(&MockRecordB::type);
	}
};

} // namespace Myriad

#endif /* MOCKRECORDB_H_ */
