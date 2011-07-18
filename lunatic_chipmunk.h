//Copyright (c) 2011 Oscar (.teri) Triano <teritriano@gmail.com>

//This software is provided 'as-is', without any express or implied
//warranty. In no event will the authors be held liable for any damages
//arising from the use of this software.

//Permission is granted to anyone to use this software for any purpose,
//including commercial applications, and to alter it and redistribute it
//freely, subject to the following restrictions:

   //1. The origin of this software must not be misrepresented; you must not
   //claim that you wrote the original software. If you use this software
   //in a product, an acknowledgment in the product documentation would be
   //appreciated but is not required.

   //2. Altered source versions must be plainly marked as such, and must not be
   //misrepresented as being the original software.

   //3. This notice may not be removed or altered from any source
   //distribution.

#ifndef _lunatic_chipmunk_
#define _lunatic_chipmunk_

#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <chipmunk/chipmunk.h>

inline void chipmunk_cpVectToTable(cpVect vector, lua_State *vm){
    lua_createtable(vm, 0, 2);
    lua_pushnumber(vm, vector.x);
    lua_setfield(vm, -2, "x");
    lua_pushnumber(vm, vector.y);
    lua_setfield(vm, -2, "y");
    //+1
}

inline cpVect chipmunk_TableTocpVect(int indextable, lua_State *vm){
    cpVect vector = cpvzero;
    lua_getfield(vm, indextable, "x");
    if (lua_isnil(vm, -1)){
        lua_pop(vm, 1);
        lua_rawgeti(vm, indextable, 1);
    }
    vector.x = lua_tonumber(vm, -1);
    lua_pop(vm, 1);
    lua_getfield(vm, indextable, "y");
    if (lua_isnil(vm, -1)){
        lua_pop(vm, 1);
        lua_rawgeti(vm, indextable, 2);
    }
    vector.y = lua_tonumber(vm, -1);
    lua_pop(vm, 1);
    return vector;
}

int luaopen_chipmunk(lua_State *);

//space
static int chipmunk_NewSpace(lua_State *);
static int chipmunk_space_newindex(lua_State *);
static int chipmunk_space_index(lua_State *);
static int chipmunk_space_gc(lua_State *);
static int chipmunk_space_Step(lua_State *);
static int chipmunk_space_AddBody(lua_State *);

//body
static int chipmunk_NewBody(lua_State *);
static int chipmunk_body_newindex(lua_State *);
static int chipmunk_body_index(lua_State *);
static int chipmunk_body_gc(lua_State *);

#endif
