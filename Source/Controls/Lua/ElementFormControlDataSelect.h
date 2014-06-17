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
 
#ifndef ROCKETCONTROLSLUAELEMENTFORMCONTROLDATASELECT_H
#define ROCKETCONTROLSLUAELEMENTFORMCONTROLDATASELECT_H

#include <Rocket/Core/Lua/lua.hpp>
#include <Rocket/Core/Lua/LuaType.h>
#include <Rocket/Controls/ElementFormControlDataSelect.h>

using Rocket::Core::Lua::LuaType;
namespace Rocket {
namespace Controls {
namespace Lua {

//method
int ElementFormControlDataSelectSetDataSource(lua_State* L, ElementFormControlDataSelect* obj);

extern Rocket::Core::Lua::RegType<ElementFormControlDataSelect> ElementFormControlDataSelectMethods[];
extern luaL_Reg ElementFormControlDataSelectGetters[];
extern luaL_Reg ElementFormControlDataSelectSetters[];

}
}
}
namespace Rocket { namespace Core { namespace Lua {
//inherits from ElementFormControl which inherits from Element
template<> void ExtraInit<Rocket::Controls::ElementFormControlDataSelect>(lua_State* L, int metatable_index);
LUACONTROLSTYPEDECLARE(Rocket::Controls::ElementFormControlDataSelect)
}}}
#endif
