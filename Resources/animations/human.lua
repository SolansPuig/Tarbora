query = {
    currentX = 0,
    currentY = 0,
    currentZ = 0,
    time = 0
}

walk = {
    blend_time = 0,
    duration = 8,
    loop = true,
    nodes = {
        body = {
            position = {0, function() return math.cos(20*query.time) end, 0}
        },
        arm_r = {
            rotation = {0, function() return -20*math.cos(10*query.time) end, 80}
        },
        arm_l = {
            rotation = {0, function() return -20*math.cos(10*query.time) end, -80}
        },
        forearm_r = {
            rotation = {0, function() return -10*math.cos(10*query.time) + 10 end, 5}
        },
        forearm_l = {
            rotation = {0, function() return -10*math.cos(10*query.time) - 10 end, -5}
        },
        thig_r = {
            rotation = {function() return -30*math.cos(10*query.time) end, 0, 0}
        },
        thig_l = {
            rotation = {function() return 30*math.cos(10*query.time) end, 0, 0}
        },
        leg_r = {
            rotation = {function() return -10*math.cos(10*query.time) + 10 end, 0, 0}
        },
        leg_l = {
            rotation = {function() return 10*math.cos(10*query.time) + 10 end, 0, 0}
        },
    }
}

idle = {
    blend_time = 0.1,
    duration = 12,
    loop = true,
    nodes = {
        body = {
            position = {0, 0, 0}
        },
        arm_r = {
            rotation = {10, 0, function() return 80 + 5*math.cos(query.time) end}
        },
        forearm_r = {
            rotation = {0, 20, 5}
        },
        arm_l = {
            rotation = {10, 0, function() return -80 - 5*math.cos(query.time) end}
        },
        forearm_l = {
            rotation = {0, -20, -5}
        },
        thig_r = {
            rotation = {0, 0, 0}
        },
        thig_l = {
            rotation = {0, 0, 0}
        },
        leg_r = {
            rotation = {0, 0, 0}
        },
        leg_l = {
            rotation = {0, 0, 0}
        },
    }
}
