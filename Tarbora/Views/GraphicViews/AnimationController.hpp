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

#ifndef __ANIMATIONCONTROLLER_H_
#define __ANIMATIONCONTROLLER_H_

#include "../../Framework/Module/Module.hpp"

namespace Tarbora {
  enum StopMode
  {
    Now,
    EndLoop,
    FadeOut,
    EndLoopFadingOut
  };

  enum BlendMode
  {
    Override,
    Add,
    Mix,
  };

  class ActorModel;

  struct Animation
  {
    std::string name;
    std::string file;
    BlendMode blend_mode;
    float blend_factor;
    float fade_in_timer;
    bool loop;

    float timer{0.f};
    float duration{0.f};
    float fade_out_timer{-1.f};

    bool update(float delta_time, ActorModel *actor);

    template <class T>
    T blendProperty(T value, T old_value, const Animation *animation);
  };

  class AnimationController
  {
  public:
    AnimationController(ActorModel *actor);

    void update(float delta_time);

    void startAnimation(Animation animation, bool background=false);

    void endAnimation(const std::string &name, StopMode mode, float fade_out_timer);

  private:
    void updateAnimation(float delta_time);

    template <class T>
    T blendProperty(T value, T old_value, const Animation *animation)
    {
      // Fade-in or fade-out if required
      float blend = animation->blend_factor;
      if (animation->fade_in_timer > 0.f)
        blend = glm::lerp(blend, 0.f, animation->fade_in_timer);
      if (animation->fade_out_timer > 0.f)
        blend = glm::lerp(0.f, blend, animation->fade_in_timer);

      // Blend the property
      switch (animation->blend_mode)
      {
        case Override:
          return value;
        case Add:
          if (animation->fade_in_timer > 0.f || animation->fade_out_timer > 0.f)
            return glm::lerp(T(0.f), value, blend) + old_value;
          return value + old_value;
        case Mix:
          return glm::lerp(old_value, value, blend);
      }
      return value;
    }

    std::list<Animation> animations_;

    ActorModel *actor_model_;
    float global_timer_;
  };

  template <class T>
  inline T Animation::blendProperty(T value, T old_value, const Animation *animation)
  {
    // Fade-in or fade-out if required
    float blend = animation->blend_factor;
    if (animation->fade_in_timer > 0.f)
      blend = glm::lerp(blend, 0.f, animation->fade_in_timer);
    if (animation->fade_out_timer > 0.f)
      blend = glm::lerp(0.f, blend, animation->fade_out_timer);

    // Blend the property
    switch (animation->blend_mode)
    {
      case Override:
        return value;
      case Add:
        if (animation->fade_in_timer > 0.f || animation->fade_out_timer > 0.f)
          return glm::lerp(T(0.f), value, blend) + old_value;
        return value + old_value;
      case Mix:
        return glm::lerp(old_value, value, blend);
    }
    return value;
  }

  template <>
  inline glm::quat Animation::blendProperty(
    glm::quat value, glm::quat old_value, const Animation *animation
  )
  {
    // Fade-in or fade-out if required
    float blend = animation->blend_factor;
    if (animation->fade_in_timer > 0.f)
      blend = glm::lerp(blend, 0.f, animation->fade_in_timer);
    if (animation->fade_out_timer > 0.f)
      blend = glm::lerp(0.f, blend, animation->fade_out_timer);


    // Blend the property
    switch (animation->blend_mode)
    {
      case Override:
        return value;
      case Add:
        if (animation->fade_in_timer > 0.f || animation->fade_out_timer > 0.f)
          return old_value * glm::lerp(glm::quat(1.f, 0.f, 0.f, 0.f), value, blend);
        return old_value * value;
      case Mix:
        return glm::mix(old_value, value, blend);
    }
    return value;
  }
}

#endif // __ANIMATIONCONTROLLER_H_
