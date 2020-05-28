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

#ifndef __TEXTURE_INTERNAL_H_
#define __TEXTURE_INTERNAL_H_

#include "../../../Framework/Global.hpp"
#include "Window.hpp"

namespace Tarbora {
  class TextureInternal
  {
  public:
    TextureInternal(int width, int height, int components, void *data);
    TextureInternal(
      int width, int height, unsigned int format,
      unsigned int type=GL_UNSIGNED_BYTE, void *data=nullptr
    );
    TextureInternal(
      int width, int height, unsigned int internalFormat,
      unsigned int type, unsigned int format, void *data=nullptr
    );

    ~TextureInternal();

    void configure(unsigned int min_filter, unsigned int mag_filter);
    void configure(
      unsigned int min_filter, unsigned int mag_filter,
      unsigned int wrapS, unsigned int wrapT
    );

    void bind(unsigned int channel=0) const;

    unsigned int getId() const { return id_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

  private:
    void build(void *data, unsigned int format, unsigned int type);
    void build(
      void *data, unsigned int internal_format, unsigned int type, unsigned int format
    );
    void deleteTexture();

    unsigned int id_;
    int width_, height_;
  };
}

#endif // __TEXTURE_INTERNAL_H_
