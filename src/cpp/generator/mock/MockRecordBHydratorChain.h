/*
 * This file is part of the myriad-tests package.
 *
 * (c) 2010 Alexander Alexandrov <alexander.s.alexandrov@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef MOCKRECORDBHYDRATORCHAIN_H_
#define MOCKRECORDBHYDRATORCHAIN_H_

#include "generator/RandomSetGenerator.h"
#include "generator/mock/MockRecordBHydratorChain.h"
#include "record/mock/MockRecordB.h"
#include "math/probability/util/CombinedPrFunctionInput.h"
#include "runtime/provider/value/ClusteredValueProvider.h"

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// HydratorChain specialization
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

/**
 * Hydrator specialization for User.
 */
class MockRecordBHydratorChain : public HydratorChain<MockRecordB>
{
public:

	// typedefs for the 'category' field
	typedef CombinedPrFunction<Enum> CategoryPrFunctionType;
	typedef ConstRangeProvider<Enum, MockRecordB> CategoryRangeProviderType;
	typedef ClusteredValueProvider<Enum, MockRecordB, CategoryPrFunctionType, CategoryRangeProviderType> CategoryValueProviderType;
	typedef FieldSetter<MockRecordB, RecordTraits<MockRecordB>::CATEGORY, CategoryValueProviderType> CategoryFieldSetterType;

	// typedefs for the 'type' field
	typedef CombinedPrFunction<Enum> TypePrFunctionType;
	typedef ContextFieldRangeProvider<Enum, MockRecordB, CategoryFieldSetterType> TypeRangeProviderType;
	typedef ClusteredValueProvider<Enum, MockRecordB, TypePrFunctionType, TypeRangeProviderType> TypeValueProviderType;
	typedef FieldSetter<MockRecordB, RecordTraits<MockRecordB>::TYPE, TypeValueProviderType> TypeFieldSetterType;

    MockRecordBHydratorChain(OperationMode& opMode, RandomStream& random, GeneratorConfig& config) :
        HydratorChain<MockRecordB>(opMode, random),
        // cardinality
        _sequenceCardinality(config.cardinality("mock_record_b")),
        // components for the 'category' field
    	_categoryPrFunction((CombinedPrFunctionInputFactory<Enum>()).getFunction<10>()),
    	_categoryRangeProvider(0, _sequenceCardinality),
    	_categoryValueProvider(*_categoryPrFunction.get(), _categoryRangeProvider),
    	_categoryFieldSetter(_categoryValueProvider),
    	// components for the 'type' field
    	_typePrFunction((CombinedPrFunctionInputFactory<Enum>()).getFunction<11>()),
    	_typeRangeProvider(_categoryFieldSetter),
    	_typeValueProvider(*_typePrFunction.get(), _typeRangeProvider),
    	_typeFieldSetter(_typeValueProvider),
        _logger(Logger::get("mock_record_b.hydrator"))
    {
    }

    virtual ~MockRecordBHydratorChain()
    {
    }

    /**
     * Object hydrating function.
     */
    void operator()(AutoPtr<MockRecordB> recordPtr) const
    {
        ensurePosition(recordPtr->genID());

        MockRecordBHydratorChain* me = const_cast<MockRecordBHydratorChain*>(this);

        // apply setter chain
        me->_categoryFieldSetter(recordPtr, me->_random);
        me->_typeFieldSetter(recordPtr, me->_random);
    }

	/**
	 * Predicate filter function.
	 */
	virtual Interval<I64u> filter(const EqualityPredicate<MockRecordB>& predicate)
	{
		Interval<I64u> result(0, _sequenceCardinality);

		// apply inverse setter chain
		_categoryFieldSetter.filterRange(predicate, result);
		_typeFieldSetter.filterRange(predicate, result);

		return result;
	}

protected:

	// cardinality
	I64u _sequenceCardinality;

	// components for the 'category' field
	AutoPtr<CategoryPrFunctionType> _categoryPrFunction;
	CategoryRangeProviderType _categoryRangeProvider;
	CategoryValueProviderType _categoryValueProvider;
	CategoryFieldSetterType _categoryFieldSetter;

	// components for the 'type' field
	AutoPtr<TypePrFunctionType> _typePrFunction;
	TypeRangeProviderType _typeRangeProvider;
	TypeValueProviderType _typeValueProvider;
	TypeFieldSetterType _typeFieldSetter;

    /**
     * Logger instance.
     */
    Logger& _logger;
};

}  // namespace Myriad

#endif /* MOCKRECORDBHYDRATORCHAIN_H_ */
