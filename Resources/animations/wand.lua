query = {
  time = 0,
  speed = 0.125,
}

idle = {
  duration = function() return 1/query.speed end,
  loop = true,

  nodes = {
    magic_ball = {
      position = function()
        return {
          0,
          2 * math.cos(2 * math.pi * 2 * query.speed * query.time + 15),
          0
        }
      end,
      rotation = function()
        return {
          0,
          180 * math.sin(2 * math.pi * query.speed * query.time),
          0
        }
      end,
    }
  }
}
