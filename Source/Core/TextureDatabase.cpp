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
  *  --== Changes ==--
 *  20 Feb 2012     Edited to support the ImageSourceListener interface     Matthew Alan Gray <mgray@hatboystudios.com>
 *  29 Feb 2012     Adding texture loading code in Fetch(ImageSource*)      Matthew Alan Gray <mgray@hatboystudios.com>
 *   2 Mar 2012     Refactored Fetch(ImageSource*) to work with recent      Matthew Alan Gray <mgray@hatboystudios.com>
 *                  changes.
 */

#include "precompiled.h"
#include "TextureDatabase.h"
#include "TextureResource.h"
#include <Rocket/Core.h>

namespace Rocket {
namespace Core {

static TextureDatabase* instance = NULL;

TextureDatabase::TextureDatabase()
{
	ROCKET_ASSERT(instance == NULL);
	instance = this;
}

TextureDatabase::~TextureDatabase()
{
	ROCKET_ASSERT(instance == this);
	instance = NULL;
}

void TextureDatabase::Initialise()
{
	new TextureDatabase();
}

void TextureDatabase::Shutdown()
{
	delete instance;
}

// If the requested texture is already in the database, it will be returned with an extra reference count. If not, it
// will be loaded through the application's render interface.
TextureResource* TextureDatabase::Fetch(const String& source, const String& source_directory)
{
	String path;
	if (source.Substring(0, 1) == "?")
		path = source;
	else
		GetSystemInterface()->JoinPath(path, source_directory.Replace("|", ":"), source);

	TextureMap::iterator iterator = instance->textures.find(path);
	if (iterator != instance->textures.end())
	{
		(*iterator).second->AddReference();
		return (*iterator).second;
	}

	TextureResource* resource = new TextureResource();
	if (!resource->Load(path))
	{
		resource->RemoveReference();
		return NULL;
	}

	instance->textures[resource->GetSource()] = resource;
	return resource;
}

// If the requested texture is already in the database, it will be returned with an extra reference count. If not, it
// will be generated through the application's render interface.
TextureResource* TextureDatabase::Fetch(ImageSource* image_source)
{
    TextureResource* resource = NULL;
    Rocket::Core::String name("?image_source::");
    name += image_source->GetImageSourceName();
    TextureMap::iterator iterator = instance->textures.find(name);
    if (iterator != instance->textures.end())
    {
        resource = iterator->second;
        resource->AddReference();
    }
    else
    {
        resource = new TextureResource();
        resource->Load(name);
        instance->textures[name] = resource;
    }

    RenderInterface* renderInterface = GetRenderInterface();
    resource->Load(renderInterface, image_source);
    return resource;
}

// Releases all textures in the database.
void TextureDatabase::ReleaseTextures()
{
	for (TextureMap::iterator i = instance->textures.begin(); i != instance->textures.end(); ++i)
		i->second->Release();
}

// Removes a texture from the database.
void TextureDatabase::RemoveTexture(TextureResource* texture)
{
	if (instance != NULL)
	{
		TextureMap::iterator iterator = instance->textures.find(texture->GetSource());
		if (iterator != instance->textures.end())
			instance->textures.erase(iterator);
	}
}

// Release all textures bound through a render interface.
void TextureDatabase::ReleaseTextures(RenderInterface* render_interface)
{
	if (instance != NULL)
	{
		for (TextureMap::iterator i = instance->textures.begin(); i != instance->textures.end(); ++i)
			i->second->Release(render_interface);
	}
}

}
}
