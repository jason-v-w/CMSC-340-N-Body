# CMSC-340-N-Body A project to simulate and graphically display bodies interacting gravitationally

Below are a few design decisions that are odd enough that they deserve
explanation:

      1. Simulation collisions are not based on the actual body
      dimensions but are based on the display dimension to give
      more visual appeal

      2. The increase in the size of a body after a collision is such
      that the new 2D area is the sum of the original area and the
      area of the smaller collided body

      3. The collision checking mechanism is not perfect and makes an
      implicit assumption that three bodies will not simultaneously
      collide on the same time step

      4. When two bodies collide, the larger (or one of them if the
      two bodies are the same size) will absorb the area of the
      smaller but neither its position vector or velocity vector will
      be influenced. Its mass, however, will absorb the smaller mass

      5. The input file type is not one that is very extensible or
      even convenient to write. It was, however, simple to program and
      was used for that reason.
      