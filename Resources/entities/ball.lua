components = {
    type = {"ground"},
    model = {
        render_pass = 1,
        model = "cube_compound.lua",
        material = "container.mat.lua"
    },
    rigidbody = {
        shape = "sphere",
        radius = 0.5
        density = 100,
        friction = 2,
        restitution = 0.9
    }
}
