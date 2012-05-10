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

#ifndef ROCKETCOREIMAGESOURCE_H
#define ROCKETCOREIMAGESOURCE_H

#include <Rocket/Core/Header.h>
#include <Rocket/Core/String.h>
#include <Rocket/Core/Texture.h>
#include <Rocket/Core/Types.h>

#include <list>

namespace Rocket {
namespace Core {

class ImageSourceListener;

/**
    Generic object that provides an interface for requesting an image from a source.
    @author Matthew Alan Gray <mgray@hatboystudios.com>

 *  --== Changes ==--
 *  20 Feb 2012     Initial Creation                                        Matthew Alan Gray <mgray@hatboystudios.com>
 *   2 Mar 2012     Added ImageSource::Update() to support texture update   Matthew Alan Gray <mgray@hatboystudios.com>
 *                  operations within the main render thread in the case
 *                  that the ImageSource instance is updated in a separate
 *                  thread.  This Update() method is called in the 
 *                  ElementImage::OnUpdate() callback.  Also removed byte
 *                  and dimension parameters from NotifyImageChange() since
 *                  this can all be retrieved via GetImage() in the Update()
 *                  method (if necessary).
 */

class ROCKETCORE_API ImageSource
{
public:
    ImageSource(const Rocket::Core::String& name = "");
    virtual ~ImageSource();

    const Rocket::Core::String& GetImageSourceName();
    static ImageSource* GetImageSource(const Rocket::Core::String& image_source_name);

    /// Fetches an image from the image source.
    /// @param[out] source The source image bytes.
    /// @param[out] source_dimensions The source image dimensions.
    virtual void GetImage(Rocket::Core::RenderInterface* render_interface, Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& source_dimensions) = 0;

    void AttachListener(ImageSourceListener* listener);
    void DetachListener(ImageSourceListener* listener);

    virtual void* GetScriptObject() const;

    /// Refreshes the image source if required, calling NotifyImageChange() if 
    /// a change occurs.
    virtual void Update();

protected:
    /// Tells all attached listeners that a new image is available.
    void NotifyImageChange();

private:
    Core::String name;

    typedef std::list< ImageSourceListener* > ListenerList;
    ListenerList listeners;
};

}
}

#endif // ROCKETCOREIMAGESOURCE_H
