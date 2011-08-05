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

#include <stdio.h>
#include <string.h>
#include <lunatic_chipmunk.h>

inline lc_space *lc_GetSpace(int indexspace, lua_State *vm){
    lc_object *object = (lc_object *)lua_touserdata(vm, indexspace);
    if (object == NULL || object->type != Space){
        return NULL;}
    return (lc_space *)object->object;
}

int lc_NewSpace(lua_State *vm){
    //-> space
    lc_object *object_space = lua_newuserdata(vm, sizeof(lc_object));
    lc_space *space = malloc(sizeof(lc_space));
    space->space = cpSpaceNew();
    lua_newtable(vm);
    space->bodies = luaL_ref(vm, LUA_REGISTRYINDEX);
    lua_newtable(vm);
    space->shapes = luaL_ref(vm, LUA_REGISTRYINDEX);
    object_space->object = space;
    object_space->type = Space;
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.spacemeta");
    lua_setmetatable(vm, -2);
    return 1;
}

int lc_space_newindex(lua_State *vm){
    cpSpace *space = (lc_GetSpace(1, vm))->space;
    const char *key = lua_tostring(vm, 2);
    if (strcmp("gravity", key) == 0 && lua_istable(vm, 3)){
        cpSpaceSetGravity(space, lc_TableTocpVect(3, vm));
    }
    else if (strcmp("iterations", key) == 0){
        cpSpaceSetIterations(space, (cpFloat)lua_tonumber(vm, 3));
    }
    return 0;
}

int lc_space_index(lua_State *vm){
    cpSpace *space = (lc_GetSpace(1, vm))->space;
    const char *key = lua_tostring(vm, 2);
    if (strcmp("gravity", key) == 0){
        lc_cpVectToTable(cpSpaceGetGravity(space), vm);
        return 1;
    }
    else if (strcmp("iterations", key) == 0){
        lua_pushnumber(vm, (lua_Number)cpSpaceGetIterations(space));
        return 1;
    }
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.space:");
    lua_getfield(vm, -1, key);
    return 1;
}

int lc_space_gc(lua_State *vm){
    lc_space *space = lc_GetSpace(1, vm);
    cpSpaceFree(space->space);
    luaL_unref(vm, LUA_REGISTRYINDEX, space->bodies);
    luaL_unref(vm, LUA_REGISTRYINDEX, space->shapes);
    free(space);
    printf("Delete space: %p\n", lua_touserdata(vm, 1));
}

int lc_space_Step(lua_State *vm){
    //space, number
    cpSpace *space = (lc_GetSpace(1, vm))->space;
    if (space == NULL){
        printf("chipmunk: Object can't call :Step\n");
        return 0;
    }
    cpSpaceStep(space, lua_tonumber(vm, 2));
    return 0;
}

int lc_space_AddBody(lua_State *vm){
    //space, body
    lc_space *space = lc_GetSpace(1, vm);
    if (space == NULL){
        printf("chipmunk: Object can't call :AddBody\n");
        return 0;
    }
    lc_body *body = lc_GetBody(2, vm);
    if (body == NULL){
        printf("space:AddBody(): Can't add a object that isn't a body.\n");
        return 0;
    }
        
    char pbody[32] = {0};
    snprintf(pbody, 32, "%p", body);
    
    //Check if the body exists in bodies table. If not exists then add to the table
    lua_rawgeti(vm, LUA_REGISTRYINDEX, space->bodies);
    lua_getfield(vm, -1, pbody);
    if (!lua_isnil(vm, -1)){
        //The body already exists. Then return.
        return 0;
    }
    lua_pop(vm, 1);
    lua_pushvalue(vm, 2);
    lua_setfield(vm, -2, pbody);
    lua_pop(vm, 1);//bodies table
    //---
    
    cpSpaceAddBody(space->space, body->body);
    return 0;
}

