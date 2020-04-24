components = {
    type = {"ground"},
    model = {
        render_pass = 0,
        model = "ground.lua",
        material = "grass.mat.lua"
    },
    rigidbody = {
        shape = "box",
        size = {100, 1, 100},
        density = 0,
        friction = 1,
        restitution = 0.3
    }
}
