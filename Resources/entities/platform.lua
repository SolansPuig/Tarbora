components = {
    type = {"ground"},
    model = {
        render_pass = 0,
        model = "platform.lua",
        material = "terrain2.mat.lua"
    },
    rigidbody = {
        shape = "box",
        size = {2, 2, 2},
        density = 0,
        friction = 1,
        restitution = 0.3
    }
}
