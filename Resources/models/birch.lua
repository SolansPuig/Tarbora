function gen_branch(nname, n, pos, size, rot)
  branch = {
    name = nname,
    shape = "cube.mesh",
    position = {pos.x, pos.y, pos.z},
    rotation = {rot.x, rot.y, rot.z},
    size = {size.x, size.y, size.x}
  }
  return branch
end

scale = 1
nodes = {
  gen_branch(
    "body",
    0,
    {x = 0, y = 0, z = 0},
    {x = 33, y = function() return math.random(250, 350) end},
    { x = math.random(-10, 10), y = math.random(-10, 10), z = math.random(-10, 10) }
  )
}
