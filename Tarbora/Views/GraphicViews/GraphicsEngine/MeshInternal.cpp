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

#include "MeshInternal.hpp"
#include "Window.hpp"

namespace Tarbora {
  MeshInternal::MeshInternal(const std::vector<float> &data, unsigned int vertices)
  {
    vertices_ = vertices;
    build(data);
  }

  MeshInternal::~MeshInternal()
  {
    deleteMesh();
  }

  void MeshInternal::bind()
  {
    glBindVertexArray(vao_);
  }

  void MeshInternal::addInstance(const RenderElementData &data)
  {
    instance_data_.push_back(data);
  }

  void MeshInternal::draw()
  {
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertices_);
  }

  void MeshInternal::drawInstanced()
  {
    unsigned int amount = instance_data_.size();
    glBindBuffer(GL_ARRAY_BUFFER, data_buffer_);
    glBufferData(
      GL_ARRAY_BUFFER,
      amount * sizeof(RenderElementData),
      &instance_data_[0],
      GL_DYNAMIC_DRAW
    );
    glBindVertexArray(vao_);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertices_, amount);
    instance_data_.clear();
  }

  void MeshInternal::build(const std::vector<float> &data)
  {
    unsigned int VBO;
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE,
      8 * sizeof(float),
      (void*)0
    );
    glEnableVertexAttribArray(1); // normal
    glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE,
      8 * sizeof(float),
      (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(2); // vertex id
    glVertexAttribPointer(
      2, 2, GL_FLOAT, GL_FALSE,
      8 * sizeof(float),
      (void*)(6 * sizeof(float))
    );

    glGenBuffers(1, &data_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, data_buffer_);
    glEnableVertexAttribArray(3); // transform[0]
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 104, (void*)0);
    glEnableVertexAttribArray(4); // transform[1]
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 104, (void*)16);
    glEnableVertexAttribArray(5); // transform[2]
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 104, (void*)32);
    glEnableVertexAttribArray(6); // transform[3]
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 104, (void*)48);
    glEnableVertexAttribArray(7); // UV Map
    glVertexAttribPointer(7, 2,  GL_UNSIGNED_SHORT, GL_FALSE, 104, (void*)64);
    glEnableVertexAttribArray(8); // Mesh size
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, 104, (void*)68);
    glEnableVertexAttribArray(9); // Texture size
    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 104, (void*)80);
    glEnableVertexAttribArray(10); // Color primary
    glVertexAttribPointer(10, 3, GL_UNSIGNED_BYTE, GL_TRUE, 104, (void*)92);
    glEnableVertexAttribArray(11); // Color secondary
    glVertexAttribPointer(11, 3, GL_UNSIGNED_BYTE, GL_TRUE, 104, (void*)95);
    glEnableVertexAttribArray(12); // Color detail
    glVertexAttribPointer(12, 3, GL_UNSIGNED_BYTE, GL_TRUE, 104, (void*)98);
    glEnableVertexAttribArray(13); // Color emissive
    glVertexAttribPointer(13, 3, GL_UNSIGNED_BYTE, GL_TRUE, 104, (void*)101);

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);
    glVertexAttribDivisor(9, 1);
    glVertexAttribDivisor(10, 1);
    glVertexAttribDivisor(11, 1);
    glVertexAttribDivisor(12, 1);
    glVertexAttribDivisor(13, 1);
  }

  void MeshInternal::deleteMesh()
  {
    glDeleteVertexArrays(1, &vao_);
  }
}
