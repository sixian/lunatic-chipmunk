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

//-0, +1
lc_shape *lc_NewBoxShape(cpBody *body, cpFloat width, cpFloat height, lua_State *vm){
    lc_object *object_shape = lua_newuserdata(vm, sizeof(lc_object));
    lc_shape *shape = malloc(sizeof(lc_shape));
    object_shape->type = PolyShape;
    shape->shape = cpBoxShapeNew(body, width, height);
    shape->bodyorspace = LUA_REFNIL;
    object_shape->object = shape;
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.shapemeta");
    lua_setmetatable(vm, -2);
    return object_shape;
}

//-0, +1
lc_shape *lc_NewCircleShape(cpBody *body, cpFloat radius, cpVect offset, lua_State *vm){
    lc_object *object_shape = lua_newuserdata(vm, sizeof(lc_object));
    lc_shape *shape = malloc(sizeof(lc_shape));
    object_shape->type = PolyShape;
    shape->shape = cpCircleShapeNew(body, radius, offset);
    shape->bodyorspace = LUA_REFNIL;
    object_shape->object = shape;
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.shapemeta");
    lua_setmetatable(vm, -2);
    return object_shape;
}

//-0, +1
lc_shape *lc_NewSegmentShape(cpBody *body, cpVect a, cpVect b, cpFloat radius, lua_State *vm){
    lc_object *object_shape = lua_newuserdata(vm, sizeof(lc_object));
    lc_shape *shape = malloc(sizeof(lc_shape));
    object_shape->type = PolyShape;
    shape->shape = cpSegmentShapeNew(body, a, b, radius);
    shape->bodyorspace = LUA_REFNIL;
    object_shape->object = shape;
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.shapemeta");
    lua_setmetatable(vm, -2);
    return object_shape;
}

int lc_shape_newindex(lua_State *vm){
    return 0;
}

int lc_shape_index(lua_State *vm){
    lua_pushnil(vm);
    return 1;
}

int lc_shape_gc(lua_State *vm){
    lc_shape *shape = lc_GetShape(1, vm);
    luaL_unref(vm, LUA_REGISTRYINDEX, shape->bodyorspace);
    cpShapeFree(shape->shape);
    free(shape);
    printf("Delete shape: %p\n", lua_touserdata(vm, 1));
    return 0;
}
