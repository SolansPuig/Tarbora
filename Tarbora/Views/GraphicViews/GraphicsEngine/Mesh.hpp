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

#ifndef __MESH_RESOURCE_H_
#define __MESH_RESOURCE_H_

#include "../../../Framework/ResourceManager/Resource.hpp"
#include "MeshInternal.hpp"

namespace Tarbora {

  class Mesh : public Resource, public MeshInternal
  {
    friend class MeshResourceLoader;
  private:
    Mesh(
      const std::string &name,
      const std::vector<float> &data,
      unsigned int vertices
    ) : Resource(name), MeshInternal(data, vertices) {}
  };

  //! \cond HIDDEN_SYMBOLS
  class MeshResourceLoader : public ResourceLoader
  {
    friend class ResourceManager;
  private:
    virtual const std::string getPattern() override { return ".mesh"; };
    virtual std::shared_ptr<Resource> load(const std::string &path) override;
  };
  //! \endcond
}

#endif // __MESH_RESOURCE_H_
