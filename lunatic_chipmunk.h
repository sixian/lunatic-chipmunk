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

#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <chipmunk/chipmunk.h>

#define RETURN_NIL lua_pushnil(vm);return 1;

void chipmunk_cpVectToTable(cpVect vector, lua_State *vm);//-0,+1
cpVect chipmunk_TableTocpVect(int indextable, lua_State *vm);//-0,+0

int luaopen_chipmunk(lua_State *);

typedef enum __chipmunk_type{
    Space,
    Body,
    StaticBody,
    PolyShape,
    CircleShape,
    SegmentShape
}chipmunk_type;

typedef struct __chipmunk_object{
    chipmunk_type type;
    void *object;
}chipmunk_object;

//space
int chipmunk_NewSpace(lua_State *);
int chipmunk_space_newindex(lua_State *);
int chipmunk_space_index(lua_State *);
int chipmunk_space_gc(lua_State *);
int chipmunk_space_Step(lua_State *);
int chipmunk_space_AddBody(lua_State *);
int chipmunk_space_AddShape(lua_State *);
int chipmunk_space_NewSegmentShape(lua_State *);

//body
int chipmunk_NewBody(lua_State *);
int chipmunk_NewStaticBody(lua_State *);
int chipmunk_body_newindex(lua_State *);
int chipmunk_body_index(lua_State *);
int chipmunk_body_gc(lua_State *);
int chipmunk_body_NewBoxShape(lua_State *);
int chipmunk_body_NewCircleShape(lua_State *);

int chipmunk_MomentForBox(lua_State *);
int chipmunk_MomentForCircle(lua_State *);

//Shape
//Shapes are created with the a space's body or with a body.
cpPolyShape *chipmunk_NewBoxShape(cpBody *, cpFloat, cpFloat, lua_State *);//-0, +1
cpCircleShape *chipmunk_NewCircleShape(cpBody *, cpFloat, cpVect, lua_State *);//-0, +1
cpSegmentShape *chipmunk_NewSegmentShape(cpBody *, cpVect, cpVect, cpFloat, lua_State *);//-0, +1
int chipmunk_shape_newindex(lua_State *);
int chipmunk_shape_index(lua_State *);
int chipmunk_shape_gc(lua_State *);

#endif
