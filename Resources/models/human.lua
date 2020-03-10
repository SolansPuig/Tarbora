local query = {
    defaults = {
        skin_color = {255, 255, 255},
        shirt_color = {255, 255, 255},
        shirt_detail_color = {255, 255, 255},
        pants_color = {255, 255, 255},
        shoes_color = {255, 255, 255},
    }
}
setmetatable(query, {__index = query.defaults })

scale = 1
nodes = {
    {
        name = "body",
        scale = 1.00,
        size = {48.00, 26.00, 28.00},
        shape = "cube.mesh",
        uv_map = {0, 55},
        color_primary = query.skin_color,
        color_secondary = query.shirt_color,
        color_detail1 = query.shirt_detail_color,
        nodes = {
            {
                name = "arm_l",
                scale = 1.00,
                origin = {-0.50, -0.40, 0.00},
                position = {38.00, 15.00, 0.00},
                rotation = {0.00, 10.00, -85.00},
                size = {30.00, 12.00, 12.00},
                shape = "cube_mirror.mesh",
                uv_map = {76, 0},
                color_primary = query.skin_color,
                color_secondary = query.shirt_color,
                color_detail1 = query.shirt_detail_color,
                nodes = {
                    {
                        name = "forearm_l",
                        scale = 1.00,
                        origin = {-0.50, 0.00, 0.00},
                        position = {27.00, 0.00, 0.00},
                        rotation = {10.00, -20.00, -5.00},
                        size = {30.00, 12.00, 12.00},
                        shape = "cube_mirror.mesh",
                        uv_map = {76, 12},
                        color_primary = query.skin_color,
                        color_secondary = query.shirt_color,
                        color_detail1 = query.shirt_detail_color,
                        nodes = {
                            {
                                name = "hand_l",
                                scale = 1.00,
                                origin = {0.00, 0.00, -0.20},
                                position = {15.00, -1.00, 0.00},
                                rotation = {4.00, 0.00, -8.00},
                                size = {14.00, 16.00, 16.00},
                                shape = "cube_mirror.mesh",
                                uv_map = {76, 24},
                                color_primary = query.skin_color,
                            },
                        }
                    },
                }
            },
            {
                name = "arm_r",
                scale = 1.00,
                origin = {0.50, -0.40, 0.00},
                position = {-38.00, 15.00, 0.00},
                rotation = {0.00, -10.00, 85.00},
                size = {30.00, 12.00, 12.00},
                shape = "cube.mesh",
                uv_map = {76, 0},
                color_primary = query.skin_color,
                color_secondary = query.shirt_color,
                color_detail1 = query.shirt_detail_color,
                nodes = {
                    {
                        name = "forearm_r",
                        scale = 1.00,
                        origin = {0.50, 0.00, 0.00},
                        position = {-27.00, 0.00, 0.00},
                        rotation = {10.00, 20.00, 5.00},
                        size = {30.00, 12.00, 12.00},
                        shape = "cube.mesh",
                        uv_map = {76, 12},
                        color_primary = query.skin_color,
                        color_secondary = query.shirt_color,
                        color_detail1 = query.shirt_detail_color,
                        nodes = {
                            {
                                name = "hand_r",
                                scale = 1.00,
                                origin = {0.00, 0.00, -0.20},
                                position = {-15.00, -1.00, 0.00},
                                rotation = {4.00, -0.00, 8.00},
                                size = {14.00, 16.00, 16.00},
                                shape = "cube.mesh",
                                uv_map = {76, 24},
                                color_primary = query.skin_color,
                            },
                        }
                    },
                }
            },
            {
                name = "belly",
                scale = 1.00,
                position = {0.00, -26.00, 0.00},
                size = {44.00, 26.00, 26.00},
                shape = "cube.mesh",
                uv_map = {0, 82},
                color_primary = query.skin_color,
                color_secondary = query.shirt_color,
                color_detail1 = query.shirt_detail_color,
                nodes = {
                    {
                        name = "hip",
                        scale = 1.00,
                        position = {0.00, -18.00, 0.00},
                        size = {44.00, 10.00, 26.00},
                        shape = "cube.mesh",
                        uv_map = {0, 108},
                        color_primary = query.skin_color,
                        color_secondary = query.pants_color,
                        nodes = {
                            {
                                name = "thig_l",
                                scale = 1.00,
                                origin = {0.00, 0.40, 0.00},
                                position = {11.00, -15.00, 0.00},
                                size = {20.00, 36.00, 20.00},
                                shape = "cube.mesh",
                                uv_map = {76, 40},
                                color_primary = query.skin_color,
                                color_secondary = query.pants_color,
                                nodes = {
                                    {
                                        name = "leg_l",
                                        scale = 1.00,
                                        origin = {0.00, 0.50, 0.50},
                                        position = {0.00, -36.00, 0.00},
                                        size = {18.00, 40.00, 18.00},
                                        shape = "cube.mesh",
                                        uv_map = {76, 68},
                                        color_primary = query.skin_color,
                                        color_secondary = query.pants_color,
                                        nodes = {
                                            {
                                                name = "foot_l",
                                                scale = 1.00,
                                                origin = {0.00, 0.00, -0.30},
                                                position = {0.00, -22.00, 4.00},
                                                size = {22.00, 10.00, 32.00},
                                                shape = "cube.mesh",
                                                uv_map = {76, 97},
                                                color_primary = query.skin_color,
                                                color_secondary = query.shoes_color,
                                            },
                                        }
                                    },
                                }
                            },
                            {
                                name = "thig_r",
                                scale = 1.00,
                                origin = {0.00, 0.40, 0.00},
                                position = {-11.00, -15.00, 0.00},
                                size = {20.00, 36.00, 20.00},
                                shape = "cube.mesh",
                                uv_map = {76, 40},
                                color_primary = query.skin_color,
                                color_secondary = query.pants_color,
                                nodes = {
                                    {
                                        name = "leg_r",
                                        scale = 1.00,
                                        origin = {0.00, 0.50, 0.50},
                                        position = {0.00, -36.00, 0.00},
                                        size = {18.00, 40.00, 18.00},
                                        shape = "cube.mesh",
                                        uv_map = {76, 68},
                                        color_primary = query.skin_color,
                                        color_secondary = query.pants_color,
                                        nodes = {
                                            {
                                                name = "foot_r",
                                                scale = 1.00,
                                                origin = {0.00, 0.00, -0.30},
                                                position = {0.00, -22.00, 4.00},
                                                size = {22.00, 10.00, 32.00},
                                                shape = "cube.mesh",
                                                uv_map = {76, 97},
                                                color_primary = query.skin_color,
                                                color_secondary = query.shoes_color,
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
                position = {0.00, 14.00, 0.00},
                size = {18.00, 16.00, 18.00},
                shape = "cube.mesh",
                uv_map = {0, 38},
                nodes = {
                    {
                        name = "head",
                        scale = 1.00,
                        origin = {0.00, -0.50, 0.00},
                        position = {0.00, 20.00, 0.00},
                        size = {38.00, 38.00, 38.00},
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
