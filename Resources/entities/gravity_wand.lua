components = {
  type = {"weapon", "gravity", "magic"},
  model = {
    render_pass = 1,
    model = "mage_wand.lua",
    material = "mage_wand.mat.lua"
  },
  rigidbody = {
    shape = "box",
    size = {0.05, 1.8, 0.05},
    density = 20000,
    friction = 0.9,
    restitution = 0.1
  },
  item = {
    name = "Gravity Wand",
    valid_groups = {"RIGHT HAND", "INVENTORY"},
    max_quantity = 1,
    offset = {0.0, -0.5, 0.0}
  },
  gravity_randomizer = {
    range = 50
  },
  animation = {
    file = "entities/gravity_wand.lua"
  }
}

animation_controller = {
  start = { play = {{ name = "idle" }} },
  idle = {
    file = "wand.lua", loop = true, base = true,
  }
}