int lc_space_RemoveBody(lua_State *vm){
    //space, body
    lc_space *space = lc_GetSpace(1, vm);
    if (space == NULL){
        printf("chipmunk: Object can't call :RemoveBody\n");
        return 0;
    }
    lc_body *body = lc_GetBody(2, vm);
    if (body == NULL){
        printf("space:RemoveBody(): Can't remove a object that isn't a body.\n");
        return 0;
    }
    
    char pbody[32] = {0};
    snprintf(pbody, 32, "%p", body);
    
    //Check if the body has been added to the space for be removed.
    lua_rawgeti(vm, LUA_REGISTRYINDEX, space->bodies);
    lua_getfield(vm, -1, pbody);
    if (lua_isnil(vm, -1)){
        //The body doesn't exists.
        return 0;
    }
    lua_pop(vm, 1);
    lua_pushnil(vm);
    lua_setfield(vm, -2, pbody);
    //---
    
    cpSpaceRemoveBody(space->space, body->body);
    return 0;
}

int lc_space_AddShape(lua_State *vm){
    //space, shape
    lc_space *space = lc_GetSpace(1, vm);
    if (space == NULL){
        printf("chipmunk: Object can't call :AddShape\n");
        return 0;
    }
    lc_shape *shape = lc_GetShape(2, vm);
    if (shape == NULL){
        printf("space:AddShape(): Can't add a shape\n");
        return 0;
    }
    
    char pshape[32] = {0};
    snprintf(pshape, 32, "%p", shape);
    
    //If the shape doesn't exists in shapes table then add it
    lua_rawgeti(vm, LUA_REGISTRYINDEX, space->shapes);
    lua_getfield(vm, -1, pshape);
    if (!lua_isnil(vm, -1)){
        //The shape alreadye exists
        return 0;
    }
    lua_pop(vm, 1);
    lua_pushvalue(vm, 2);
    lua_setfield(vm, -2, pshape);
    lua_pop(vm, 1);//shapes table
    //---
    
    cpSpaceAddShape(space->space, shape->shape);
    return 0;
}

int lc_space_RemoveShape(lua_State *vm){
    //space, shape
    lc_space *space = lc_GetSpace(1, vm);
    if (space == NULL){
        printf("chipmunk: Object can't call :RemoveShape\n");
        return 0;
    }
    lc_shape *shape = lc_GetShape(2, vm);
    if (shape == NULL){
        printf("space:AddShape(): Can't remove a shape\n");
        return 0;
    }
    
    char pshape[32] = {0};
    snprintf(pshape, 32, "%p", shape);
    
    //Check if the body has been added to the space for be removed.
    lua_rawgeti(vm, LUA_REGISTRYINDEX, space->shapes);
    lua_getfield(vm, -1, pshape);
    if (lua_isnil(vm, -1)){
        //The body doesn't exists.
        return 0;
    }
    lua_pop(vm, 1);
    lua_pushnil(vm);
    lua_setfield(vm, -2, pshape);
    //---
    
    cpSpaceRemoveShape(space->space, shape->shape);
    return 0;
}

int lc_space_NewSegmentShape(lua_State *vm){
    //space, cpVect1 {x, y}, cpVect2 {x, y}, cpFloat -> shape
    lc_space *space = lc_GetSpace(1, vm);
    if (space == NULL){
        printf("chipmunk: Object can't call :NewSegmentShape\n");
        return 0;
    }
    cpVect a = cpvzero, b = cpvzero;
    cpFloat thickness = 0;
    if (!lua_istable(vm, 2)){
        printf("space:NewSegmentShape() -> 1st parameter must be a table.");
        RETURN_NIL;
    }
    a = lc_TableTocpVect(2, vm);
    if (!lua_istable(vm, 3)){
        printf("space:NewSegmentShape() -> 2nd parameter must be a table.");
        RETURN_NIL;
    }
    b = lc_TableTocpVect(3, vm);
    if (!lua_isnumber(vm, 4)){
        printf("space:NewSegmentShape() -> 2hd parameter must be a valid number.");
        RETURN_NIL;
    }
    thickness = lua_tonumber(vm, 4);
    lc_shape *shape = lc_NewSegmentShape(cpSpaceGetStaticBody(space->space), a, b, thickness, vm);
    lua_pushvalue(vm, 1);
    shape->bodyorspace = luaL_ref(vm, LUA_REGISTRYINDEX);
    return 1;
}
