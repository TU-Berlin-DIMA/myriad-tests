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
 * @author: Alexander Alexandrov <alexander@alex.androv.eu>
 */

#ifndef CARD_H_
#define CARD_H_

#include "record/Record.h"

namespace Myriad {

class Card: public Record
{
public:

	enum SuiteType
	{
		CLUB, DIAMOND, HEART, SPADE
	};

	void surrogateKey(const ID& v);
	const ID& surrogateKey() const;

	void type(const String& v);
	const String& type() const;

	void suite(const SuiteType& v);
	const SuiteType& suite() const;

	void clusterID(const I16u& v);
	const I16u& clusterID() const;

	void clusterInterval(const Interval<ID>& v);
	const Interval<ID>& clusterInterval() const;

private:

	ID field_surrogate_key;
	String field_type;
	SuiteType field_suite;
	I16u field_cluster_id;
	Interval<ID> field_cluster_interval;
};

inline void Card::surrogateKey(const ID& v)
{
	field_surrogate_key = v;
}

inline const ID& Card::surrogateKey() const
{
	return field_surrogate_key;
}

inline void Card::type(const String& v)
{
	field_type = v;
}

inline const String& Card::type() const
{
	return field_type;
}

inline void Card::suite(const Card::SuiteType& v)
{
	field_suite = v;
}

inline const Card::SuiteType& Card::suite() const
{
	return field_suite;
}

inline void Card::clusterID(const I16u& v)
{
	field_cluster_id = v;
}

inline const I16u& Card::clusterID() const
{
	return field_cluster_id;
}

inline void Card::clusterInterval(const Interval<ID>& v)
{
	field_cluster_interval = v;
}

inline const Interval<ID>& Card::clusterInterval() const
{
	return field_cluster_interval;
}


} // namespace Myriad

#endif /* CARD_H_ */
