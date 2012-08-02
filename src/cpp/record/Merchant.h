// auto-generatad C++ file for `merchant`

#ifndef MERCHANT_H_
#define MERCHANT_H_

#include "record/Record.h"
#include "record/MerchantMeta.h"

using namespace Myriad;

namespace MyriadTestMock {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// forward declarations
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class Merchant;
class MerchantConfig;
class MerchantGenerator;
class MerchantHydratorChain;

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// record type
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

class Merchant: public Record
{
public:

    Merchant(const MerchantMeta& meta) :
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
    const MerchantMeta& _meta;

    // fields
    I16u _risk_rating;
    Enum _category;
    I32u _catalog_size;
    Enum _type;
};

inline void Merchant::riskRating(const I16u& v)
{
    _risk_rating = v;
}

inline const I16u& Merchant::riskRating() const
{
    return _risk_rating;
}

inline void Merchant::category(const Enum& v)
{
    _category = v;
}

inline const Enum& Merchant::category() const
{
    return _category;
}

inline void Merchant::catalogSize(const I32u& v)
{
    _catalog_size = v;
}

inline const I32u& Merchant::catalogSize() const
{
    return _catalog_size;
}

inline void Merchant::type(const Enum& v)
{
    _type = v;
}

inline const Enum& Merchant::type() const
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
RecordTraits<MyriadTestMock::Merchant>
{
    typedef MyriadTestMock::MerchantMeta MetaType;
    typedef MyriadTestMock::MerchantGenerator GeneratorType;
    typedef MyriadTestMock::MerchantHydratorChain HydratorChainType;
    typedef RecordFactory<MyriadTestMock::Merchant> FactoryType;
	typedef RecordRangePredicate<MyriadTestMock::Merchant> RangePredicateType;

	enum Field { UNKNOWN, GEN_ID, RISK_RATING };
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// record field inspection structures
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// risk_rating
template<>
struct RecordFieldTraits<RecordTraits<MyriadTestMock::Merchant>::RISK_RATING, MyriadTestMock::Merchant>
{
	typedef I16u FieldType;
	typedef typename MethodTraits<MyriadTestMock::Merchant, FieldType>::Getter GetterType;
	typedef typename MethodTraits<MyriadTestMock::Merchant, FieldType>::Setter SetterType;

	static const char* name;

	static SetterType setter()
	{
		return static_cast<SetterType>(&MyriadTestMock::Merchant::riskRating);
	}

	static GetterType getter()
	{
		return static_cast<GetterType>(&MyriadTestMock::Merchant::riskRating);
	}
};

const char* RecordFieldTraits<2, MyriadTestMock::Merchant>::name = "risk_rating";

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// serialize method specialization
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

template<> inline void OutputCollector<MyriadTestMock::Merchant>::CollectorType::serialize(OutputCollector<MyriadTestMock::Merchant>::CollectorType::StreamType& out, const MyriadTestMock::Merchant& record)
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

#endif /* MERCHANT_H_ */
