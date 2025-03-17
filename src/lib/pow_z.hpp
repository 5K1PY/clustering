#ifdef Z2
#define POWZ(x) ((x)*(x))
#define INVPOWZ(x) sqrt(x)
#else
#define POWZ(x) (x)
#define INVPOWZ(x) (x)
#endif
