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

#ifndef ROCKETCOREIMAGESOURCELISTENER_H
#define ROCKETCOREIMAGESOURCELISTENER_H

#include <Rocket/Core/Header.h>
#include <Rocket/Core/String.h>
#include <Rocket/Core/Texture.h>

namespace Rocket {
namespace Core {

class ImageSource;

/**
    Interface for objects wishing to listen to image source events. Listeners should use
    the AttachListener() on ImageSource to begin observing an image source.

    @author Matthew Alan Gray <mgray@hatboystudios.com>

 *  --== Changes ==--
 *  20 Feb 2012     Initial Creation                                        Matthew Alan Gray <mgray@hatboystudios.com>
 *   2 Mar 2012     Removed byte and source_dimensions parameters since     Matthew Alan Gray <mgray@hatboystudios.com>
 *                  this can be obtained via ImageSource::GetImage()
 */

class ROCKETCORE_API ImageSourceListener
{
public:
    ImageSourceListener();
    virtual ~ImageSourceListener();

    /// Notification of the destruction of an observed image source.
    /// @param[in] image_source Image source being destroyed.
    virtual void OnImageSourceDestroy(ImageSource* image_source);
    /// Notification of a new image from an observed data source.
    /// @param[in] image_source Image source being changed.
    virtual void OnImageChange(ImageSource* image_source);
};

}
}

#endif // ROCKETCOREIMAGESOURCELISTENER_H