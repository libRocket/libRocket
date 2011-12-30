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
#include <Rocket/Core/FontDatabase.h>
#include <Rocket/Core.h>
#include <Rocket/Core/FontProvider.h>


namespace Rocket {
namespace Core {

FontDatabase* FontDatabase::instance = NULL;
FontDatabase::FontProviderTable FontDatabase::font_provider_table;

typedef std::map< String, FontEffect* > FontEffectCache;
FontEffectCache font_effect_cache;

FontDatabase::FontDatabase()
{
	ROCKET_ASSERT(instance == NULL);
	instance = this;
}

FontDatabase::~FontDatabase()
{
	ROCKET_ASSERT(instance == this);
	instance = NULL;
}

bool FontDatabase::Initialise()
{
	if (instance == NULL)
	{
		new FontDatabase();
	}

	return true;
}

void FontDatabase::Shutdown()
{
	if (instance != NULL)
	{
		delete instance;
	}
}

// Returns a handle to a font face that can be used to position and render text.
FontFaceHandle* FontDatabase::GetFontFaceHandle(const String& family, const String& charset, Font::Style style, Font::Weight weight, int size)
{
    size_t provider_index, provider_count;
    
    provider_count = font_provider_table.size();
    
	for( provider_index = 0; provider_index < provider_count; ++provider_index )
    {
        FontFaceHandle * face_handle;
        
        face_handle = font_provider_table[ provider_index ]->GetFontFaceHandle( family, charset, style, weight, size );
        
        if( face_handle )
        {
            return face_handle;
        }
    }
    
    return NULL; 
}

// Returns a font effect, either a newly-instanced effect from the factory or an identical shared
// effect.
FontEffect* FontDatabase::GetFontEffect(const String& name, const PropertyDictionary& properties)
{
	// The caching here should be moved into the Factory for optimal behaviour. This system has a
	// few shortfalls:
	//  * ignores default properties
	//  * could be shared with decorators as well

	// Generate a key so we can distinguish unique property sets quickly.
	typedef std::list< std::pair< String, String > > PropertyList;
	PropertyList sorted_properties;
	for (PropertyMap::const_iterator property_iterator = properties.GetProperties().begin(); property_iterator != properties.GetProperties().end(); ++property_iterator)
	{
		// Skip the font-effect declaration.
		if (property_iterator->first == "font-effect")
			continue;

		PropertyList::iterator insert = sorted_properties.begin();
		while (insert != sorted_properties.end() &&
			   insert->first < property_iterator->first)
		   ++insert;

		sorted_properties.insert(insert, PropertyList::value_type(property_iterator->first, property_iterator->second.Get< String >()));
	}

	// Generate the font effect's key from the properties.
	String key = name + ";";
	for (PropertyList::iterator i = sorted_properties.begin(); i != sorted_properties.end(); ++i)
		key += i->first + ":" + i->second + ";";

	// Check if we have a previously instanced effect.
	FontEffectCache::iterator i = font_effect_cache.find(key);
	if (i != font_effect_cache.end())
	{
		FontEffect* effect = i->second;
		effect->AddReference();

		return effect;
	}

	FontEffect* font_effect = Factory::InstanceFontEffect(name, properties);
	if (font_effect == NULL)
		return NULL;

	font_effect_cache[key] = font_effect;
	return font_effect;
}

// Removes a font effect from the font database's cache.
void FontDatabase::ReleaseFontEffect(const FontEffect* effect)
{
	for (FontEffectCache::iterator i = font_effect_cache.begin(); i != font_effect_cache.end(); ++i)
	{
		if (i->second == effect)
		{
			font_effect_cache.erase(i);
			return;
		}
	}
}

void FontDatabase::AddFontProvider( FontProvider * provider )
{
    instance->font_provider_table.push_back( provider );
}

}
}
