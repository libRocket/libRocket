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
#include "TextureLayout.h"
#include <algorithm>
#include "TextureLayoutRectangle.h"
#include "TextureLayoutTexture.h"

namespace Rocket {
namespace Core {

struct RectangleSort
{
	bool operator()(const TextureLayoutRectangle* lhs, const TextureLayoutRectangle* rhs) const
	{
		return lhs->GetDimensions().y > rhs->GetDimensions().y;
	}
};

TextureLayout::TextureLayout()
{
}

TextureLayout::~TextureLayout()
{
  for (TextureList::iterator i = textures.begin(); i != textures.end(); ++i)
    (*i)->RemoveReference();
  for (RectangleList::iterator i = rectangles.begin(); i != rectangles.end(); ++i)
    (*i)->RemoveReference();
}

// Adds a rectangle to the list of rectangles to be laid out.
void TextureLayout::AddRectangle(int id, const Vector2i& dimensions)
{
	rectangles.push_back(new TextureLayoutRectangle(id, dimensions));
}

// Returns one of the layout's rectangles.
TextureLayoutRectangle& TextureLayout::GetRectangle(int index)
{
	ROCKET_ASSERT(index >= 0);
	ROCKET_ASSERT(index < GetNumRectangles());

	return *rectangles[index];
}

// Returns the number of rectangles in the layout.
int TextureLayout::GetNumRectangles() const
{
	return (int) rectangles.size();
}

// Returns one of the layout's textures.
TextureLayoutTexture& TextureLayout::GetTexture(int index)
{
	ROCKET_ASSERT(index >= 0);
	ROCKET_ASSERT(index < GetNumTextures());

	return *textures[index];
}

// Returns the number of textures in the layout.
int TextureLayout::GetNumTextures() const
{
	return (int) textures.size();
}

// Attempts to generate an efficient texture layout for the rectangles.
bool TextureLayout::GenerateLayout(int max_texture_dimensions)
{
	// Sort the rectangles by height.
	std::sort(rectangles.begin(), rectangles.end(), RectangleSort());

	int num_placed_rectangles = 0;
	while (num_placed_rectangles != GetNumRectangles())
	{
		TextureLayoutTexture *texture = new TextureLayoutTexture();
		int texture_size = texture->Generate(*this, max_texture_dimensions);
		if (texture_size == 0)
    {
      texture->RemoveReference();
			return false;
    }

		textures.push_back(texture);
		num_placed_rectangles += texture_size;
	}

	return true;
}

TextureLayout& TextureLayout::operator+=(const TextureLayout &layout)
{
  const int texture_size = GetNumTextures();

  for (TextureList::const_iterator i = layout.textures.begin(); i != layout.textures.end(); ++i)
  {
    TextureLayoutTexture *texture = *i;

    texture->AddReference();
    textures.push_back(texture);
  }
  for (RectangleList::const_iterator i = layout.rectangles.begin(); i != layout.rectangles.end(); ++i)
  {
    TextureLayoutRectangle *rect = *i;

    rect->AddReference();
    rect->Place(texture_size + rect->GetTextureIndex(), rect->GetPosition());
    rectangles.push_back(rect);
  }

  return *this;
}

}
}
