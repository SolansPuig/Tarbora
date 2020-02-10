components = {
    type = {"tree"},
    model = {
        render_pass = 1,
        model = "birch.lua",
        material = "birch.mat.lua"
    },
    physics = {
        shape = "box",
        size = {0.75, 2.25, 0.75},
        density = 0,
        friction = 1,
        restitution = 0.3
    }
}
