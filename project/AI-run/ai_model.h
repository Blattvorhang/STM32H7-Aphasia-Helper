#ifndef AI_MODEL_H
#define AI_MODEL_H

#include "ai_platform.h"

int aiInit(void);
int aiRun(const void *in_data, void *out_data);
void aiInference(void);

#endif /* AI_MODEL_H */
