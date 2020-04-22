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

#ifndef __CONSTRAINT_H_
#define __CONSTRAINT_H_

#include "Rigidbody.hpp"

namespace Tarbora {
  class Constraint
  {
  public:
    Constraint(Rigidbody *A);

    void setDamping(float linear, float angular);
    void resetDamping();

    void setAngularFactor(const glm::vec3 &axis);
    void resetAngularFactor();

    virtual std::shared_ptr<btTypedConstraint> getConstraint() = 0;

  protected:
    std::shared_ptr<btRigidBody> A_;

    float original_linear_damping_;
    float original_angular_damping_;
    btVector3 original_angular_factor_;
  };

  class PointConstraint : public Constraint
  {
  public:
    PointConstraint(
      Rigidbody *A,
      Rigidbody *B,
      const glm::vec3 &posA,
      const glm::vec3 &posB
    );
    PointConstraint(Rigidbody *A, const glm::vec3 &posA);
    virtual ~PointConstraint();
       
    void setPosA(const glm::vec3 &posA);
    void setPosB(const glm::vec3 &posB);

    virtual std::shared_ptr<btTypedConstraint> getConstraint() override;

  private:
    std::shared_ptr<btRigidBody> B_;
    std::shared_ptr<btPoint2PointConstraint> constraint_;
  };
}

#endif // __CONSTRAINT_H_
