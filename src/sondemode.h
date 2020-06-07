#ifndef SONDEMODE_H
#define SONDEMODE_H


class SondeMode
{
public:
    enum ModeSelection{NoMode, Calibration, Measurement, Custom, Test, LoRa};
    SondeMode();
private:
    ModeSelection mode;
};

#endif // SONDEMODE_H
