components = {
    type = {"magic", "projectile", "arcane"},
    model = {
        render_pass = 1,
        model = "orb.lua",
        material = "mage_wand.mat.lua"
    },
    rigidbody= {
        shape = "sphere",
        radius = 0.3,
        density = 90,
        friction = 0.5,
        restitution = 3.0
    },
}
