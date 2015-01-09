/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2014 Markus Schöngart
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
#include "PropertyParserTransform.h"

namespace Rocket {
namespace Core {

PropertyParserTransform::PropertyParserTransform()
	: abs_number(PropertyParserNumber::ABS_NUMBER),
	  number(PropertyParserNumber::NUMBER),
	  length(PropertyParserNumber::LENGTH),
	  angle(PropertyParserNumber::ANGLE)
{
}

PropertyParserTransform::~PropertyParserTransform()
{
}

// Called to parse a RCSS transform declaration.
bool PropertyParserTransform::ParseValue(Property& property, const String& value, const ParameterMap& ROCKET_UNUSED(parameters)) const
{
	SharedReference< Transform > transform(new Transform);

	char const* next = value.CString();

	Transforms::NumericValue args[16];

	const PropertyParser* angle1[] = { &angle };
	const PropertyParser* angle2[] = { &angle, &angle };
	const PropertyParser* length1[] = { &length };
	const PropertyParser* length2[] = { &length, &length };
	const PropertyParser* length3[] = { &length, &length, &length };
	const PropertyParser* length3angle1[] = { &length, &length, &length, &angle };
	const PropertyParser* number1[] = { &number };
	const PropertyParser* number2[] = { &number, &number };
	const PropertyParser* number3[] = { &number, &number, &number };
	const PropertyParser* abs_numbers6[] = { &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number };
	const PropertyParser* abs_numbers16[] = { &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number, &abs_number };

	while (strlen(next))
	{
		int bytes_read = 0;

		if ((bytes_read = Scan(next, "perspective", length1, args, 1)))
		{
			transform->AddPrimitive(Transforms::Perspective(args));
		}
		else if ((bytes_read = Scan(next, "matrix", abs_numbers6, args, 6)))
		{
			transform->AddPrimitive(Transforms::Matrix2D(args));
		}
		else if ((bytes_read = Scan(next, "matrix3d", abs_numbers16, args, 16)))
		{
			transform->AddPrimitive(Transforms::Matrix3D(args));
		}
		else if ((bytes_read = Scan(next, "translateX", length1, args, 1)))
		{
			transform->AddPrimitive(Transforms::TranslateX(args));
		}
		else if ((bytes_read = Scan(next, "translateY", length1, args, 1)))
		{
			transform->AddPrimitive(Transforms::TranslateY(args));
		}
		else if ((bytes_read = Scan(next, "translateZ", length1, args, 1)))
		{
			transform->AddPrimitive(Transforms::TranslateZ(args));
		}
		else if ((bytes_read = Scan(next, "translate", length2, args, 2)))
		{
			transform->AddPrimitive(Transforms::Translate2D(args));
		}
		else if ((bytes_read = Scan(next, "translate3d", length3, args, 3)))
		{
			transform->AddPrimitive(Transforms::Translate3D(args));
		}
		else if ((bytes_read = Scan(next, "scaleX", number1, args, 1)))
		{
			transform->AddPrimitive(Transforms::ScaleX(args));
		}
		else if ((bytes_read = Scan(next, "scaleY", number1, args, 1)))
		{
			transform->AddPrimitive(Transforms::ScaleY(args));
		}
		else if ((bytes_read = Scan(next, "scaleZ", number1, args, 1)))
		{
			transform->AddPrimitive(Transforms::ScaleZ(args));
		}
		else if ((bytes_read = Scan(next, "scale", number2, args, 2)))
		{
			transform->AddPrimitive(Transforms::Scale2D(args));
		}
		else if ((bytes_read = Scan(next, "scale3d", number3, args, 3)))
		{
			transform->AddPrimitive(Transforms::Scale3D(args));
		}
		else if ((bytes_read = Scan(next, "rotateX", angle1, args, 1)))
		{
			transform->AddPrimitive(Transforms::RotateX(args));
		}
		else if ((bytes_read = Scan(next, "rotateY", angle1, args, 1)))
		{
			transform->AddPrimitive(Transforms::RotateY(args));
		}
		else if ((bytes_read = Scan(next, "rotateZ", angle1, args, 1)))
		{
			transform->AddPrimitive(Transforms::RotateZ(args));
		}
		else if ((bytes_read = Scan(next, "rotate", angle1, args, 1)))
		{
			transform->AddPrimitive(Transforms::Rotate2D(args));
		}
		else if ((bytes_read = Scan(next, "rotate3d", length3angle1, args, 4)))
		{
			transform->AddPrimitive(Transforms::Rotate3D(args));
		}
		else if ((bytes_read = Scan(next, "skewX", angle1, args, 1)))
		{
			transform->AddPrimitive(Transforms::SkewX(args));
		}
		else if ((bytes_read = Scan(next, "skewY", angle1, args, 1)))
		{
			transform->AddPrimitive(Transforms::SkewY(args));
		}
		else if ((bytes_read = Scan(next, "skew", angle2, args, 2)))
		{
			transform->AddPrimitive(Transforms::Skew2D(args));
		}

		if (bytes_read > 0)
		{
			next += bytes_read;
		}
		else
		{
			return false;
		}
	}

	property.value = Variant(TransformRef(transform));
	property.unit = Property::TRANSFORM;
}

// Destroys the parser.
void PropertyParserTransform::Release()
{
	delete this;
}

// Scan a string for a parameterized keyword with a certain number of numeric arguments.
int PropertyParserTransform::Scan(const char* str, const char* keyword, const PropertyParser** parsers, Transforms::NumericValue* args, int nargs) const
{
	int total_bytes_read = 0, bytes_read = 0;

	/* use the quicker stack-based argument buffer, if possible */
	char *arg = 0;
	char arg_stack[1024];
	std::string arg_heap;
	if (strlen(str) < sizeof(arg_stack))
	{
		arg = arg_stack;
	}
	else
	{
		arg_heap = str;
		arg = &arg_heap[0];
	}

	/* skip leading white space */
	bytes_read = 0;
	sscanf(str, " %n", &bytes_read);
	str += bytes_read;
	total_bytes_read += bytes_read;

	/* find the keyword */
	if (!memcmp(str, keyword, strlen(keyword)))
	{
		bytes_read = strlen(keyword);
		str += bytes_read;
		total_bytes_read += bytes_read;
	}
	else
	{
		return 0;
	}

	/* skip any white space */
	bytes_read = 0;
	sscanf(str, " %n", &bytes_read);
	str += bytes_read;
	total_bytes_read += bytes_read;

	/* find the opening brace */
	bytes_read = 0;
	if (sscanf(str, " ( %n", &bytes_read), bytes_read)
	{
		str += bytes_read;
		total_bytes_read += bytes_read;
	}
	else
	{
		return 0;
	}

	/* parse the arguments */
	for (int i = 0; i < nargs; ++i)
	{
		Property prop;

		bytes_read = 0;
		if (sscanf(str, " %[^,)] %n", arg, &bytes_read), bytes_read
			&& parsers[i]->ParseValue(prop, String(arg), ParameterMap()))
		{
			args[i].number = prop.value.Get<float>();
			args[i].unit = prop.unit;
			str += bytes_read;
			total_bytes_read += bytes_read;
		}
		else
		{
			return 0;
		}

		/* find the comma */
		if (i < nargs - 1)
		{
			bytes_read = 0;
			if (sscanf(str, " , %n", &bytes_read), bytes_read)
			{
				str += bytes_read;
				total_bytes_read += bytes_read;
			}
			else
			{
				return 0;
			}
		}
	}

	/* find the closing brace */
	bytes_read = 0;
	if (sscanf(str, " ) %n", &bytes_read), bytes_read)
	{
		str += bytes_read;
		total_bytes_read += bytes_read;
	}
	else
	{
		return 0;
	}

	return total_bytes_read;
}

}
}
