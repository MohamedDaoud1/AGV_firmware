
#ifndef HandleIncEn_H
#define HandleIncEn_H

#define WheelRadius 10
#define PPR 100
#define Pi 3.141592654
#define pScale 1

extern double InitDistanceFactor(void);
extern int16_t FindDistanceR(void);
extern int16_t FindDistanceL(void);
extern void UpdateDistances(void);
extern void FindSpeed(void);

#endif
