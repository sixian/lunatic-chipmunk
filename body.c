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

#include <math.h>
#include <lunatic_chipmunk.h>

inline lc_body *lc_GetBody(int indexbody, lua_State *vm){
    lc_object *object = (lc_object *)lua_touserdata(vm, indexbody);
    if (object == NULL || object->type != Body){
        return NULL;}
    return (lc_body *)object->object;
}

int lc_NewBody(lua_State *vm){
    //mass, moi -> body
    cpFloat m = INFINITY, moi = INFINITY;
    if (lua_isnumber(vm, 1)){
        m = lua_tonumber(vm, 1);
    }
    if (lua_isnumber(vm, 2)){
        moi = lua_tonumber(vm, 2);
    }
    lc_object *object_body = lua_newuserdata(vm, sizeof(lc_object));
    lc_body *body = malloc(sizeof(lc_body));
    body->body = cpBodyNew(m, moi);
    object_body->type = Body;
    object_body->object = body;
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.bodymeta");
    lua_setmetatable(vm, -2);
    return 1;
}

int lc_NewStaticBody(lua_State *vm){
    //-> body
    lc_object *object_body = lua_newuserdata(vm, sizeof(lc_object));
    object_body->type = Body;
    object_body->object = cpBodyNewStatic();
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.bodymeta");
    lua_setmetatable(vm, -2);
    return 1;
}

int lc_body_newindex(lua_State *vm){
    //userdata, key, data
    const char *key = lua_tostring(vm, 2);
    cpBody *body = (lc_GetBody(1, vm))->body;
    if (strcmp("pos", key) == 0 && lua_istable(vm, 3)){
        cpBodySetPos(body, lc_TableTocpVect(3, vm));
    }
    else if (strcmp("vel", key) == 0 && lua_istable(vm, 3)){
        cpBodySetVel(body, lc_TableTocpVect(3, vm));
    }
    else if (strcmp("angle", key) == 0 || strcmp("radangle", key) == 0){
        cpBodySetAngle(body, (cpFloat)lua_tonumber(vm, 3));
    }
    else if (strcmp("degangle", key) == 0){
        cpBodySetAngle(body, ( (cpFloat)lua_tonumber(vm, 3)*(cpFloat)M_PI/(cpFloat)180.f ) );
    }
    return 0;
}

int lc_body_index(lua_State *vm){
    //userdata, key
    const char *key = lua_tostring(vm, 2);
    cpBody *body = (lc_GetBody(1, vm))->body;
    if (strcmp("pos", key) == 0){
        lc_cpVectToTable(cpBodyGetPos(body), vm);
        return 1;
    }
    else if (strcmp("vel", key) == 0){
        lc_cpVectToTable(cpBodyGetVel(body), vm);
        return 1;
    }
    else if (strcmp("angle", key) == 0 || strcmp("radangle", key) == 0){
        lua_pushnumber(vm, (lua_Number)cpBodyGetAngle(body));
        return 1;
    }
    else if (strcmp("degangle", key) == 0){
        lua_pushnumber(vm, (lua_Number)( cpBodyGetAngle(body)*(cpFloat)180.f/(cpFloat)M_PI) );
        return 1;
    }
    lua_getfield(vm, LUA_REGISTRYINDEX, "chipmunk.body:");
    lua_getfield(vm, -1, key);
    return 1;
}

int lc_body_gc(lua_State *vm){
    lc_body *body = lc_GetBody(1, vm);
    cpBodyFree(body->body);
    free(body);
    printf("Delete body: %p\n", lua_touserdata(vm, 1));
}

int lc_body_NewBoxShape(lua_State *vm){
    //body, with, height -> shape (box)
    lc_body *body = lc_GetBody(1, vm);
    if (body == NULL){
        printf("%p hasn't :NewBoxShape()\n", body);
        RETURN_NIL;
    }
    cpFloat width = 0, height = 0;
    width = lua_tonumber(vm, 2);
    height = lua_tonumber(vm, 3);
    if (width <= 0){
        printf("body:NewBoxShape() -> width must be greater than 0.");
        RETURN_NIL;
    }
    if (height <= 0){
        printf("body:NewBoxShape() -> height must be greater than 0.");
        RETURN_NIL;
    }
    lc_shape *shape = lc_NewBoxShape(body->body, width, height, vm);
    lua_pushvalue(vm, 1);
    shape->bodyorspace = luaL_ref(vm, LUA_REGISTRYINDEX);
    return 1;
}

int lc_body_NewCircleShape(lua_State *vm){
    //body, radius, {offset}
    lc_body *body = lc_GetBody(1, vm);
    if (body == NULL){
        printf("%p hasn't :NewCircleShape()\n", body);
        lua_pushnil(vm);
        return 1;
    }
    cpFloat radius = 0.f;
    if (!lua_isnumber(vm, 2)){
        printf("body:NewCircleShape() -> 2nd argument must be a valid number");
        RETURN_NIL;
    }
    radius = lua_tonumber(vm, 2);
    if (radius <= 0){
        printf("body:NewCircleShape() -> radius must be greather than 0.");
        RETURN_NIL;
    }
    cpVect offset = cpvzero;
    if (lua_gettop(vm) > 2 && lua_istable(vm, 3)){
        offset = lc_TableTocpVect(3, vm);
    }
    lc_shape *shape = lc_NewCircleShape(body->body, radius, offset, vm);
    lua_pushvalue(vm, 1);
    shape->bodyorspace = luaL_ref(vm, LUA_REGISTRYINDEX);
    return 1;
}

int lc_MomentForBox(lua_State *vm){
    //m, width, height -> number
    cpFloat m = 0, width = 0, height = 0;
    m = lua_tonumber(vm, 1);
    width = lua_tonumber(vm, 2);
    height = lua_tonumber(vm, 3);
    lua_pushnumber(vm, cpMomentForBox(m, width, height));
    return 1;
}

int lc_MomentForCircle(lua_State *vm){
    //m, radius1, radius2, offset {x, y} -> number
    cpFloat m = 0, radius1 = 0, radius2 = 0;
    cpVect offset = cpvzero;
    m = lua_tonumber(vm, 1);
    radius1 = lua_tonumber(vm, 2);
    radius2 = lua_tonumber(vm, 3);
    if (lua_gettop(vm) > 3 && lua_istable(vm, 3)){
        offset = lc_TableTocpVect(3, vm);}
    lua_pushnumber(vm, cpMomentForCircle(m, radius1, radius2, offset));
    return 1;
}
