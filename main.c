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

void lc_cpVectToTable(cpVect vector, lua_State *vm){
    lua_createtable(vm, 0, 2);
    lua_pushnumber(vm, vector.x);
    lua_setfield(vm, -2, "x");
    lua_pushnumber(vm, vector.y);
    lua_setfield(vm, -2, "y");
    //+1
}

cpVect lc_TableTocpVect(int indextable, lua_State *vm){
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

int luaopen_chipmunk(lua_State *vm){
    printf("\nInit chipmunk\n");
    luaL_Reg functions[] = {
    {"NewSpace", lc_NewSpace},
    {"NewBody", lc_NewBody},
    {"NewStaticBody", lc_NewStaticBody},
    {"MomentForBox", lc_MomentForBox},
    {"MomentForCircle", lc_MomentForCircle},
    {NULL, NULL}};
    
    luaL_register(vm, "chipmunk", functions);
    lua_pop(vm, 1);
    
    luaL_Reg spacemeta[] = {
    {"__newindex", lc_space_newindex},
    {"__index", lc_space_index},
    {"__gc", lc_space_gc},
    {NULL, NULL}};
    lua_createtable(vm, 0, 3);
    luaL_register(vm, NULL, spacemeta);
    lua_setfield(vm, LUA_REGISTRYINDEX, "chipmunk.spacemeta");
    
    luaL_Reg spacemethods[] = {
    {"Step", lc_space_Step},
    {"AddBody", lc_space_AddBody},
    {"RemoveBody", lc_space_RemoveBody},
    {"AddShape", lc_space_AddShape},
    {"RemoveShape", lc_space_RemoveShape},
    {"NewSegmentShape", lc_space_NewSegmentShape},
    {NULL, NULL}};
    lua_createtable(vm, 0, 2);
    luaL_register(vm, NULL, spacemethods);
    lua_setfield(vm, LUA_REGISTRYINDEX, "chipmunk.space:");
    
    luaL_Reg bodymeta[] = {
    {"__newindex", lc_body_newindex},
    {"__index", lc_body_index},
    {"__gc", lc_body_gc},
    {NULL, NULL}};
    lua_createtable(vm, 0, 3);
    luaL_register(vm, NULL, bodymeta);
    lua_setfield(vm, LUA_REGISTRYINDEX, "chipmunk.bodymeta");
    
    luaL_Reg bodymethods[] = {
    {"NewBoxShape", lc_body_NewBoxShape},
    {"NewCircleShape", lc_body_NewCircleShape},
    {NULL, NULL}};
    lua_createtable(vm, 0, 1);
    luaL_register(vm, NULL, bodymethods);
    lua_setfield(vm, LUA_REGISTRYINDEX, "chipmunk.body:");
    
    luaL_Reg shapemeta[] = {
    {"__newindex", lc_shape_newindex},
    {"__index", lc_shape_index},
    {"__gc", lc_shape_gc},
    {NULL, NULL}};
    lua_createtable(vm, 0, 3);
    luaL_register(vm, NULL, shapemeta);
    lua_setfield(vm, LUA_REGISTRYINDEX, "chipmunk.shapemeta");
    
}
