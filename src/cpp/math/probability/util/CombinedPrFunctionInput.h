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

#ifndef COMBINEDPRFUNCTIONINPUT_H_
#define COMBINEDPRFUNCTIONINPUT_H_

#include "math/probability/CombinedPrFunction.h"

#include <Poco/RefCountedObject.h>

#include <cmath>

using namespace Poco;
using namespace std;

namespace Myriad {

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// utility class for storing and checking CombinedPrFunction configurations
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

template<typename T> class CombinedPrFunctionInput : public RefCountedObject
{
public:

    typedef Interval<T> TInterval;

    CombinedPrFunctionInput(double nullProbability, T testRangeMin, T testRangeMax) :
        _nullProbability(nullProbability),
        _testRange(testRangeMin, testRangeMax)
    {
        _domain.set(numericLimits<T>::max(), numericLimits<T>::min());
    }

    CombinedPrFunctionInput(const CombinedPrFunctionInput& o) :
        _nullProbability(o._nullProbability),
        _buckets(o._buckets),
        _exactValueBuckets(o._exactValueBuckets),
        _multiValueBuckets(o._multiValueBuckets),
        _pdf(o._pdf),
        _cdf(o._cdf),
        _domain(o._domain)
    {
    }

    T testRangeMin()
    {
        return _testRange.min();
    }

    T testRangeMax()
    {
        return _testRange.max();
    }

    CombinedPrFunctionInput<T>& add(double probability, const T& x)
    {
        size_t i = _buckets.size();

        _buckets.push_back(Interval<T>(x, x+1));
        _pdf.push_back(probability);
        _cdf.push_back(i > 0 ? _cdf[i-1] + probability : probability);

        _exactValueBuckets.push_back(i);

        _domain.set(min(_domain.min(), x), max(_domain.max(), static_cast<T>(x+1)));

        return *this;
    }

    CombinedPrFunctionInput<T>& add(double probability, const T& l, const T& u)
    {
        size_t i = _buckets.size();

        _buckets.push_back(Interval<T>(l, u));
        _pdf.push_back(probability);
        _cdf.push_back(i > 0 ? _cdf[i-1] + probability : probability);

        _multiValueBuckets.push_back(i);

        _domain.set(min(_domain.min(), l), max(_domain.max(), u));

        return *this;
    }

    double pdf(const T& x)
    {
        if (nullValue<T>() == x)
        {
            return _nullProbability;
        }
        else if (_domain.contains(x))
        {
            size_t i = lookup(x);
            return _pdf[i]/static_cast<double>(_buckets[i].length());
        }
        else
        {
            return 0.0;
        }
    }

    double cdf(const T& x)
    {
        if (_domain.min() > x)
        {
            return 0.0;
        }
        else if (_domain.max() <= x)
        {
            return 1.0 - _nullProbability;
        }
        else
        {
            size_t i = lookup(x);
            if (i > 0)
            {
                return _cdf[i-1] + _pdf[i] * (x + 1 - _buckets[i].min())/static_cast<double>(_buckets[i].length());
            }
            else
            {
                return _pdf[i] * (x + 1 - _buckets[i].min())/static_cast<double>(_buckets[i].length());
            }
        }
    }

    std::istream& serialize()
    {
        _in.clear();

        _in << "@numberofexactvals = " << _exactValueBuckets.size()  << "\n"
            << "@numberofbins = "      << _multiValueBuckets.size()  << "\n"
            << "@nullprobability = "   << _nullProbability           << "\n";

        _in << "# some empty lines and lines with comments\n"
            << "\t\n"
            << " \t\n"
            << " \t\n"
            << "\t \n"
            << "\t \t\t \t   \t # comment here \n";

        _in << "# exact value probabilities\n";
        for (size_t j = 0; j < _exactValueBuckets.size(); j++)
        {
            size_t i = _exactValueBuckets[j];
            _in << "p(X) = " << _pdf[i] << "    for X = { " << _buckets[i].min() << " }\n";
        }

        _in << "# bucket probabilities\n";
        for (size_t j = 0; j < _multiValueBuckets.size(); j++)
        {
            size_t i = _multiValueBuckets[j];
            _in << "p(X) = " << _pdf[i] << "    for X = { x in [" << _buckets[i].min() << ", " << _buckets[i].max() << ") }\n";
        }

        return _in;
    }

    const TInterval& domain() const
    {
    	return _domain;
    }

private:

    size_t lookup(const T& x)
    {
        // we assert that the value x is in the [_min, _max] range
        int min = 0;
        int max = _buckets.size() - 1;
        int mid = 0;

        // continue searching while [min, max] is not empty
        while (max >= min)
        {
            // calculate the midpoint for roughly equal partition //
            mid = (min + max) / 2;

            // determine which subarray to search
            if (_buckets[mid].max() <=  x)
            {
                // change min index to search upper subarray
                min = mid + 1;
            }
            else if (_buckets[mid].min() > x)
            {
                // change max index to search lower subarray
                max = mid - 1;
            }
            else
            {
                // key found at index mid
                return mid;
            }
        }

        return mid;
    }

    double _nullProbability;

    std::vector<TInterval> _buckets;

    std::vector<size_t> _exactValueBuckets;

    std::vector<size_t> _multiValueBuckets;

    std::vector<double> _pdf;

    std::vector<double> _cdf;

    TInterval _domain;

    TInterval _testRange;

    std::stringstream _in;
};

template<typename T>
class CombinedPrFunctionInputFactory
{
public:

