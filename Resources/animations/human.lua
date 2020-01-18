g = {
    currentX = 0,
    currentY = 0,
    currentZ = 0,
    time = 0
}

run = {
    blendTime = 0,
    duration = 8,
    loop = true,
    nodes = {
        body = {
            position = {0, function() return math.cos(20*g.time) end, 0}
        },
        arm_r = {
            rotation = {0, function() return -50*math.cos(10*g.time) end, 80}
        },
        arm_l = {
            rotation = {0, function() return -50*math.cos(10*g.time) end, -80}
        },
        forearm_r = {
            rotation = {0, function() return -20*math.cos(10*g.time) + 20 end, 5}
        },
        forearm_l = {
            rotation = {0, function() return -20*math.cos(10*g.time) - 20 end, -5}
        },
        thig_r = {
            rotation = {function() return -40*math.cos(10*g.time) end, 0, 0}
        },
        thig_l = {
            rotation = {function() return 40*math.cos(10*g.time) end, 0, 0}
        },
        leg_r = {
            rotation = {function() return -20*math.cos(10*g.time) + 20 end, 0, 0}
        },
        leg_l = {
            rotation = {function() return 20*math.cos(10*g.time) + 20 end, 0, 0}
        },
    }
}

idle = {
    blendTime = 0,
    duration = 12,
    loop = true,
    nodes = {
        arm_r = {
            rotation = {10, 0, function() return 80 + 5*math.cos(g.time) end}
        },
        forearm_r = {
            rotation = {0, 20, 5}
        },
        arm_l = {
            rotation = {10, 0, function() return -80 - 5*math.cos(g.time) end}
        },
        forearm_l = {
            rotation = {0, -20, -5}
        }
    }
}
