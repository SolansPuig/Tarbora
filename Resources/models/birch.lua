function gen_leaf(nname, pos, size, rot)
  local leaf = {
    name = nname,
    shape = "cube.mesh",
    position = {pos.x, pos.y, pos.z},
    rotation = {rot.x, rot.y, rot.z},
    size = {size.x, size.y, size.x},
    texture_size = {100, 100, 100},
    color_primary= {157, 214, 85},
    mesh_size = {size.x/2, size.y/2, size.x/2},
    nodes = {
      {
        name = nname .. "1",
        shape = "plane.mesh",
        position = {0, 0, 0},
        rotation = {0, 45, 0},
        size = {size.x, size.y, size.x},
        texture_size = {200, 200, 200},
        mesh_size = {200, 200, 200},
        color_primary= {157, 214, 85},
        uv_map = {0, 200}
      },
      {
        name = nname .. "2",
        shape = "plane.mesh",
        position = {0, 0, 0},
        rotation = {0, -45, 0},
        size = {size.x, size.y, size.x},
        texture_size = {200, 200, 200},
        mesh_size = {200, 200, 200},
        color_primary= {157, 214, 85},
        uv_map = {0, 200}
      }
    }
  }
  return leaf;
end

function gen_branch(nname, n, pos, size, rot)
  local branch = {
    name = nname,
    shape = "cube.mesh",
    position = {pos.x, pos.y, pos.z},
    rotation = {rot.x, rot.y, rot.z},
    origin = {0, -0.5, 0},
    size = {size.x, size.y, size.x},
    texture_size = {43, 200, 43},
    mesh_size = {43, size.y, 43},
    uv_map = {0, 200},
    nodes = {}
  }

  -- Generate branches
  if (n < 1) then
    -- Always a child branch
    nsize = math.random(700, 900)
    nrot = {
      x = math.random(math.min(-rot.x, -2 * rot.x), math.max(-rot.x, -2 * rot.x)),
      y = math.random(math.min(-rot.y, -2 * rot.y), math.max(-rot.y, -2 * rot.y)),
      z = math.random(math.min(-rot.z, -2 * rot.z), math.max(-rot.z, -2 * rot.z))
    }
    table.insert(
      branch.nodes,
      gen_branch(
        "body" .. n,
        n + 1,
        {x = 0, y = (size.y + nsize)/2, z = 0},
        {x = 43, y = nsize},
        nrot
      )
    )

    -- Almost always a second child branch
    if (math.random() > 0.1) then
      nsize = math.random(700, 900)
      table.insert(
        branch.nodes,
        gen_branch(
          "body2" .. n,
          n + 1,
          {x = 0, y = (size.y + nsize)/2, z = 0},
          {x = 33, y = nsize},
          {
            x = math.random(-3, 3)^3,
            y = math.random(-3, 3)^3,
            z = math.random(-3, 3)^3
          }
        )
      )
    end

    -- Sometimes a third child branch
    if (math.random() > 0.7) then
      nsize = math.random(700, 900)
      table.insert(
        branch.nodes,
        gen_branch(
          "body3" .. n,
          n + 1,
          {x = 0, y = (size.y + nsize)/2 + math.random(50, 100), z = 0},
          {x = 29, y = nsize},
          {
            x = math.random(-4, 4)^3,
            y = math.random(-4, 4)^3,
            z = math.random(-4, 4)^3
          }
        )
      )
    end
  else -- Generate leaves
    table.insert(
      branch.nodes,
      gen_leaf(
        "leaf1",
        {x = 0, y = math.random(400, 500), z = 0},
        {x = math.random(200, 300), y = math.random(220, 300)},
        {
          x = math.random(-25, 25),
          y = math.random(-25, 25),
          z = math.random(-25, 25)
        }
      )
    )
    table.insert(
      branch.nodes,
      gen_leaf(
        "leaf2",
        {x = 0, y = math.random(100, 200), z = 0},
        {x = math.random(200, 300), y = math.random(200, 300)},
        {
          x = math.random(-25, 25),
          y = math.random(-25, 25),
          z = math.random(-25, 25)
        }
      )
    )
    table.insert(
      branch.nodes,
      gen_leaf(
        "leaf3",
        {x = 0, y = math.random(50, 200), z = 0},
        {x = math.random(200, 300), y = math.random(200, 300)},
        {
          x = math.random(-25, 25),
          y = math.random(-25, 25),
          z = math.random(-25, 25)
        }
      )
    )
    table.insert(
      branch.nodes,
      gen_leaf(
        "leaf4",
        {
          x = math.random(-200, 200),
          y = math.random(-200, 200),
          z = math.random(-200, 200)
        },
        {x = math.random(100, 200), y = math.random(100, 200)},
        {
          x = math.random(-25, 25),
          y = math.random(-25, 25),
          z = math.random(-25, 25)
        }
      )
    )
    table.insert(
      branch.nodes,
      gen_leaf(
        "leaf5",
        {
          x = math.random(-200, 200),
          y = math.random(-200, 200),
          z = math.random(-200, 200)
        },
        {x = math.random(100, 200), y = math.random(100, 200)},
        {
          x = math.random(-25, 25),
          y = math.random(-25, 25),
          z = math.random(-25, 25)
        }
      )
    )
  end

  return branch
end

scale = 1
nodes = function()
  local nsize = math.random(200, 1000)
  return {
    gen_branch(
      "body",
      0,
      {x = 0, y = 0, z = 0},
      {x = 43, y = nsize},
      {
        x = math.random(-3, 3),
        y = math.random(-3, 3),
        z = math.random(-3, 3)
      }
    )}
end
