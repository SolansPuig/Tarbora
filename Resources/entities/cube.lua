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
        density = 100,
        friction = 0.7,
        restitution = 0.1
    },
    item = {
      name = "cube",
      valid_groups = {"RIGHT HAND", "INVENTORY"},
      max_quantity = 2
    }
}
