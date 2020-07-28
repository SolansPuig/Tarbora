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

#ifndef __MESH_INTERNAL_H_
#define __MESH_INTERNAL_H_

#include "../../../Framework/Global.hpp"

namespace Tarbora {
  typedef unsigned int MeshId;

  struct RenderElementData
  {
    glm::mat4 transform;
    glm::tvec2<unsigned short> uv_map;
    glm::vec3 mesh_size;
    glm::vec3 texture_size;
    glm::tvec3<unsigned char> color_primary;
    glm::tvec3<unsigned char> color_secondary;
    glm::tvec3<unsigned char> color_detail;
    glm::tvec3<unsigned char> color_emissive;
  };

  class MeshInternal
  {
  public:
    MeshInternal(const std::vector<float> &data, unsigned int vertices);
    ~MeshInternal();

    void bind();
    void draw();
    void addInstance(const RenderElementData &data);
    void drawInstanced();

    const MeshId &getId() { return vao_; }
    int getVertices() const { return vertices_; }

  private:
    void build(const std::vector<float> &data);
    void deleteMesh();

    unsigned int vao_;
    unsigned int data_buffer_;
    int vertices_;
    std::vector<RenderElementData> instance_data_;
  };
}

#endif // __MESH_INTERNAL_H_
