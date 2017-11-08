#ifndef LOGGER_H
#define LOGGER_H

class Logger {
  public:
  Logger();
  void begin();

  void start();
  void stop();
  void update();

  bool recording;
  unsigned long last;
  unsigned int linesWritten;
};

#endif

