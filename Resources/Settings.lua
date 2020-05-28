phrases = {
  "Tarbora Game Engine",
  "What a beautiful day to play Tarbora!",
  "Tarbora: The Game Engine for Makers",
  "Tarborize Your Life!",
  "Now featuring extra bugs",
  "...!",
  "Doot",
  "Stay home, play Tarbora"
}

window = {
    title = function() return phrases[math.random(#phrases)] end,
    size = { 1600, 900 },
    postprocessShader = "shaders/postprocess.shader.lua"
}
