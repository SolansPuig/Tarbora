components = {
    type = {"human"},
    model = {
        render_pass = 1,
        model = "human4.lua",
        material = "human.mat.lua"
    },
    animation = {
        file = "human.lua",
    },
    physics = {
        shape = "capsule",
        height = 1.4,
        radius = 0.5,
        density = 2
    },
    controller = {
        speed = 6.0,
        run_speed = 9.0,
    }
}
