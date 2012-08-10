// auto-generatad C++ file for `merchant`

#ifndef MOCKRECORDBMETA_H_
#define MOCKRECORDBMETA_H_

#include "record/Record.h"

using namespace Myriad;

namespace MyriadTestMock {

// forward declarations
class MockRecordB;

class MockRecordBMeta : public RecordMeta<MockRecordB>
{
public:

    MockRecordBMeta(const I64u cardinality = 0) :
		RecordMeta<MockRecordB>(cardinality) // FIXME: mandatory cardinality
	{
	}
};

} // namespace MyriadTest

#endif /* MOCKRECORDBMETA_H_ */
