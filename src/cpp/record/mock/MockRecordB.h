// auto-generatad C++ file for `merchant`

#ifndef MOCKRECORDB_H_
#define MOCKRECORDB_H_

#include "record/Record.h"
#include "record/mock/MockRecordBMeta.h"

using namespace Myriad;

namespace MyriadTestMock {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// forward declarations
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordB;
class MockRecordBConfig;
class MockRecordBGenerator;
class MockRecordBHydratorChain;

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// record type
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class MockRecordB: public Record
{
public:

    MockRecordB(const MockRecordBMeta& meta) :
        _meta(meta)
    {
    }

    void riskRating(const I16u& v);
    const I16u& riskRating() const;

    void category(const Enum& v);
    const Enum& category() const;

    void catalogSize(const I32u& v);
    const I32u& catalogSize() const;

    void type(const Enum& v);
    const Enum& type() const;

protected:

    // meta
    const MockRecordBMeta& _meta;

    // fields
    I16u _risk_rating;
    Enum _category;
    I32u _catalog_size;
    Enum _type;
};

inline void MockRecordB::riskRating(const I16u& v)
{
    _risk_rating = v;
}

inline const I16u& MockRecordB::riskRating() const
{
    return _risk_rating;
}

inline void MockRecordB::category(const Enum& v)
{
    _category = v;
}

inline const Enum& MockRecordB::category() const
{
    return _category;
}

inline void MockRecordB::catalogSize(const I32u& v)
{
    _catalog_size = v;
}

inline const I32u& MockRecordB::catalogSize() const
{
    return _catalog_size;
}

inline void MockRecordB::type(const Enum& v)
{
    _type = v;
}

inline const Enum& MockRecordB::type() const
{
    return _type;
}

} // namespace MyriadTest

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// record traits specialization
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// record traits specialization
template<> struct
RecordTraits<MyriadTestMock::MockRecordB>
{
    typedef MyriadTestMock::MockRecordBMeta MetaType;
    typedef MyriadTestMock::MockRecordBGenerator GeneratorType;
    typedef MyriadTestMock::MockRecordBHydratorChain HydratorChainType;
    typedef RecordFactory<MyriadTestMock::MockRecordB> FactoryType;
	typedef RecordRangePredicate<MyriadTestMock::MockRecordB> RangePredicateType;

	enum Field { UNKNOWN, GEN_ID, RISK_RATING };
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// record field inspection structures
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// risk_rating
template<>
struct RecordFieldTraits<RecordTraits<MyriadTestMock::MockRecordB>::RISK_RATING, MyriadTestMock::MockRecordB>
{
	typedef I16u FieldType;
	typedef typename MethodTraits<MyriadTestMock::MockRecordB, FieldType>::Getter GetterType;
	typedef typename MethodTraits<MyriadTestMock::MockRecordB, FieldType>::Setter SetterType;

	static const char* name;

	static SetterType setter()
	{
		return static_cast<SetterType>(&MyriadTestMock::MockRecordB::riskRating);
	}

	static GetterType getter()
	{
		return static_cast<GetterType>(&MyriadTestMock::MockRecordB::riskRating);
	}
};

const char* RecordFieldTraits<2, MyriadTestMock::MockRecordB>::name = "risk_rating";

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// serialize method specialization
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

template<> inline void OutputCollector<MyriadTestMock::MockRecordB>::CollectorType::serialize(OutputCollector<MyriadTestMock::MockRecordB>::CollectorType::StreamType& out, const MyriadTestMock::MockRecordB& record)
{
    write(out, record.riskRating(), false);
    out << '|';
    write(out, record.category(), false);
    out << '|';
    write(out, record.type(), false);
    out << '|';
    write(out, record.catalogSize(), false);
    out << '|';
    out << '\n';
}

} // namespace Myriad

#endif /* MOCKRECORDB_H_ */
