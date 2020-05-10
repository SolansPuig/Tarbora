components = {
    type = {"ground"},
    model = {
        render_pass = 1,
        model = "cube.lua",
        material = "container.mat.lua"
    },
    rigidbody= {
        shape = "box",
        size = {1, 1, 1},
        density = 90,
        friction = 0.3,
        restitution = 0.9
    },
    item = {
      name = "cube",
      valid_groups = {"RIGHT HAND", "INVENTORY"},
      max_quantity = 2
    }
}
