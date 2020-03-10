query = {
    time = 0,
    speed = 4,
}

walk = {
    duration = function () return (4)/(query.speed) end,
    step = 1,
    loop = true,

    nodes = {
        body = {
            position = function () return {0, 1 * math.cos(math.pi * query.speed * query.time), 0} end,
        },
        thig_l = {
            rotation = function () return {20 * math.sin(math.pi/2 * query.speed * query.time), 0, 2} end,
        },
        thig_r = {
            rotation = function () return {-20 * math.sin(math.pi/2 * query.speed * query.time), 0, -2} end,
        },
        leg_l = {
            rotation = function () return {10 * math.sin(math.pi/2 * query.speed * query.time) + 10, 0, 0} end,
        },
        leg_r = {
            rotation = function () return {-10 * math.sin(math.pi/2 * query.speed * query.time) + 10, 0, 0} end,
        },
        foot_l = {
            rotation = function () return {10 * math.sin(math.pi/2 * query.speed * query.time), 0, -2} end,
        },
        foot_r = {
            rotation = function () return {-10 * math.sin(math.pi/2 * query.speed * query.time), 0, 2} end,
        },
        arm_r = {
            rotation = function() return {0, -10 * math.sin(math.pi/2 * query.speed * query.time), 0} end,
        },
        arm_l = {
            rotation = function() return {0, -10 * math.sin(math.pi/2 * query.speed * query.time), 0} end,
        },
        forearm_r = {
            rotation = function() return {0, -5 * math.sin(math.pi/2 * query.speed * query.time) + 5, 0} end,
        },
        forearm_l = {
            rotation = function() return {0, -5 * math.sin(math.pi/2 * query.speed * query.time) - 5, 0} end,
        },
    }
}

idle = {
    duration = 2,
    step = 2,
    loop = true,

    nodes = {
        arm_l = {
            rotation = {
                [0] = {1, 1, -1},
                [1] = {-1, -1, 1},
            }
        },
        arm_r = {
            rotation = {
                [0] = {1, -1, 1},
                [1] = {-1, 1, -1},
            }
        },
        belly = {
            position = {
                [0] = {0, 0, -0.25},
                [1] = {0, 0, 0.25},
            },
            scale = {
                [0] = {0, 0, -0.5},
                [1] = {0, 0, 0.5},
            }
        },
        hip = {
            position = {
                [0] = {0, 0, 0.25},
                [1] = {0, 0, -0.25}
            }
        },
        thig_l = { rotation = {[0] = {-5, 0, 2}} },
        thig_r = { rotation = {[0] = {-5, 0, -2}} },
        leg_l = { rotation = {[0] = {10, 0, 0}} },
        leg_r = { rotation = {[0] = {10, 0, 0}} },
        foot_l = { rotation = {[0] = {-5, 0, -2}} },
        foot_r = { rotation = {[0] = {-5, 0, 2}} }
    }
}
