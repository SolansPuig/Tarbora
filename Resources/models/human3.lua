scale = 1
nodes = {
    {
        name = "body",
        scale = 1.00,
        size = {50.00, 30.00, 30.00},
        shape = "cube.mesh",
        uv_map = {0, 60},
        texture_size = {0.01, 0.01, 0.00},
        nodes = {
            {
                name = "arm_l",
                scale = 1.00,
                origin = {-0.50, -0.40, 0.00},
                position = {38.00, 19.00, 0.00},
                rotation = {0.00, -0.00, -77.00},
                size = {35.00, 13.00, 13.00},
                shape = "cube.mesh",
                uv_map = {0, 0},
                nodes = {
                    {
                        name = "forearm_l",
                        scale = 1.00,
                        origin = {-0.50, 0.00, 0.00},
                        position = {30.00, 0.00, 0.00},
                        size = {33.00, 13.00, 13.00},
                        shape = "cube.mesh",
                        uv_map = {0, 0},
                        texture_size = {0.33, 0.11, 0.12},
                        nodes = {
                            {
                                name = "hand_l",
                                scale = 1.00,
                                origin = {0.00, 0.00, -0.20},
                                position = {15.00, -1.00, 0.00},
                                size = {15.00, 15.00, 15.00},
                                shape = "cube.mesh",
                                uv_map = {0, 0},
                            },
                        }
                    },
                }
            },
            {
                name = "arm_r",
                scale = 1.00,
                origin = {0.50, -0.40, 0.00},
                position = {-38.00, 19.00, 0.00},
                rotation = {0.00, -0.00, 77.00},
                size = {35.00, 13.00, 13.00},
                shape = "cube.mesh",
                uv_map = {0, 0},
                nodes = {
                    {
                        name = "forearm_r",
                        scale = 1.00,
                        origin = {0.50, 0.00, 0.00},
                        position = {-30.00, 0.00, 0.00},
                        size = {33.00, 13.00, 13.00},
                        shape = "cube.mesh",
                        uv_map = {0, 0},
                        texture_size = {0.33, 0.11, 0.12},
                        nodes = {
                            {
                                name = "hand_r",
                                scale = 1.00,
                                origin = {0.00, 0.00, -0.20},
                                position = {-15.00, -1.00, 0.00},
                                size = {15.00, 15.00, 15.00},
                                shape = "cube.mesh",
                                uv_map = {0, 0},
                            },
                        }
                    },
                }
            },
            {
                name = "belly",
                scale = 1.00,
                position = {0.00, -23.00, 0.00},
                size = {46.00, 28.00, 26.00},
                shape = "cube.mesh",
                uv_map = {0, 0},
                texture_size = {0.48, 0.29, 0.28},
                nodes = {
                    {
                        name = "hip",
                        scale = 1.00,
                        position = {0.00, -14.00, 0.00},
                        size = {48.00, 14.00, 30.00},
                        shape = "cube.mesh",
                        uv_map = {0, 0},
                        texture_size = {0.50, 0.14, 0.30},
                        nodes = {
                            {
                                name = "thig_l",
                                scale = 1.00,
                                origin = {0.00, 0.40, 0.00},
                                position = {12.00, -14.00, 0.00},
                                size = {20.00, 37.00, 20.00},
                                shape = "cube.mesh",
                                uv_map = {0, 0},
                                nodes = {
                                    {
                                        name = "leg_l",
                                        scale = 1.00,
                                        origin = {0.00, 0.50, 0.50},
                                        position = {0.00, -38.00, 0.00},
                                        size = {18.00, 40.00, 18.00},
                                        shape = "cube.mesh",
                                        uv_map = {0, 0},
                                        texture_size = {0.18, 0.40, 0.18},
                                        nodes = {
                                            {
                                                name = "foot_l",
                                                scale = 1.00,
                                                origin = {0.00, 0.00, -0.30},
                                                position = {0.00, -22.00, 4.00},
                                                size = {21.00, 10.00, 31.00},
                                                shape = "cube.mesh",
                                                uv_map = {0, 0},
                                                texture_size = {0.22, 0.10, 0.31},
                                            },
                                        }
                                    },
                                }
                            },
                            {
                                name = "thig_r",
                                scale = 1.00,
                                origin = {0.00, 0.40, 0.00},
                                position = {-12.00, -14.00, 0.00},
                                size = {20.00, 37.00, 20.00},
                                shape = "cube.mesh",
                                uv_map = {0, 0},
                                nodes = {
                                    {
                                        name = "leg_r",
                                        scale = 1.00,
                                        origin = {0.00, 0.50, 0.50},
                                        position = {0.00, -38.00, 0.00},
                                        size = {18.00, 40.00, 18.00},
                                        shape = "cube.mesh",
                                        uv_map = {0, 0},
                                        texture_size = {0.18, 0.40, 0.18},
                                        nodes = {
                                            {
                                                name = "foot_r",
                                                scale = 1.00,
                                                origin = {0.00, 0.00, -0.30},
                                                position = {0.00, -22.00, 4.00},
                                                size = {21.00, 10.00, 31.00},
                                                shape = "cube.mesh",
                                                uv_map = {0, 0},
                                                texture_size = {0.22, 0.10, 0.31},
                                            },
                                        }
                                    },
                                }
                            },
                        }
                    },
                }
            },
            {
                name = "neck",
                scale = 1.00,
                position = {0.00, 16.00, 0.00},
                size = {18.00, 15.00, 18.00},
                shape = "cube.mesh",
                uv_map = {0, 0},
                nodes = {
                    {
                        name = "head",
                        scale = 1.00,
                        origin = {0.00, -0.50, 0.00},
                        position = {0.00, 20.00, 0.00},
                        size = {36.00, 38.00, 36.00},
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
        }
    },
}
