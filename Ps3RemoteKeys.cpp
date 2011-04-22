
#ifndef ARDUINO

#include "Ps3RemoteKeys.h"
#include <cstddef>
#include <string.h>

const PS3REMOTEKEYS::keys_pair PS3REMOTEKEYS::keys[] = \
{
  { "NUM3"        , NUM3        },
  { "NUM5"        , NUM5        },
  { "NUM7"        , NUM7        },
  { "SUBTITLE"    , SUBTITLE    },
  { "RETURN"      , RETURN      },
  { "PLAY"        , PLAY        },
  { "PREV"        , PREV        },
  { "NUM2"        , NUM2        },
  { "NUM4"        , NUM4        },
  { "ANGLE"       , ANGLE       },
  { "BLUE"        , BLUE        },
  { "POPUP"       , POPUP       },
  { "SCAN_BCK"    , SCAN_BCK    },
  { "STEP_FWD"    , STEP_FWD    },
  { "NUM1"        , NUM1        },
  { "AUDIO"       , AUDIO       },
  { "TIME"        , TIME        },
  { "YELLOW"      , YELLOW      },
  { "TOP_MENU"    , TOP_MENU    },
  { "R3"          , R3          },
  { "PAUSE"       , PAUSE       },
  { "EJECT"       , EJECT       },
  { "NUM9"        , NUM9        },
  { "GREEN"       , GREEN       },
  { "PS3_DISPLAY" , PS3_DISPLAY },
  { "NEXT"        , NEXT        },
  { "START"       , START       },
  { "NUM6"        , NUM6        },
  { "NUM8"        , NUM8        },
  { "CLEAR"       , CLEAR       },
  { "RED"         , RED         },
  { "SCAN_FWD"    , SCAN_FWD    },
  { "STOP"        , STOP        },
  { "STEP_BCK"    , STEP_BCK    },
  { "L3"          , L3          },
  { "CIRCLE"      , CIRCLE      },
  { "TRIANGLE"    , TRIANGLE    },
  { "ENTER"       , ENTER       },
  { "CROSS"       , CROSS       },
  { "SQUARE"      , SQUARE      },
  { "PS"          , PS          },
  { "R2"          , R2          },
  { "SELECT"      , SELECT      },
  { "UP"          , UP          },
  { "LEFT"        , LEFT        },
  { "RIGHT"       , RIGHT       },
  { "DOWN"        , DOWN        },
  { "R1"          , R1          },
  { "L1"          , L1          },
  { "L2"          , L2          },
  { NULL          , NULL        }
  // { ""            , NULL                      }
};

uint16_t PS3REMOTEKEYS::str2sym(const char * str)
// uint16_t PS3REMOTE::str2sym(const string str)
{
  int i = 0;
  while( keys[i].str != NULL )
  // while(! keys[i].str.empty() )
  {
    // if(str == keys[i].str)
    if( !strcmp(str, keys[i].str) )
    {
      // printf("0x%.2x\n", keys[i].sym);
      // return (uint16_t)keys[i].sym;
      return (uint8_t)keys[i].sym;
    }
    i++;
  }
}


#endif // !ARDUINO

