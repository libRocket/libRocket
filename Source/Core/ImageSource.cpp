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
 *  20 Feb 2012     Initial Creation                                        Matthew Alan Gray <mgray@hatboystudios.com>
 */

#include <Rocket/Core/ImageSource.h>
#include <Rocket/Core/ImageSourceListener.h>
#include <Rocket/Core/Log.h>
#include <algorithm>
#include <map>

namespace Rocket {
namespace Core {

typedef std::map< Rocket::Core::String, ImageSource* > ImageSourceMap;
static ImageSourceMap image_sources;

ImageSource::ImageSource(const Rocket::Core::String& _name)
{
    if (!_name.Empty())
    {
        name = _name;
    }
    else
    {
        name.FormatString(64, "%x", this);
    }
    image_sources[name] = this;
}

ImageSource::~ImageSource()
{
    while (!listeners.empty())
    {
        listeners.front()->OnImageSourceDestroy(this);
        listeners.pop_front();
    }

    ImageSourceMap::iterator iter = image_sources.find(name);
    if (iter != image_sources.end() && iter->second == this)
    {
        image_sources.erase(iter);
    }
}

const Rocket::Core::String& ImageSource::GetImageSourceName()
{
    return name;
}

ImageSource* ImageSource::GetImageSource(const Rocket::Core::String& image_source_name)
{
    ImageSourceMap::iterator iter = image_sources.find(image_source_name);
    if (iter != image_sources.end())
    {
        return iter->second;
    }

    return NULL;
}

void ImageSource::AttachListener(ImageSourceListener* listener)
{
    if (find(listeners.begin(), listeners.end(), listener) != listeners.end())
    {
        ROCKET_ERROR;
        return;
    }

    listeners.push_back(listener);
}

void ImageSource::DetachListener(ImageSourceListener* listener)
{
    ListenerList::iterator iter = find(listeners.begin(), listeners.end(), listener);
    ROCKET_ASSERT(iter != listeners.end());
    if (iter != listeners.end())
    {
        listeners.erase(iter);
    }
}

void* ImageSource::GetScriptObject() const
{
    return NULL;
}

void ImageSource::NotifyImageChange(const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
    ListenerList::iterator iter = listeners.begin();
    while (iter != listeners.end())
    {
        (*iter)->OnImageChange(this, source, source_dimensions);
        iter++;
    }
}

}
}