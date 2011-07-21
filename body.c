//Copyright (c) 2011 Oscar (.teri) Triano <teritriano@gmail.com>

//Permission is hereby granted, free of charge, to any person obtaining a 
//copy of this software and associated documentation files (the "Software"), 
//to deal in the Software without restriction, including without limitation the rights to use, 
//copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
//and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included 
//in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
//OTHER DEALINGS IN THE SOFTWARE.

#include <lunatic_chipmunk.h>

static int chipmunk_NewBody(lua_State *vm){
    //mass, moi
    cpFloat m = INFINITY, moi = INFINITY;
    if (lua_isnumber(vm, 1)){
        m = lua_tonumber(vm, 1);
    }
    if (lua_isnumber(vm, 2)){
        moi = lua_tonumber(vm, 2);
    }
    cpBody *body = lua_newuserdata(vm, sizeof(cpBody));
    cpBodyInit(body, m, moi);
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.bodymeta");
    lua_setmetatable(vm, -2);
    return 1;
}

static int chipmunk_body_newindex(lua_State *vm){
    const char *key = lua_tostring(vm, 2);
    cpBody *body = lua_touserdata(vm, 1);
    if (strcmp("pos", key) == 0 && lua_istable(vm, 3)){
        cpBodySetPos(body, chipmunk_TableTocpVect(3, vm));
    }
    else if (strcmp("vel", key) == 0 && lua_istable(vm, 3)){
        cpBodySetVel(body, chipmunk_TableTocpVect(3, vm));
    }
    return 0;
}

static int chipmunk_body_index(lua_State *vm){
    const char *key = lua_tostring(vm, 2);
    cpBody *body = lua_touserdata(vm, 1);
    if (strcmp("pos", key) == 0){
        chipmunk_cpVectToTable(cpBodyGetPos(body), vm);
        return 1;
    }
    else if (strcmp("vel", key) == 0){
        chipmunk_cpVectToTable(cpBodyGetVel(body), vm);
        return 1;
    }
    lua_pushnil(vm);
    return 1;
}

static int chipmunk_body_gc(lua_State *vm){
    cpBody *body = lua_touserdata(vm, 1);
    cpBodyDestroy(body);
    printf("Delete body: %p\n", body);
}
