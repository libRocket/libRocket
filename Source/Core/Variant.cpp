/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "precompiled.h"
#include "../../Include/Rocket/Core/Variant.h"

namespace Rocket {
namespace Core {

Variant::Variant() : type(NONE)
{
	// Make sure our object size assumptions fit inside the static buffer
	ROCKET_STATIC_ASSERT(sizeof(Colourb) <= LOCAL_DATA_SIZE, LOCAL_DATA_TOO_SMALL_FOR_Colourb);
	ROCKET_STATIC_ASSERT(sizeof(Colourf) <= LOCAL_DATA_SIZE, LOCAL_DATA_TOO_SMALL_FOR_Colourf);
	ROCKET_STATIC_ASSERT(sizeof(String) <= LOCAL_DATA_SIZE, LOCAL_DATA_TOO_SMALL_FOR_String);
}

Variant::Variant(const Variant& copy) : type(NONE) { Set(copy); }

Variant::~Variant() { Clear(); }

void Variant::Clear()
{
	// Free any allocated types.
	switch (type) {
	case STRING: {
		// Clean up the string.
		String* string = (String*)&data;
		string->~String();
		break;
	}
	case VECTOR2: {
		auto d = (Vector2f*)&data;
		d->~Vector2f();
		break;
	}
	case COLOURF: {
		auto d = (Colourf*)&data;
		d->~Colourf();
		break;
	}
	case COLOURB: {
		auto d = (Colourb*)&data;
		d->~Colourb();
		break;
	}

	default:
		break;
	}
	type = NONE;
}

Variant::Type Variant::GetType() const { return type; }

//////////////////////////////////////////////////
// Set methods
//////////////////////////////////////////////////

#define SET_VARIANT(type) Clear(); new (std::addressof(data))(type)(value)

void Variant::Set(const Variant& copy)
{
	switch (copy.type) {
	case STRING: {
		// Create the string
		Set(*(String*)std::addressof(copy.data));
		break;
	}
	case VECTOR2: {
		Set(*(Vector2f*)std::addressof(copy.data));
		break;
	}
	case COLOURF: {
		Set(*(Colourf*)std::addressof(copy.data));
		break;
	}
	case COLOURB: {
		Set(*(Colourb*)std::addressof(copy.data));
		break;
	}

	default:
		Clear();
		memcpy(std::addressof(data), std::addressof(copy.data), LOCAL_DATA_SIZE);
		break;
	}
	type = copy.type;
}

void Variant::Set(const byte value)
{
	SET_VARIANT(byte);
	type = BYTE;
}

void Variant::Set(const char value)
{
	SET_VARIANT(char);
	type = CHAR;
}

void Variant::Set(const float value)
{
	SET_VARIANT(float);
	type = FLOAT;
}

void Variant::Set(const int value)
{
	SET_VARIANT(int);
	type = INT;
}

void Variant::Set(const String& value)
{
	if (type == STRING) {
		((String*)&data)->Assign(value);
	} else {
		SET_VARIANT(String);
	}
	type = STRING;
}

void Variant::Set(const word value)
{
	SET_VARIANT(word);
	type = WORD;
}

void Variant::Set(const char* value) { Set(String(value)); }

void Variant::Set(void* value)
{
	SET_VARIANT(void*);
	type = VOIDPTR;
}

void Variant::Set(const Vector2f& value)
{
	SET_VARIANT(Vector2f);
	type = VECTOR2;
}

void Variant::Set(const Colourf& value)
{
	SET_VARIANT(Colourf);
	type = COLOURF;
}

void Variant::Set(const Colourb& value)
{
	SET_VARIANT(Colourb);
	type = COLOURB;
}

void Variant::Set(ScriptInterface* value)
{
	SET_VARIANT(ScriptInterface*);
	type = SCRIPTINTERFACE;
}

Variant& Variant::operator=(const Variant& copy)
{
	Set(copy);
	return *this;
}

} // namespace Core
} // namespace Rocket
