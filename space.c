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

int chipmunk_NewSpace(lua_State *vm){
    //-> space
    chipmunk_object *object_space = lua_newuserdata(vm, sizeof(chipmunk_object));
    object_space->object = cpSpaceNew();
    object_space->type = Space;
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.spacemeta");
    lua_setmetatable(vm, -2);
    return 1;
}

int chipmunk_space_newindex(lua_State *vm){
    chipmunk_object *object_space = lua_touserdata(vm, 1);
    cpSpace *space = object_space->object;
    const char *key = lua_tostring(vm, 2);
    if (strcmp("gravity", key) == 0 && lua_istable(vm, 3)){
        cpSpaceSetGravity(space, chipmunk_TableTocpVect(3, vm));
    }
    return 0;
}

int chipmunk_space_index(lua_State *vm){
    chipmunk_object *object_space = lua_touserdata(vm, 1);
    cpSpace *space = object_space->object;
    const char *key = lua_tostring(vm, 2);
    if (strcmp("gravity", key) == 0){
        chipmunk_cpVectToTable(cpSpaceGetGravity(space), vm);
        return 1;
    }
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.space:");
    lua_getfield(vm, -1, key);
    return 1;
}

int chipmunk_space_gc(lua_State *vm){
    chipmunk_object *object_space = (chipmunk_object *)lua_touserdata(vm, 1);
    cpSpaceFree((cpSpace *)object_space->object);
    printf("Delete space: %p\n", object_space);
}

int chipmunk_space_Step(lua_State *vm){
    //space, number
    chipmunk_object *object_space = lua_touserdata(vm, 1);
    if (object_space == NULL || object_space->type != Space){
        printf("chipmunk: Object can't call :Step\n");
        return 0;
    }
    cpSpaceStep((cpSpace *)object_space->object, lua_tonumber(vm, 2));
    return 0;
}

int chipmunk_space_AddBody(lua_State *vm){
    //space, body
    chipmunk_object *object_space = (chipmunk_object *)lua_touserdata(vm, 1);
    if (object_space == NULL || object_space->type != Space){
        printf("chipmunk: Object can't call :AddBody\n");
        return 0;
    }
    chipmunk_object *object_body = (chipmunk_object *)lua_touserdata(vm, 2);
    if (object_body == NULL || object_body->type != Body){
        printf("space:AddBody(): Can't add a body\n");
        return 0;
    }
    cpSpaceAddBody((cpSpace *)object_space->object, (cpBody *)object_body->object);
    return 0;
}

int chipmunk_space_RemoveBody(lua_State *vm){
    //space, body
    chipmunk_object *object_space = (chipmunk_object *)lua_touserdata(vm, 1);
    if (object_space == NULL || object_space->type != Space){
        printf("chipmunk: Object can't call :RemoveBody\n");
        return 0;
    }
    chipmunk_object *object_body = (chipmunk_object *)lua_touserdata(vm, 2);
    if (object_body == NULL || object_body->type != Body){
        printf("space:RemoveBody(): Can't remove a body\n");
        return 0;
    }
    cpSpaceRemoveBody((cpSpace *)object_space->object, (cpBody *)object_body->object);
    return 0;
}

int chipmunk_space_AddShape(lua_State *vm){
    //space, shape
    chipmunk_object *object_space = (chipmunk_object *)lua_touserdata(vm, 1);
    if (object_space == NULL || object_space->type != Space){
        printf("chipmunk: Object can't call :AddShape\n");
        return 0;
    }
    chipmunk_object *object_shape = (chipmunk_object *)lua_touserdata(vm, 2);
    if (object_shape == NULL || object_shape->type == Body || object_shape->type == Space){
        printf("space:AddShape(): Can't add a shape\n");
        return 0;
    }
    cpSpaceAddShape((cpSpace *)object_space->object, (cpShape *)object_shape->object);
    return 0;
}

int chipmunk_space_RemoveShape(lua_State *vm){
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

int chipmunk_space_NewSegmentShape(lua_State *vm){
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
