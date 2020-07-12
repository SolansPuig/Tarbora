--[===================================================================[
  * Copyright (C) 2020 Roger Solans Puig
  * Email: roger@solanspuig.cat
  *
  * This file is part of Tarbora. You can obtain a copy at
  * https://github.com/SolansPuig/Tarbora
  *
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at https://mozilla.org/MPL/2.0/.
--]===================================================================]

query = {
  time = 0,
  speed = 4,
}

idle = {
  duration = 1,
  step = 1,

  nodes = {
    crest_1 = {
      position = { [0] = { 0, -1, 0 } },
      rotation = { [0] = { -30, 0, 0 } }
    },
    crest_2 = { rotation = { [0] = { -30, 0, 0 } } },
    crest_3 = { rotation = { [0] = { -30, 0, 0 } } },
  }
}

raise_crest = {
  duration = 1,
  step = 1,

  nodes = {
    crest_1 = { rotation = { [0] = { 0, 0, 0 } } },
    crest_2 = { rotation = { [0] = { 0, 0, 0 } } },
    crest_3 = { rotation = { [0] = { 0, 0, 0 } } },
  }
}

scratching = {
  duration = 4,
  step = 1,

  nodes = {
    leg_r = {
      position = function() return {
          0,
          17 + 0.5 * math.cos(6 * math.pi * query.time),
          2
      } end,
      rotation = function() return {
        -43,
        2,
        11
      } end,
    },
    head = {
      position = { [0] = {0, -3, 5} },
      rotation = function() return {
        25+ 1 * math.cos(2 * math.pi * query.time),
        0+ 1 * math.cos(2 * math.pi * query.time),
        33+ 1 * math.cos(2 * math.pi * query.time)
      } end
    }
  }
}

scared = {
  duration = 4,
  step = 1,

  nodes = {
    crest_1 = { rotation = { [0] = { 10, 0, 0 } } },
    crest_2 = { rotation = { [0] = { 10, 0, 0 } } },
    crest_3 = { rotation = { [0] = { 10, 0, 0 } } },
    wing_r = {
      rotation = function() return { -105.0, 40 * math.cos(11 * math.pi * query.time) -80, 4 } end,
      position = {
        [0] = { 2, -3, -4  },
      },
    },
    wing_l = {
      rotation = function() return { -105.0, -40 * math.cos(11 * math.pi * query.time) +80, -4 } end,
      position = {
        [0] = { -2, -3, -4  },
      },
    },
    feathers_r = {
      scale = {
        [0] = { 10, 16, 0 },
      },
      position = {
        [0] = { 0, -9, -4  },
      },
    },
    feathers_l = {
      scale = {
        [0] = { 10, 16, 0 },
      },
      position = {
        [0] = { 0, -9, -4  },
      },
    }
  }
}

