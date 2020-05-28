/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#include "GraphicsEngine.hpp"
#include "TextureInternal.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../../Framework/External/stb_image.h"

namespace Tarbora {
  std::shared_ptr<Resource> TextureResourceLoader::load(const std::string &path)
  {
    ZoneScoped;

    // Load the raw image
    int components, width, height;
    void *data = stbi_load(path.c_str(), &width, &height, &components, 0);
    if (data == nullptr)
    {
      LOG_ERR(
        "TextureLoader: The image %s failed to load due to: %s",
        path.c_str(), stbi_failure_reason()
      );
      data = stbi_load(
        "../Resources/textures/missing.png", &width, &height, &components, 0
      );

      if (data == nullptr)
      {
        LOG_ERR(
          "TextureLoader: The image textures/missing.png failed to load due to: %s",
          stbi_failure_reason()
        );
      }

      return std::shared_ptr<Resource>();
    }

    std::shared_ptr<Resource> r = std::shared_ptr<Resource>(
      new Texture(path, width, height, components, data)
    );

    // Delete the image
    stbi_image_free(data);

    // Configure the texture
    std::static_pointer_cast<Texture>(r)->configure(
      GL_NEAREST,
      GL_NEAREST,
      GL_CLAMP_TO_EDGE,
      GL_CLAMP_TO_EDGE
    );

    return r;
  }
}
