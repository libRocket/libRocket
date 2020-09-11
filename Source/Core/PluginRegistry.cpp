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
#include "PluginRegistry.h"
#include "../../Include/Rocket/Core/Plugin.h"

namespace Rocket {
namespace Core {

PluginList *PluginRegistry::basic_plugins = NULL;
PluginList *PluginRegistry::document_plugins = NULL;
PluginList *PluginRegistry::element_plugins = NULL;


PluginRegistry::PluginRegistry()
{
}

bool PluginRegistry::Initialise()
{
    basic_plugins = new PluginList();
    document_plugins = new PluginList();
    element_plugins = new PluginList();
    return true;
}

    void PluginRegistry::Shutdown()
    {
        delete basic_plugins;
        delete document_plugins;
        delete element_plugins;
    }

    void PluginRegistry::RegisterPlugin(Plugin* plugin)
{
	int event_classes = plugin->GetEventClasses();
	
	if (event_classes & Plugin::EVT_BASIC)
		basic_plugins->push_back(plugin);
	if (event_classes & Plugin::EVT_DOCUMENT)
        document_plugins->push_back(plugin);
	if (event_classes & Plugin::EVT_ELEMENT)
        element_plugins->push_back(plugin);
}

// Calls OnInitialise() on all plugins.
void PluginRegistry::NotifyInitialise()
{
    for (size_t i = 0; i < basic_plugins->size(); ++i)
		(*basic_plugins)[i]->OnInitialise();
}

// Calls OnShutdown() on all plugins.
void PluginRegistry::NotifyShutdown()
{
    while (!basic_plugins->empty())
	{
        basic_plugins->back()->OnShutdown();
        basic_plugins->pop_back();
	}
    document_plugins->clear();
    element_plugins->clear();
}

// Calls OnContextCreate() on all plugins.
void PluginRegistry::NotifyContextCreate(Context* context)
{
    for (size_t i = 0; i < basic_plugins->size(); ++i)
		(*basic_plugins)[i]->OnContextCreate(context);
}

// Calls OnContextDestroy() on all plugins.
void PluginRegistry::NotifyContextDestroy(Context* context)
{
    for (size_t i = 0; i < basic_plugins->size(); ++i)
		(*basic_plugins)[i]->OnContextDestroy(context);
}

// Calls OnDocumentOpen() on all plugins.
void PluginRegistry::NotifyDocumentOpen(Context* context, const String& document_path)
{
    for (size_t i = 0; i < document_plugins->size(); ++i)
		(*document_plugins)[i]->OnDocumentOpen(context, document_path);
}

// Calls OnDocumentLoad() on all plugins.
void PluginRegistry::NotifyDocumentLoad(ElementDocument* document)
{
    for (size_t i = 0; i < document_plugins->size(); ++i)
		(*document_plugins)[i]->OnDocumentLoad(document);
}

// Calls OnDocumentUnload() on all plugins.
void PluginRegistry::NotifyDocumentUnload(ElementDocument* document)
{
    for (size_t i = 0; i < document_plugins->size(); ++i)
		(*document_plugins)[i]->OnDocumentUnload(document);
}

// Calls OnElementCreate() on all plugins.
void PluginRegistry::NotifyElementCreate(Element* element)
{
    for (size_t i = 0; i < element_plugins->size(); ++i)
		(*element_plugins)[i]->OnElementCreate(element);
}

// Calls OnElementDestroy() on all plugins.
void PluginRegistry::NotifyElementDestroy(Element* element)
{
    for (size_t i = 0; i < element_plugins->size(); ++i)
		(*element_plugins)[i]->OnElementDestroy(element);
}

}
}
