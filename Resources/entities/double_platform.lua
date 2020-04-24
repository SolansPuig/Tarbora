components = {
    type = {"ground"},
    model = {
        render_pass = 0,
        model = "double_platform.lua",
        material = "terrain2.mat.lua"
    },
    rigidbody = {
        shape = "box",
        size = {2, 4, 4},
        density = 0,
        friction = 1,
        restitution = 0.3
    }
}
