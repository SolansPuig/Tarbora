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

#ifndef __LAYER_H_
#define __LAYER_H_

#include "../../Framework/Module/ModuleComponent.hpp"
#include "GraphicView.hpp"

namespace Tarbora {
  class HumanView;
 
  class Layer : public ModuleComponent
  {
  public:
    Layer(HumanView *view, bool start_active = true);
    virtual ~Layer() {}

    virtual void onActivate() {}
    virtual void onDeactivate() {}

    virtual bool getInput() { return false; }
    virtual void update(float delta_time) { (void)(delta_time); }
    virtual void draw() {}

    void setActive(bool active)
    {
      active_ = active;
      if (active_) onActivate();
      else onDeactivate();
    }

    void toggleActive()
    {
      setActive(!active_);
    }

    bool isActive() const
    {
      return active_;
    }

    inline std::shared_ptr<Input> getInputManager()
    {
      return static_cast<GraphicView*>(module_)->getGraphicsEngine()->getInputManager();
    }

  protected:
    bool active_;
    bool event_blocking_;

    HumanView *view_;
  };
}

#endif //__LAYER_H_
