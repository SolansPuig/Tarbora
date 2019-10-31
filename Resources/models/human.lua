scale = 1
pixel_density = 40
texture_size = 136
root = {
    name = "body",
    shape = "cube.mesh",
    origin = {0, 0, 0},
    position = {0, 0, 0},
    rotation = {0, 0, 0},
    size = {26, 33, 16},
    uv = {0, 60},
    nodes = {
        {
            name = "neck",
            shape = "cube.mesh",
            origin = {0, 0, 0},
            position = {0, 18, 0},
            rotation = {0, 0, 0},
            size = {12, 4, 12},
            uv = {0, 44},
            nodes = {
                {
                    name = "head",
                    shape = "cube.mesh",
                    origin = {0, -0.5, 0},
                    position = {0, 12.5, 0},
                    rotation = {0, 0, 0},
                    size = {22, 22, 22},
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
            position = {-19.4, 15.5, 0},
            rotation = {0, 0, 0},
            size = {13, 8, 8},
            uv = {88, 85},
            nodes = {
                {
                    name = "forearm_r",
                    shape = "cube.mesh",
                    origin = {0.5, 0.25, 0},
                    position = {-11.9, 0, 0},
                    rotation = {0, 0, 0},
                    size = {13, 8, 8},
                    uv = {88, 101},
                    nodes = {
                        {
                            name = "hand_r",
                            shape = "cube.mesh",
                            origin = {0, 0, 0},
                            position = {-10.9, -0.5, 0},
                            rotation = {0, 0, 0},
                            size = {9, 9, 10},
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
            position = {19.4, 15.5, 0},
            rotation = {0, 0, 0},
            size = {13, 8, 8},
            uv = {88, 85},
            nodes = {
                {
                    name = "forearm_l",
                    shape = "cube_mirror.mesh",
                    origin = {-0.5, 0.25, 0},
                    position = {11.9, 0, 0},
                    rotation = {0, 0, 0},
                    size = {13, 8, 8},
                    uv = {88, 101},
                    nodes = {
                        {
                            name = "hand_l",
                            shape = "cube_mirror.mesh",
                            origin = {0, 0, 0},
                            position = {10.9, -0.5, 0},
                            rotation = {0, 0, 0},
                            size = {9, 9, 10},
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
            position = {-7, -24, 0},
            rotation = {0, 0, 0},
            size = {10, 15, 12},
            uv = {0, 109},
            nodes = {
                {
                    name = "leg_r",
                    shape = "cube.mesh",
                    origin = {0, 0, 0},
                    position = {0, -15, 0},
                    rotation = {0, 0, 0},
                    size = {10, 15, 10},
                    uv = {44, 109},
                    nodes = {
                        {
                            name = "foot_r",
                            shape = "cube.mesh",
                            origin = {0, 0, 0},
                            position = {-0.5, -9.5, 1.75},
                            rotation = {0, 0, 0},
                            size = {12, 4, 16},
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
            position = {7, -24, 0},
            rotation = {0, 0, 0},
            size = {10, 15, 12},
            uv = {0, 109},
            nodes = {
                {
                    name = "leg_l",
                    shape = "cube_mirror.mesh",
                    origin = {0, 0, 0},
                    position = {0, -15, 0},
                    rotation = {0, 0, 0},
                    size = {10, 15, 10},
                    uv = {44, 109},
                    nodes = {
                        {
                            name = "foot_l",
                            shape = "cube_mirror.mesh",
                            origin = {0, 0, 0},
                            position = {0.5, -9.5, 1.75},
                            rotation = {0, 0, 0},
                            size = {12, 4, 16},
                            uv = {68, 44}
                        }
                    }
                }
            }
        }
    }
}
