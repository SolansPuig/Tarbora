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

namespace Tarbora {
  std::shared_ptr<Resource> MeshResourceLoader::load(const std::string &path)
  {
    std::ifstream file;
    file.open(path.c_str());
    if (file.fail())
      return std::shared_ptr<Resource>();

    // Read the file into a vector
    std::string line;
    std::vector<float> data;
    unsigned int vertices = 0;
    while (std::getline(file, line))
    {
      float value;
      bool valid_line = false;
      std::stringstream ss(line);
      while (ss >> value)
      {
        data.push_back(value);
        valid_line = true;
      }
      if (valid_line) vertices++;
    }

    auto r = std::shared_ptr<Resource>(new Mesh(path, data, vertices));
    return r;
  }
}