    template<I16u functionID> CombinedPrFunctionInput<T>* getFunctionInput()
    {
        // Unsupported base parameter type T
        throw std::exception();
    }

    template<I16u functionID> CombinedPrFunction<T>* getFunction()
    {
    	AutoPtr< CombinedPrFunctionInput<T> > prFunctionInput = getFunctionInput<functionID>();
    	return new CombinedPrFunction<T>(prFunctionInput->serialize());
    }
};

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// factory specializations with various CombinedPrFunction configurations
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

template<> template<>
CombinedPrFunctionInput<I16u>* CombinedPrFunctionInputFactory<I16u>::getFunctionInput<0>()
{
    CombinedPrFunctionInput<I16u>* input = new CombinedPrFunctionInput<I16u>(0.04000, 0, 100);

    input->add(0.100, 10, 29);
    input->add(0.100, 29);
    input->add(0.100, 30, 34);
    input->add(0.100, 34);
    input->add(0.100, 35, 43);
    input->add(0.100, 43);
    input->add(0.100, 44);
    input->add(0.100, 45, 59);
    input->add(0.100, 59);
    input->add(0.100, 60, 90);

    return input;
}

template<> template<>
CombinedPrFunctionInput<I16u>* CombinedPrFunctionInputFactory<I16u>::getFunctionInput<1>()
{
    CombinedPrFunctionInput<I16u>* input = new CombinedPrFunctionInput<I16u>(0.100, 0, 9);

    input->add(0.100, 0, 5);
    input->add(0.800, 5, 9);

    return input;
}

template<> template<>
CombinedPrFunctionInput<I16u>* CombinedPrFunctionInputFactory<I16u>::getFunctionInput<2>()
{
    CombinedPrFunctionInput<I16u>* input = new CombinedPrFunctionInput<I16u>(0.000, 1, 50);

    input->add(0.150,  1, 10);
    input->add(0.250, 10, 20);
    input->add(0.300, 20, 30);
    input->add(0.250, 30, 40);
    input->add(0.150, 40, 50);

    return input;
}

template<> template<>
CombinedPrFunctionInput<I64u>* CombinedPrFunctionInputFactory<I64u>::getFunctionInput<0>()
{
    CombinedPrFunctionInput<I64u>* input = new CombinedPrFunctionInput<I64u>(0.04000, 0, 100);

    input->add(0.1900, 10, 29);
    input->add(0.0700, 29);
    input->add(0.0400, 30, 34);
    input->add(0.0100, 34);
    input->add(0.0800, 35, 43);
    input->add(0.0500, 43);
    input->add(0.0500, 44);
    input->add(0.1400, 45, 59);
    input->add(0.0300, 59);
    input->add(0.3000, 60, 90);

    return input;
}

template<> template<>
CombinedPrFunctionInput<Enum>* CombinedPrFunctionInputFactory<Enum>::getFunctionInput<10>()
{
    CombinedPrFunctionInput<Enum>* input = new CombinedPrFunctionInput<Enum>(0.0, 0, 5);

    input->add(0.200, 0);
    input->add(0.200, 1);
    input->add(0.200, 2);
    input->add(0.200, 3);
    input->add(0.200, 4);

    return input;
}

template<> template<>
CombinedPrFunctionInput<Enum>* CombinedPrFunctionInputFactory<Enum>::getFunctionInput<11>()
{
    CombinedPrFunctionInput<Enum>* input = new CombinedPrFunctionInput<Enum>(0.0, 0, 2);

    input->add(0.500, 0);
    input->add(0.500, 1);

    return input;
}

template<> template<>
CombinedPrFunctionInput<Date>* CombinedPrFunctionInputFactory<Date>::getFunctionInput<0>()
{
    CombinedPrFunctionInput<Date>* input = new CombinedPrFunctionInput<Date>(0.04000, Date("1992-04-01"), Date("1992-07-15"));

    input->add(0.1900, "1992-04-10", "1992-04-29");
    input->add(0.0700, "1992-04-29");
    input->add(0.0400, "1992-04-30", "1992-05-04");
    input->add(0.0100, "1992-05-04");
    input->add(0.0800, "1992-05-05", "1992-05-13");
    input->add(0.0500, "1992-05-13");
    input->add(0.0500, "1992-05-14");
    input->add(0.1400, "1992-05-15", "1992-05-29");
    input->add(0.0300, "1992-05-29");
    input->add(0.3000, "1992-05-30", "1992-06-29");

    return input;
}

} // namespace Myriad

#endif /* COMBINEDPRFUNCTIONTEST_H_ */
