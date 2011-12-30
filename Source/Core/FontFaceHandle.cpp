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
#include <Rocket/Core/FontFaceHandle.h>
#include <algorithm>
#include <Rocket/Core.h>
#include "FontFaceLayer.h"
#include "TextureLayout.h"

namespace Rocket {
namespace Core {

class FontEffectSort
{
public:
	bool operator()(const FontEffect* lhs, const FontEffect* rhs)
	{
		return lhs->GetZIndex() < rhs->GetZIndex();
	}
};

FontFaceHandle::FontFaceHandle()
{
	size = 0;
	average_advance = 0;
	x_height = 0;
	line_height = 0;
	baseline = 0;

	underline_position = 0;
	underline_thickness = 0;

	base_layer = NULL;
}

FontFaceHandle::~FontFaceHandle()
{
	for (FontGlyphMap::iterator i = glyphs.begin(); i != glyphs.end(); ++i)
		delete[] i->second.bitmap_data;

	for (FontLayerMap::iterator i = layers.begin(); i != layers.end(); ++i)
		delete i->second;
}

// Returns the point size of this font face.
int FontFaceHandle::GetSize() const
{
	return size;
}

// Returns the average advance of all glyphs in this font face.
int FontFaceHandle::GetCharacterWidth() const
{
	return average_advance;
}

// Returns the pixel height of a lower-case x in this font face.
int FontFaceHandle::GetXHeight() const
{
	return x_height;
}

// Returns the default height between this font face's baselines.
int FontFaceHandle::GetLineHeight() const
{
	return line_height;
}

// Returns the font's baseline.
int FontFaceHandle::GetBaseline() const
{
	return baseline;
}

// Returns the font's glyphs.
const FontGlyphMap& FontFaceHandle::GetGlyphs() const
{
	return glyphs;
}

// Returns the width a string will take up if rendered with this handle.
int FontFaceHandle::GetStringWidth(const WString& string, word prior_character) const
{
	int width = 0;

	for (size_t i = 0; i < string.Length(); i++)
	{
		word character_code = string[i];

		FontGlyphMap::const_iterator iterator = glyphs.find(character_code);
		if (iterator == glyphs.end())
			continue;

		// Adjust the cursor for the kerning between this character and the previous one.
		if (prior_character != 0)
			width += GetKerning(prior_character, string[i]);
		// Adjust the cursor for this character's advance.
		width += iterator->second.advance;

		prior_character = character_code;
	}

	return width;
}

// Generates the texture data for a layer (for the texture database).
bool FontFaceHandle::GenerateLayerTexture(const byte*& texture_data, Vector2i& texture_dimensions, FontEffect* layer_id, int texture_id)
{
	FontLayerMap::iterator layer_iterator = layers.find(layer_id);
	if (layer_iterator == layers.end())
		return false;

	return layer_iterator->second->GenerateTexture(texture_data, texture_dimensions, texture_id);
}

// Generates the geometry required to render a single line of text.
int FontFaceHandle::GenerateString(GeometryList& geometry, const WString& string, const Vector2f& position, const Colourb& colour, int layer_configuration_index) const
{
	int geometry_index = 0;
	int line_width = 0;

	ROCKET_ASSERT(layer_configuration_index >= 0);
	ROCKET_ASSERT(layer_configuration_index < (int) layer_configurations.size());

	// Fetch the requested configuration and generate the geometry for each one.
	const LayerConfiguration& layer_configuration = layer_configurations[layer_configuration_index];
	for (size_t i = 0; i < layer_configuration.size(); ++i)
	{
		FontFaceLayer* layer = layer_configuration[i];

		Colourb layer_colour;
		if (layer == base_layer)
			layer_colour = colour;
		else
			layer_colour = layer->GetColour();

		// Resize the geometry list if required.
		if ((int) geometry.size() < geometry_index + layer->GetNumTextures())
			geometry.resize(geometry_index + layer->GetNumTextures());

		// Bind the textures to the geometries.
		for (int i = 0; i < layer->GetNumTextures(); ++i)
			geometry[geometry_index + i].SetTexture(layer->GetTexture(i));

		line_width = 0;
		word prior_character = 0;

		const word* string_iterator = string.CString();
		const word* string_end = string.CString() + string.Length();

		for (; string_iterator != string_end; string_iterator++)
		{
			FontGlyphMap::const_iterator iterator = glyphs.find(*string_iterator);
			if (iterator == glyphs.end())
				continue;

			// Adjust the cursor for the kerning between this character and the previous one.
			if (prior_character != 0)
				line_width += GetKerning(prior_character, *string_iterator);

			layer->GenerateGeometry(&geometry[geometry_index], *string_iterator, Vector2f(position.x + line_width, position.y), layer_colour);

			line_width += iterator->second.advance;
			prior_character = *string_iterator;
		}

		geometry_index += layer->GetNumTextures();
	}

	// Cull any excess geometry from a previous generation.
	geometry.resize(geometry_index);

	return line_width;
}

// Generates the geometry required to render a line above, below or through a line of text.
void FontFaceHandle::GenerateLine(Geometry* geometry, const Vector2f& position, int width, Font::Line height, const Colourb& colour) const
{
	std::vector< Vertex >& line_vertices = geometry->GetVertices();
	std::vector< int >& line_indices = geometry->GetIndices();

	float offset;
	switch (height)
	{
		case Font::UNDERLINE:			offset = -underline_position; break;
		case Font::OVERLINE:			// where to place? offset = -line_height - underline_position; break;
		case Font::STRIKE_THROUGH:		// where to place? offset = -line_height * 0.5f; break;
		default:						return;
	}

	line_vertices.resize(line_vertices.size() + 4);
	line_indices.resize(line_indices.size() + 6);
	GeometryUtilities::GenerateQuad(&line_vertices[0] + (line_vertices.size() - 4), &line_indices[0] + (line_indices.size() - 6), Vector2f(position.x, position.y + offset), Vector2f((float) width, underline_thickness), colour, line_vertices.size() - 4);
}

// Returns the font face's raw charset (the charset range as a string).
const String& FontFaceHandle::GetRawCharset() const
{
	return raw_charset;
}

// Returns the font face's charset.
const UnicodeRangeList& FontFaceHandle::GetCharset() const
{
	return charset;
}

// Destroys the handle.
void FontFaceHandle::OnReferenceDeactivate()
{
	delete this;
}


int FontFaceHandle::GetKerning(word lhs, word rhs) const
{
	FontKerningMap::const_iterator rhs_iterator = kerning.find(rhs);
	if (rhs_iterator == kerning.end())
		return 0;

	GlyphKerningMap::const_iterator lhs_iterator = rhs_iterator->second.find(lhs);
	if (lhs_iterator == rhs_iterator->second.end())
		return 0;

	return lhs_iterator->second;
}


// Generates (or shares) a layer derived from a font effect.
FontFaceLayer* FontFaceHandle::GenerateLayer(FontEffect* font_effect)
{
    // See if this effect has been instanced before, as part of a different configuration.
    FontLayerMap::iterator i = layers.find(font_effect);
    if (i != layers.end())
        return i->second;

    FontFaceLayer* layer = new FontFaceLayer();
    layers[font_effect] = layer;

    if (font_effect == NULL)
    {
        layer->Initialise(this);
    }
    else
    {
        // Determine which, if any, layer the new layer should copy its geometry and textures from.
        FontFaceLayer* clone = NULL;
        bool deep_clone = true;
        String generation_key;

        if (!font_effect->HasUniqueTexture())
        {
            clone = base_layer;
            deep_clone = false;
        }
        else
        {
            generation_key = font_effect->GetName() + ";" + font_effect->GetGenerationKey();
            FontLayerCache::iterator cache_iterator = layer_cache.find(generation_key);
            if (cache_iterator != layer_cache.end())
                clone = cache_iterator->second;
        }

        // Create a new layer.
        layer->Initialise(this, font_effect, clone, deep_clone);

        // Cache the layer in the layer cache if it generated its own textures (ie, didn't clone).
        if (clone == NULL)
            layer_cache[generation_key] = layer;
    }

    return layer;
}


}
}
