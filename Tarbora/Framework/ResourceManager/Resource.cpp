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

#include "Resource.hpp"

namespace Tarbora {
  std::shared_ptr<Resource> TextResourceLoader::load(const std::string &path)
  {
    std::shared_ptr<Resource> r;
    {
      // Try to open the file.
      std::ifstream file(path.c_str());
      if (!file.fail())
      {
        // Parse the contents to a string.
        const std::string s = std::string(
          std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()
        );
        r = std::shared_ptr<Resource>(new Text(path, s));
      }
    }

    return r;
  }
}
