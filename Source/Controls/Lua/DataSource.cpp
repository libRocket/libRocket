#include "precompiled.h"
#include "DataSource.h"
#include <Rocket/Core/Log.h>

namespace Rocket {
namespace Core {
namespace Lua {
typedef LuaDataSource DataSource;

int DataSourceNotifyRowAdd(lua_State* L, DataSource* obj)
{
    LUACHECKOBJ(obj);
    const char* table_name = luaL_checkstring(L,1);
    int first_row_added = luaL_checkint(L,2);
    int num_rows_added = luaL_checkint(L,3);
    obj->NotifyRowAdd(table_name,first_row_added,num_rows_added);
    return 0;
}

int DataSourceNotifyRowRemove(lua_State* L, DataSource* obj)
{
    LUACHECKOBJ(obj);
    const char* table_name = luaL_checkstring(L,1);
    int first_row_removed = luaL_checkint(L,2);
    int num_rows_removed = luaL_checkint(L,3);
    obj->NotifyRowRemove(table_name,first_row_removed,num_rows_removed);
    return 0;
}

int DataSourceNotifyRowChange(lua_State* L, DataSource* obj)
{
    LUACHECKOBJ(obj);
    const char* table_name = luaL_checkstring(L,1);
    if(lua_gettop(L) < 2)
    {
        obj->NotifyRowChange(table_name);
    }
    else
    {
        int first_row_changed = luaL_checkint(L,2);
        int num_rows_changed = luaL_checkint(L,3);
        obj->NotifyRowChange(table_name,first_row_changed,num_rows_changed);
    }
    return 0;
}

int DataSourceSetAttrGetNumRows(lua_State* L)
{
    DataSource* obj = LuaType<DataSource>::check(L,1);
    LUACHECKOBJ(obj);
    if(lua_type(L,2) == LUA_TFUNCTION)
    {
        lua_pushvalue(L,2); //copy of the function, so it is for sure at the top of the stack
        obj->getNumRowsRef = lua_ref(L,true);
    }
    else
        Log::Message(Log::LT_WARNING, "Lua: Must assign DataSource.GetNumRows as a function, value received was of %s type", lua_typename(L,2));
    return 0;
}

int DataSourceSetAttrGetRow(lua_State* L)
{
    DataSource* obj = LuaType<DataSource>::check(L,1);
    LUACHECKOBJ(obj);
    if(lua_type(L,2) == LUA_TFUNCTION)
    {
        lua_pushvalue(L,2); //copy of the functions, so it is for sure at the top of the stack
        obj->getRowRef = lua_ref(L,true);
    }
    else
        Log::Message(Log::LT_WARNING, "Lua: Must assign DataSource.GetRow as a function, value received was of %s type", lua_typename(L,2));
    return 0;
}


RegType<DataSource> DataSourceMethods[] =
{
    LUAMETHOD(DataSource,NotifyRowAdd)
    LUAMETHOD(DataSource,NotifyRowRemove)
    LUAMETHOD(DataSource,NotifyRowChange)
    { NULL, NULL },
};

luaL_reg DataSourceGetters[] =
{
    { NULL, NULL },
};

luaL_reg DataSourceSetters[] =
{
    LUASETTER(DataSource,GetNumRows)
    LUASETTER(DataSource,GetRow)
    { NULL, NULL },
};

}
}
}