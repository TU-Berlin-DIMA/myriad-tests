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

#ifndef PRODUCTCLASS_H_
#define PRODUCTCLASS_H_

#include "record/AbstractRecord.h"

namespace Myriad {

class ProductClass: public AbstractRecord
{
public:

	ProductClass()
	{
	}

	ProductClass(map<string, Any> properties)
	{
		// metas
		genID(AnyCast<ID>(properties["gen_id"]));
		alias(AnyCast<String>(properties["pc_meta_alias"]));
		// fields
		classSK(fromString<ID>(AnyCast<String>(properties["pc_class_sk"])));
		className(AnyCast<String>(properties["pc_class_name"]));
	}

	const String& alias() const;
	void alias(const String& v);

	ID classSK() const;
	void classSK(const ID& v);

	const String& className() const;
	void className(const String& v);

	ID parentSK() const;
	void parentSK(const ID& v);

	I16u left() const;
	void left(const I16u& v);

	I16u right() const;
	void right(const I16u& v);

	// field names for the 'field_' members
	static const string CLASS_SK;
	static const string CLASS_NAME;
	static const string PARENT_SK;
	static const string LEFT;
	static const string RIGHT;

private:
	// meta fields
	String meta_alias;

	// data fields
	ID field_product_class_sk;
	String field_name;
	ID field_parent_sk;
	I16u field_left;
	I16u field_right;
};

inline const String& ProductClass::alias() const
{
	return meta_alias;
}

inline void ProductClass::alias(const String& v)
{
	meta_alias = v;
}

inline ID ProductClass::classSK() const
{
	return field_product_class_sk;
}

inline void ProductClass::classSK(const ID& v)
{
	field_product_class_sk = v;
}

inline const String& ProductClass::className() const
{
	return field_name;
}

inline void ProductClass::className(const String& v)
{
	field_name = v;
}

inline ID ProductClass::parentSK() const
{
	return field_parent_sk;
}

inline void ProductClass::parentSK(const ID& v)
{
	field_parent_sk = v;
}

inline I16u ProductClass::left() const
{
	return field_left;
}

inline void ProductClass::left(const I16u& v)
{
	field_left = v;
}

inline I16u ProductClass::right() const
{
	return field_right;
}

inline void ProductClass::right(const I16u& v)
{
	field_right = v;
}

} // namespace Myriad

#endif /* PRODUCTCLASS_H_ */
