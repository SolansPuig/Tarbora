components = {
    type = {"tree"},
    model = {
        render_pass = 2,
        model = "birch.lua",
        material = "birch.mat.lua"
    },
    rigidbody = {
        shape = "box",
        size = {0.75, 2.25, 0.75},
        density = 0,
        friction = 1,
        restitution = 0.3
    }
}
