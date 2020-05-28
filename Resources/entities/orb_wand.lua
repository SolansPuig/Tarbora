components = {
  type = {"weapon", "arcane", "magic"},
  model = {
    render_pass = 1,
    model = "mage_wand3.lua",
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
    name = "Orb Wand",
    valid_groups = {"RIGHT HAND", "INVENTORY"},
    max_quantity = 1,
    offset = {0.0, -0.5, 0.0}
  },
  throw = {
    projectile = "orb.lua",
    strength = 200.0,
    max_projectiles = 3,
    origin = {-0.3, 0.5, 0.6}
  },
  animation = {
    file = "entities/gravity_wand.lua"
  }
}
