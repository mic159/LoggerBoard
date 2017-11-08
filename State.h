#ifndef STATE_H
#define STATE_H

enum SD_STATE {
  SD_NOCARD,
  SD_BADCARD,
  SD_INSERTED,
};

struct state_t {
  SD_STATE sd;
};

#endif

