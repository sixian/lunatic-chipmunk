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
        cpSpaceSetGravity(space, chipmunk_TableTocpVect(3, vm));
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
        chipmunk_cpVectToTable(cpSpaceGetGravity(space), vm);
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
    cpSpace *space = (lc_GetSpace(1, vm))->space;
    cpSpaceFree(space);
    luaL_unref(vm, LUA_REGISTRYINDEX, space->bodies);
    luaL_unref(vm, LUA_REGISTRYINDEX, space->shapes);
    printf("Delete space: %p\n", object_space);
}

int lc_space_Step(lua_State *vm){
    //space, number
    cpSpace *space = (lc_GetSpace(1, vm))->space;
    if (object_space == NULL || object_space->type != Space){
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
    
    //Check if the body alreaddy has been added to a space
    if (body->space != LUA_REFNIL){
        printf("space:AddBody(): Body already added to a space. Remove the from it first.\n");
        return 0;
    }
    //---
    
    char pbody[17] = {0};
    snprintf(pbody, 17, "%p", body);
    
    //Check if the body exists in bodies table. If not exists then add to the table
    lua_rawgeti(vm, LUA_REGISTRYINDEX, bodies);
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
    //Set a scene reference to the body
    lua_pushvalue(vm, 1);
    body->space = luaL_ref(vm, LUA_REGISTRYINDEX);
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
    
    char pbody[17] = {0};
    snprintf(pbody, 17, "%p", body);
    
    //Check if the body has been added to the space for be removed.
    lua_rawgeti(vm, LUA_REGISTRYINDEX, bodies);
    lua_getfield(vm, -1, pbody);
    if (lua_isnil(vm, -1)){
        //The body doesn't exists.
        return 0;
    }
    lua_pop(vm, 1);
    lua_pushnil(vm);
    lua_setfield(vm, -2, pbody);
    //---
    //Unref body space
    luaL_unref(vm, LUA_REGISTRYINDEX, body->space);
    body->space = LUA_REFNIL;
    //
    
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
    lc_shape *body = lc_GetBody(2, vm);
    if (object_shape == NULL || object_shape->type == Body || object_shape->type == Space){
        printf("space:AddShape(): Can't add a shape\n");
        return 0;
    }
    cpSpaceAddShape((cpSpace *)object_space->object, (cpShape *)object_shape->object);
    return 0;
}

int lc_space_RemoveShape(lua_State *vm){
    //space, shape
    chipmunk_object *object_space = (chipmunk_object *)lua_touserdata(vm, 1);
    if (object_space == NULL || object_space->type != Space){
        printf("chipmunk: Object can't call :RemoveShape\n");
        return 0;
    }
    chipmunk_object *object_shape = (chipmunk_object *)lua_touserdata(vm, 2);
    if (object_shape == NULL || object_shape->type == Body || object_shape->type == Space){
        printf("space:AddShape(): Can't remove a shape\n");
        return 0;
    }
    cpSpaceRemoveShape((cpSpace *)object_space->object, (cpShape *)object_shape->object);
    return 0;
}

int lc_space_NewSegmentShape(lua_State *vm){
    //space, cpVect1 {x, y}, cpVect2 {x, y}, cpFloat -> shape
    chipmunk_object *object_space = (chipmunk_object *)lua_touserdata(vm, 1);
    if (object_space == NULL || object_space->type != Space){
        printf("chipmunk: Object can't call :NewSegmentShape\n");
        return 0;
    }
    cpVect a = cpvzero, b = cpvzero;
    cpFloat thickness = 0;
    if (!lua_istable(vm, 2)){
        printf("space:NewSegmentShape() -> 1st parameter must be a table.");
        RETURN_NIL;
    }
    a = chipmunk_TableTocpVect(2, vm);
    if (!lua_istable(vm, 3)){
        printf("space:NewSegmentShape() -> 2nd parameter must be a table.");
        RETURN_NIL;
    }
    b = chipmunk_TableTocpVect(3, vm);
    if (!lua_isnumber(vm, 4)){
        printf("space:NewSegmentShape() -> 2hd parameter must be a valid number.");
        RETURN_NIL;
    }
    thickness = lua_tonumber(vm, 4);
    chipmunk_NewSegmentShape(cpSpaceGetStaticBody((cpSpace *)object_space->object), a, b, thickness, vm);
    return 1;
}
