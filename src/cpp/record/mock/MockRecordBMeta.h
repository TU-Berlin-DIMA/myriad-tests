// auto-generatad C++ file for `merchant`

#ifndef MOCKRECORDBMETA_H_
#define MOCKRECORDBMETA_H_

#include "record/Record.h"

namespace Myriad {

// forward declarations
class MockRecordB;

class MockRecordBMeta : public RecordMeta<MockRecordB>
{
public:

    MockRecordBMeta(const I64u cardinality = 0) :
		RecordMeta<MockRecordB>(cardinality) // FIXME: mandatory cardinality
	{
	}

    MockRecordBMeta(const EnumSetPool& enumSets, const I64u cardinality = 0) :
		RecordMeta<MockRecordB>(cardinality) // FIXME: mandatory cardinality
	{
	}
};

} // namespace Myriad

#endif /* MOCKRECORDBMETA_H_ */
