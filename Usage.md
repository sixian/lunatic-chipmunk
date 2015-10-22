# Introduction #

Using lunatic-chipmunk is easy. All functions are stored in a table named "chipmunk" once loaded the module. All the objects are garbage collected.

# Usage #

_moi = chipmunk.MomentForBox( **mass**, **width**, **height**)_: Moment of inertia for a box

_moi = chipmunk.MomentForSegment( **mass**, **a**, **b**)_: **a** and **b** are tables {x, y} that represents a start points and a end point.

## Space ##

_space = chipmunk.NewSpace()_

_space.gravity = {x, y}_

_space.iterations_

_space:AddBody( **body** )_

_space:RemoveBody( **body** )_

_space:AddShape( **shape** )_

_space:RemoveShape( **shape**)_

_space:Step( **step** )_: **step** must be in seconds. Call :Step() in your program loop for the simulation.

_shape = space:NewSegmentShape( **a**, **b**, **radius** )_: **a** and **b** are tables {x, y} that represents a start point and a end point. **radius** is the thickness of the segment.

## Body ##

_body = chipmunk.NewBody( (**mass**), (**moi**) )_: **mass** and **moi** are optional parameters. Default values are INFINITY.

_body = chipmunk.NewStaticBody()_

_body.pos = {x, y}_

_body.vel = {x, y}_

_body.angle_: Angle in randias.

_body.radangle_: Angle in radians.

_body.degangle_: Angle in degrees.

_shape = body:NewBoxShape( **width**, **height** )_

_shape = body:NewCircleShape( **radius**, **offset** )_: **offset** is a table with the {x, y}

## Shape ##