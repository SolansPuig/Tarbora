walk = {
    duration = 8,
    step = 1,
    loop = true,

    nodes = {
        body = {
            position = {
                [0] = {0, -100, 0},
                [4] = {0, 100, 0},
            }
        }
    }
}

idle = {
    duration = 4,
    step = 2,
    loop = true,

    nodes = {
        body = {
            scale = {
                [0] = {0, 0, -1},
                [2] = {0, 0, 1},
            }
        }
    }
}
