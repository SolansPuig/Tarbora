components = {
  type = {"weapon", "gravity"},
  model = {
    render_pass = 1,
    model = "gravity_cannon.lua",
    material = "gravity_cannon.mat.lua"
  },
  rigidbody = {
    shape = "box",
    size = {0.35, 0.2, 0.12},
    density = 20000,
    friction = 0.9,
    restitution = 0.1
  },
  item = {
    name = "Gravity Cannon",
    valid_groups = {"RIGHT HAND", "INVENTORY"},
    max_quantity = 1,
    offset = {-0.15, 0.0, 0.13}
  },
  gravity_cannon = {}
}
