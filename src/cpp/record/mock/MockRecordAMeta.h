// auto-generatad C++ file for `merchant`

#ifndef MOCKRECORDAMETA_H_
#define MOCKRECORDAMETA_H_

#include "record/Record.h"

using namespace Myriad;

namespace Myriad {

// forward declarations
class MockRecordA;

class MockRecordAMeta : public RecordMeta<MockRecordA>
{
public:

	MockRecordAMeta(const I64u cardinality = 0) :
		RecordMeta<MockRecordA>(cardinality)
	{
	}
};

} // namespace MyriadTest

#endif /* MOCKRECORDAMETA_H_ */
