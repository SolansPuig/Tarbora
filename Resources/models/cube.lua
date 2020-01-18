scale = 1
root = {
    name = "body",
    shape = "cube.mesh",
    origin = {0, 0, 0},
    position = {0, 0, 0},
    rotation = {0, 0, 0},
    size = {100, 100, 100},
    color_primary = {
        function() return math.random(0, 255) end,
        function() return math.random(0, 255) end,
        function() return math.random(0, 255) end
    },
    color_secondary = {
        function() return math.random(0, 255) end,
        function() return math.random(0, 255) end,
        function() return math.random(0, 255) end
    },
    color_detail = {
        function() return math.random(0, 255) end,
        function() return math.random(0, 255) end,
        function() return math.random(0, 255) end
    },
    uv_map = {0, 0}
}
