scale = 1
nodes = {
    {
        name = "body",
        scale = 1.00,
        size = {65.00, 82.50, 40.00},
        shape = "cube.mesh",
        uv_map = {0, 60},
        nodes = {
            {
                name = "arm_l",
                scale = 1.00,
                origin = {-0.50, -0.25, 0.00},
                position = {46.50, 43.75, 0.00},
                size = {32.50, 20.00, 20.00},
                shape = "cube_mirror.mesh",
                uv_map = {88, 85},
                nodes = {
                    {
                        name = "forearm_l",
                        scale = 1.00,
                        origin = {-0.50, 0.25, 0.00},
                        position = {27.50, 0.00, 0.00},
                        size = {32.50, 20.00, 20.00},
                        shape = "cube_mirror.mesh",
                        uv_map = {88, 101},
                        nodes = {
                            {
                                name = "hand_l",
                                scale = 1.00,
                                position = {27.25, -1.30, 0.00},
                                size = {22.50, 22.50, 25.00},
                                shape = "cube_mirror.mesh",
                                uv_map = {88, 117},
                            },
                        }
                    },
                }
            },
            {
                name = "arm_r",
                scale = 1.00,
                origin = {0.50, -0.25, 0.00},
                position = {-46.50, 46.00, 0.00},
                rotation = {0.00, -0.00, 43.00},
                size = {32.50, 20.00, 20.00},
                shape = "cube.mesh",
                uv_map = {88, 85},
                nodes = {
                    {
                        name = "forearm_r",
                        scale = 1.00,
                        origin = {0.50, 0.25, 0.00},
                        position = {-27.50, 0.00, 0.00},
                        size = {32.50, 20.00, 20.00},
                        shape = "cube.mesh",
                        uv_map = {88, 101},
                        nodes = {
                            {
                                name = "hand_r",
                                scale = 1.00,
                                position = {-27.25, -1.30, 0.00},
                                size = {22.50, 22.50, 25.00},
                                shape = "cube.mesh",
                                uv_map = {88, 117},
                            },
                        }
                    },
                }
            },
            {
                name = "neck",
                scale = 1.00,
                position = {0.00, 45.00, 0.00},
                size = {30.00, 10.00, 30.00},
                shape = "cube.mesh",
                uv_map = {0, 44},
                nodes = {
                    {
                        name = "head",
                        scale = 1.00,
                        origin = {0.00, -0.50, 0.00},
                        position = {0.00, 31.00, 0.00},
                        size = {55.00, 55.00, 55.00},
                        shape = "cube.mesh",
                        uv_map = {0, 0},
                        nodes = {
                            {
                                name = "1st_person",
                                scale = 1.00,
                                type = "camera"
                            },
                            {
                                name = "3rd_person",
                                scale = 1.00,
                                origin = {0.00, -120.00, 800.00},
                                position = {0.00, 120.00, -800.00},
                                type = "camera"
                            },
                        }
                    },
                }
            },
            {
                name = "thig_l",
                scale = 1.00,
                origin = {0.00, 0.50, 0.00},
                position = {17.50, -58.00, 0.00},
                size = {25.00, 37.50, 30.00},
                shape = "cube_mirror.mesh",
                uv_map = {0, 109},
                nodes = {
                    {
                        name = "leg_l",
                        scale = 1.00,
                        origin = {0.00, 0.50, 0.00},
                        position = {0.00, -35.50, 0.00},
                        size = {25.00, 37.50, 25.00},
                        shape = "cube_mirror.mesh",
                        uv_map = {44, 109},
                        nodes = {
                            {
                                name = "foot_l",
                                scale = 1.00,
                                position = {1.25, -23.75, 4.30},
                                size = {30.00, 10.00, 40.00},
                                shape = "cube_mirror.mesh",
                                uv_map = {68, 44},
                            },
                        }
                    },
                }
            },
            {
                name = "thig_r",
                scale = 1.00,
                origin = {0.00, 0.50, 0.00},
                position = {-17.50, -58.00, 0.00},
                size = {25.00, 37.50, 30.00},
                shape = "cube.mesh",
                uv_map = {0, 109},
                nodes = {
                    {
                        name = "leg_r",
                        scale = 1.00,
                        origin = {0.00, 0.50, 0.00},
                        position = {0.00, -35.50, 0.00},
                        size = {25.00, 37.50, 25.00},
                        shape = "cube.mesh",
                        uv_map = {44, 109},
                        nodes = {
                            {
                                name = "foot_r",
                                scale = 1.00,
                                position = {-1.25, -23.75, 4.30},
                                size = {30.00, 10.00, 40.00},
                                shape = "cube.mesh",
                                uv_map = {68, 44},
                            },
                        }
                    },
                }
            },
        }
    },
}
