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

#ifndef __GRAPHICVIEW_H_
#define __GRAPHICVIEW_H_

#include "../../Framework/Module/Module.hpp"
#include "GraphicsEngine/GraphicsEngine.hpp"

namespace Tarbora {
  class GraphicView : public Module
  {
  public:
    GraphicView(const ClientId &client_id, const std::string &settings_file)
      : Module(client_id)
    {
      graphics_engine_ = std::make_shared<GraphicsEngine>(this, settings_file);
    }

    std::shared_ptr<GraphicsEngine> getGraphicsEngine() { return graphics_engine_; }

  private:
    std::shared_ptr<GraphicsEngine> graphics_engine_;
  };
}

#endif // __GRAPHICVIEW_H_
