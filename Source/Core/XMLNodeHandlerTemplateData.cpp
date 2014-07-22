/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2011 Frank Becker
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
#include "XMLNodeHandlerTemplateData.h"
#include "Template.h"
#include "TemplateCache.h"
#include "XMLParseTools.h"
#include <Rocket/Core.h>

namespace Rocket {
namespace Core {

XMLNodeHandlerTemplateData::XMLNodeHandlerTemplateData()
{
}

XMLNodeHandlerTemplateData::~XMLNodeHandlerTemplateData()
{
}

Element* XMLNodeHandlerTemplateData::ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes)
{
	// Tell the parser to use the element handler for all child nodes
	parser->PushDefaultHandler();

	Element *parent = parser->GetParseFrame()->element;
	String itemName = attributes.Get<String>("name","");

	//At this point we've already inserted the template body.
	//Find all template_sub elements from the template body and see if any match this element
	ElementList templateDataElements;
	parent->GetElementsByTagName(templateDataElements, "template_sub");

	for (ElementList::iterator itr = templateDataElements.begin(); itr != templateDataElements.end(); ++itr)
	{
		String name = (*itr)->GetAttribute<String>("name","");
		if( name == itemName)
		{
			//it's a match, now replace the default contents of this node
			Element *item = (*itr);
			if( item->HasChildNodes())
			{
				//shouldn't have more than one child (default content)
				item->RemoveChild( item->GetFirstChild());
			}

			return item;
		}
	}

	return parent;
}

}
}
