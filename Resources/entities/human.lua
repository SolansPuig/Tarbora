components = {
    type = {"human"},
    model = {
        render_pass = 1,
        model = "human4.lua",
        material = "human.mat.lua"
    },
    animation = {
        file = "entities/human.lua"
    },
    physics = {
        shape = "capsule",
        height = 1.4,
        radius = 0.5,
        density = 2
    },
    controller = {
        speed = 4.0,
        run_speed = 9.0,
    }
}

animation_controller = {
    start = { play = {{ name = "idle" }} },
    idle = {
        file = "human2.lua", loop = true, base = true,
        events = {
            walk = { stop = {}, play = {{ name = "walk" }} },
        }
    },
    walk = {
        file = "human2.lua", loop = true, speed = 4, base = true,
        events = {
            stop = { stop = {stop_mode = 2, fade_out=0.5}, play = {{ name = "idle" }} },
        }
    }
}
