#ifdef Z2
#define Z 2
#define POWZ(x) ((x)*(x))
#define INVPOWZ(x) sqrt(x)
#else
#define Z 1
#define POWZ(x) (x)
#define INVPOWZ(x) (x)
#endif
