scale = 1
pixel_density = 40
texture_size = 136
root = {
    name = "body",
    shape = "cube.mesh",
    origin = {0, 0, 0},
    position = {0, 0, 0},
    rotation = {0, 0, 0},
    size = {0.65, 0.825, 0.40},
    uv = {0, 60},
    nodes = {
        {
            name = "neck",
            shape = "cube.mesh",
            origin = {0, 0, 0},
            position = {0, 0.45, 0},
            rotation = {0, 0, 0},
            size = {0.30, 0.10, 0.30},
            uv = {0, 44},
            nodes = {
                {
                    name = "head",
                    shape = "cube.mesh",
                    origin = {0, -0.5, 0},
                    position = {0, 0.31, 0},
                    rotation = {0, 0, 0},
                    size = {0.55, 0.55, 0.55},
                    uv = {0, 0},
                    cameras = {
                        {
                            name = "3rd_person",
                            origin = {0, -1.2, 8},
                            position = {0, 12, -8},
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
            position = {-0.485, 0.3875, 0},
            rotation = {0, 0, 0},
            size = {0.325, 0.20, 0.20},
            uv = {88, 85},
            nodes = {
                {
                    name = "forearm_r",
                    shape = "cube.mesh",
                    origin = {0.5, 0.25, 0},
                    position = {-0.275, 0, 0},
                    rotation = {0, 0, 0},
                    size = {0.325, 0.20, 0.20},
                    uv = {88, 101},
                    nodes = {
                        {
                            name = "hand_r",
                            shape = "cube.mesh",
                            origin = {0, 0, 0},
                            position = {-0.2725, -0.013, 0},
                            rotation = {0, 0, 0},
                            size = {0.225, 0.225, 0.25},
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
            position = {0.485, 0.3875, 0},
            rotation = {0, 0, 0},
            size = {0.325, 0.20, 0.20},
            uv = {88, 85},
            nodes = {
                {
                    name = "forearm_l",
                    shape = "cube_mirror.mesh",
                    origin = {-0.5, 0.25, 0},
                    position = {0.275, 0, 0},
                    rotation = {0, 0, 0},
                    size = {0.325, 0.20, 0.20},
                    uv = {88, 101},
                    nodes = {
                        {
                            name = "hand_l",
                            shape = "cube_mirror.mesh",
                            origin = {0, 0, 0},
                            position = {0.2725, -0.013, 0},
                            rotation = {0, 0, 0},
                            size = {0.225, 0.225, 0.25},
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
            position = {-0.175, -0.60, 0},
            rotation = {0, 0, 0},
            size = {0.25, 0.375, 0.30},
            uv = {0, 109},
            nodes = {
                {
                    name = "leg_r",
                    shape = "cube.mesh",
                    origin = {0, 0, 0},
                    position = {0, -0.375, 0},
                    rotation = {0, 0, 0},
                    size = {0.25, 0.375, 0.25},
                    uv = {44, 109},
                    nodes = {
                        {
                            name = "foot_r",
                            shape = "cube.mesh",
                            origin = {0, 0, 0},
                            position = {-0.0125, -0.2375, 0.043},
                            rotation = {0, 0, 0},
                            size = {0.30, 0.10, 0.40},
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
            position = {0.175, -0.60, 0},
            rotation = {0, 0, 0},
            size = {0.25, 0.375, 0.30},
            uv = {0, 109},
            nodes = {
                {
                    name = "leg_l",
                    shape = "cube_mirror.mesh",
                    origin = {0, 0, 0},
                    position = {0, -0.375, 0},
                    rotation = {0, 0, 0},
                    size = {0.25, 0.375, 0.25},
                    uv = {44, 109},
                    nodes = {
                        {
                            name = "foot_l",
                            shape = "cube_mirror.mesh",
                            origin = {0, 0, 0},
                            position = {0.0125, -0.2375, 0.043},
                            rotation = {0, 0, 0},
                            size = {0.30, 0.10, 0.40},
                            uv = {68, 44}
                        }
                    }
                }
            }
        }
    }
}
