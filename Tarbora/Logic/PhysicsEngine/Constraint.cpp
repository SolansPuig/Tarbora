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

#include "Constraint.hpp"
#include "PhysicsEngine.hpp"

namespace Tarbora {
  Constraint::Constraint(Rigidbody *A)
  {
    A_ = A->body_;
    original_linear_damping_ = A_->getLinearDamping();
    original_angular_damping_ = A_->getAngularDamping();
    original_angular_factor_ = A_->getAngularFactor();
  }

  void Constraint::setDamping(float linear, float angular)
  {
    A_->setDamping(linear, angular);
  }

  void Constraint::resetDamping()
  {
    A_->setDamping(original_linear_damping_, original_angular_damping_);
  }

  void Constraint::setAngularFactor(const glm::vec3 &axis)
  {
    A_->setAngularFactor(PhysicsEngine::toBullet(axis));
  }

  void Constraint::resetAngularFactor()
  {
    A_->setAngularFactor(original_angular_factor_);
  }

  PointConstraint::PointConstraint(
    Rigidbody *A,
    Rigidbody *B,
    const glm::vec3 &posA,
    const glm::vec3 &posB
  ) : Constraint(A)
  {
    constraint_ = std::make_shared<btPoint2PointConstraint>(
      *(A->body_.get()),
      *(B->body_.get()),
      PhysicsEngine::toBullet(posA),
      PhysicsEngine::toBullet(posB)
    );

    PhysicsEngine::registerConstraint(this);
  }

  PointConstraint::PointConstraint(Rigidbody *A, const glm::vec3 &posA) :
    Constraint(A)
  {
    constraint_ = std::make_shared<btPoint2PointConstraint>(
      *(A->body_.get()), PhysicsEngine::toBullet(posA)
    );

    PhysicsEngine::registerConstraint(this);
  }

  PointConstraint::~PointConstraint()
  {
    resetDamping();
    resetAngularFactor();
    PhysicsEngine::deleteConstraint(this);
  }

  void PointConstraint::setPosA(const glm::vec3 &posA)
  {
    constraint_->setPivotA(PhysicsEngine::toBullet(posA));
  }

  void PointConstraint::setPosB(const glm::vec3 &posB)
  {
    constraint_->setPivotB(PhysicsEngine::toBullet(posB));
  }

  std::shared_ptr<btTypedConstraint> PointConstraint::getConstraint()
  {
    return constraint_;
  }
}
