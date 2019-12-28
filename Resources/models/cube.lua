scale = 1
pixel_density = 16
texture_size = 64
root = {
    name = "body",
    shape = "cube.mesh",
    origin = {0, 0, 0},
    position = {0, 0, 0},
    rotation = {0, 0, 0},
    size = {1, 1, 1},
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
    uv = {0, 0}
}
