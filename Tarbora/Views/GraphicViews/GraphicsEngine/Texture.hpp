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

#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#include "../../../Framework/ResourceManager/Resource.hpp"
#include "TextureInternal.hpp"

namespace Tarbora {
  class Texture : public Resource, public TextureInternal
  {
    friend class TextureResourceLoader;

  private:
    Texture(const std::string &name, int width, int height, int components, void *data)
      : Resource(name), TextureInternal(width, height, components, data) {}
  };

  //! \cond HIDDEN_SYMBOLS
  class TextureResourceLoader : public ResourceLoader
  {
    friend class ResourceManager;
  private:
    virtual const std::string getPattern() override { return ".png"; };
    virtual std::shared_ptr<Resource> load(const std::string &path) override;
  };
  //! \endcond
}

#endif // __TEXTURE_H_
