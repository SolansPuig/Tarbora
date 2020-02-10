components = {
    type = {"ground"},
    model = {
        render_pass = 1,
        model = "cube.lua",
        material = "container.mat.lua"
    },
    physics = {
        shape = "box",
        size = {1, 1, 1},
        density = 90,
        friction = 0.3,
        restitution = 0.2
    }
}
