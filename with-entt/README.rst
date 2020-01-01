Game of Life - With EnTT
========================

This is an implementation of Conway's Game of Life with the ECS pattern using
the EnTT library.

Architecture
------------

Entites
~~~~~~~~

Entities represent one thing in this simulation, cells.

Components
~~~~~~~~~~

There are three components in this simulation, `position`, `isAlive`, and
`isAliveNext`.

`position` is made up of two ints, `x` and `y`. It gives the cell's position.

`isAlive` and `isAliveNext` are bools but their value is insignificant, it is
their presence (or lack their of) that is usedful. They indicate whether the
cell is alive now and whether it is alive the next round, respectively. They
allow us to modify the registry (world) in-place. `isAlive` is used in the
`Lifecycle System`_ to determine whether a cell has been created this round and
that it should take the cell into account. `isAlive` is also used by the `Render
System`_ to determine whether or not the cell should be rendered. `isAliveNext`
allows us to compute the next round using the state of the current count.

Systems
~~~~~~~

Lifecycle System
^^^^^^^^^^^^^^^^

The lifecycle system is what sets the cell status', `isAlive` and `isAliveNext`.
It applies the rules of the Game of Life:

- Cell survives to the next round if it has 2 or 3 neighbors
- Cell dies if it does not have 2 or 3 neighbors
- Cell is created if a coordinate has *exactly* 3 neighbors

The presence of the `isAlive` component on the cell indicates that it is alive
this round and plays a part in the computation of the rules above. The
`isAliveNext` component is assigned in this system based on the rules above.

Render System
^^^^^^^^^^^^^

The render system renders the simulation. It iterates over the world and renders
cells with the `isAlive` component.

Cleanup System
^^^^^^^^^^^^^^

The cleanup system removes entities with `isAlive` but not `isAliveNext`.

Update System
^^^^^^^^^^^^^

The update system ensures `isAlive` is present on cells with `isAliveNext` and
removes the `isAliveNext` component.
