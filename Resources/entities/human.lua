components = {
    type = {"human"},
    model = {
        render_pass = 1,
        model = "human.lua",
        material = "human.mat.lua"
    },
    animation = {
        file = "entities/human.lua"
    },
    rigidbody = {
        shape = "capsule",
        height = 1.4,
        radius = 0.5,
        density = 2
    },
    controller = {
        speed = 10.0,
        run_speed = 9.0,
    },
    sight = {
        eye_position = {0.0, 0.5, 0.0}
    },
    grab = {},
    inventory = {
      { name = "INVENTORY", slots = 10 },
      {
        name = "RIGHT HAND", node = "hand_r",
        offset = {{-0.0, 0.0, 0.0}},
        orientation = {{90.0, 0.0, 180.0}}
      }
    }
}

animation_controller = {
  start = { play = {{ name = "idle" }} },
  idle = {
    file = "human2.lua", loop = true, base = true,
    events = {
      walk = { stop = {}, play = {{ name = "walk" }} },
      cast = { play = {{ name = "cast" }} }
    }
  },
  walk = {
    file = "human2.lua", loop = true, speed = 4, base = true,
    events = {
      stop = { stop = {stop_mode = 2, fade_out=0.5}, play = {{ name = "idle" }} },
      cast = { play = {{ name = "cast" }}  }
    }
  },
  cast = {
    file = "human2.lua", loop = false, speed = 1, base = false
  }
}
