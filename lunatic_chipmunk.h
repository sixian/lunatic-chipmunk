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

#ifndef _lunatic_chipmunk_
#define _lunatic_chipmunk_

#include <stdlib.h>
#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <chipmunk/chipmunk.h>

#define RETURN_NIL lua_pushnil(vm);return 1;

void lc_cpVectToTable(cpVect vector, lua_State *vm);//-0,+1
cpVect lc_TableTocpVect(int indextable, lua_State *vm);//-0,+0

int luaopen_chipmunk(lua_State *);

typedef enum __lc_type{
    Space,
    Body,
    StaticBody,
    PolyShape,
    CircleShape,
    SegmentShape
}lc_type;

typedef struct __lc_object{
    lc_type type;
    void *object;
}lc_object;

//space

typedef struct __lc_space{
    cpSpace *space;
    int bodies;//table with bodies
    int shapes;//table with shapes
}lc_space;

int lc_NewSpace(lua_State *);
int lc_space_newindex(lua_State *);
int lc_space_index(lua_State *);
int lc_space_gc(lua_State *);
int lc_space_Step(lua_State *);
int lc_space_AddBody(lua_State *);
int lc_space_RemoveBody(lua_State *);
int lc_space_AddShape(lua_State *);
int lc_space_RemoveShape(lua_State *);
int lc_space_NewSegmentShape(lua_State *);

inline lc_space *lc_GetSpace(int indexspace, lua_State *vm){
    lc_object *object = (lc_object *)lua_touserdata(vm, indexspace);
    if (object == NULL || object->type != Space){
        return NULL;}
    return (lc_space *)object->object;
}

//body

typedef struct __lc_body{
    cpBody *body;
}lc_body;

inline lc_body *lc_GetBody(int indexbody, lua_State *vm){
    lc_object *object = (lc_object *)lua_touserdata(vm, indexbody);
    if (object == NULL || object->type != Body){
        return NULL;}
    return (lc_body *)object->object;
}

int lc_NewBody(lua_State *);
int lc_NewStaticBody(lua_State *);
int lc_body_newindex(lua_State *);
int lc_body_index(lua_State *);
int lc_body_gc(lua_State *);
int lc_body_NewBoxShape(lua_State *);
int lc_body_NewCircleShape(lua_State *);

int lc_MomentForBox(lua_State *);
int lc_MomentForCircle(lua_State *);

//Shape

typedef struct __lc_shape{
    cpShape *shape;
    int bodyorspace;//ref to body or space (space body)
}lc_shape;

//Shapes are created with the a space's body or with a body.
lc_shape *lc_NewBoxShape(cpBody *, cpFloat, cpFloat, lua_State *);//-0, +1
lc_shape *lc_NewCircleShape(cpBody *, cpFloat, cpVect, lua_State *);//-0, +1
lc_shape *lc_NewSegmentShape(cpBody *, cpVect, cpVect, cpFloat, lua_State *);//-0, +1

inline lc_shape *lc_GetShape(int indexshape, lua_State *vm){
    lc_object *object = (lc_object *)lua_touserdata(vm, indexshape);
    if (object == NULL || object->type == Body || object->type == Space){
        return NULL;}
    return (lc_shape *)object->object;
}

int lc_shape_newindex(lua_State *);
int lc_shape_index(lua_State *);
int lc_shape_gc(lua_State *);

#endif
