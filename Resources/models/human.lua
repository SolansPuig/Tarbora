scale = 1
pixel_density = 40
texture_size = 136
root = {
    name = "body",
    shape = "cube.mesh",
    origin = {0, 0, 0},
    position = {0, 0, 0},
    rotation = {0, 0, 0},
    size = {65, 82.5, 40},
    uv = {0, 60},
    nodes = {
        {
            name = "neck",
            shape = "cube.mesh",
            origin = {0, 0, 0},
            position = {0, 45, 0},
            rotation = {0, 0, 0},
            size = {30, 10, 30},
            uv = {0, 44},
            nodes = {
                {
                    name = "head",
                    shape = "cube.mesh",
                    origin = {0, -0.5, 0},
                    position = {0, 31, 0},
                    rotation = {0, 0, 0},
                    size = {55, 55, 55},
                    uv = {0, 0},
                    cameras = {
                        {
                            name = "3rd_person",
                            origin = {0, -1.2, 8},
                            position = {0, 1.2, -8},
                            rotation = {10, 0, 0}
                        },
                        {
                            name = "1st_person",
                            origin = {0, 0, 0},
                            position = {0, 0, 0},
                            rotation = {0, 0, 0}
                        }
                    }
                }
            }
        },
        {
            name = "arm_r",
            shape = "cube.mesh",
            origin = {0.5, -0.25, 0},
            position = {-48.5, 38.75, 0},
            rotation = {0, 0, 0},
            size = {32.5, 20, 20},
            uv = {88, 85},
            nodes = {
                {
                    name = "forearm_r",
                    shape = "cube.mesh",
                    origin = {0.5, 0.25, 0},
                    position = {-27.5, 0, 0},
                    rotation = {0, 0, 0},
                    size = {32.5, 20, 20},
                    uv = {88, 101},
                    nodes = {
                        {
                            name = "hand_r",
                            shape = "cube.mesh",
                            origin = {0, 0, 0},
                            position = {-27.25, -1.3, 0},
                            rotation = {0, 0, 0},
                            size = {22.5, 22.5, 25},
                            uv = {88, 117}
                        }
                    }
                }
            }
        },
        {
            name = "arm_l",
            shape = "cube_mirror.mesh",
            origin = {-0.5, -0.25, 0},
            position = {48.5, 38.75, 0},
            rotation = {0, 0, 0},
            size = {32.5, 20, 20},
            uv = {88, 85},
            nodes = {
                {
                    name = "forearm_l",
                    shape = "cube_mirror.mesh",
                    origin = {-0.5, 0.25, 0},
                    position = {27.5, 0, 0},
                    rotation = {0, 0, 0},
                    size = {32.5, 20, 20},
                    uv = {88, 101},
                    nodes = {
                        {
                            name = "hand_l",
                            shape = "cube_mirror.mesh",
                            origin = {0, 0, 0},
                            position = {27.25, -1.3, 0},
                            rotation = {0, 0, 0},
                            size = {22.5, 22.5, 25},
                            uv = {88, 117}
                        }
                    }
                }
            }
        },
        {
            name = "thig_r",
            shape = "cube.mesh",
            origin = {0, 0, 0},
            position = {-17.5, -60, 0},
            rotation = {0, 0, 0},
            size = {25, 37.5, 30},
            uv = {0, 109},
            nodes = {
                {
                    name = "leg_r",
                    shape = "cube.mesh",
                    origin = {0, 0, 0},
                    position = {0, -37.5, 0},
                    rotation = {0, 0, 0},
                    size = {25, 37.5, 25},
                    uv = {44, 109},
                    nodes = {
                        {
                            name = "foot_r",
                            shape = "cube.mesh",
                            origin = {0, 0, 0},
                            position = {-1.25, -23.75, 4.3},
                            rotation = {0, 0, 0},
                            size = {30, 10, 40},
                            uv = {68, 44}
                        }
                    }
                }
            }
        },
        {
            name = "thig_l",
            shape = "cube_mirror.mesh",
            origin = {0, 0, 0},
            position = {17.5, -60, 0},
            rotation = {0, 0, 0},
            size = {25, 37.5, 30},
            uv = {0, 109},
            nodes = {
                {
                    name = "leg_l",
                    shape = "cube_mirror.mesh",
                    origin = {0, 0, 0},
                    position = {0, -37.5, 0},
                    rotation = {0, 0, 0},
                    size = {25, 37.5, 25},
                    uv = {44, 109},
                    nodes = {
                        {
                            name = "foot_l",
                            shape = "cube_mirror.mesh",
                            origin = {0, 0, 0},
                            position = {1.25, -23.75, 4.3},
                            rotation = {0, 0, 0},
                            size = {30, 10, 40},
                            uv = {68, 44}
                        }
                    }
                }
            }
        }
    }
}
