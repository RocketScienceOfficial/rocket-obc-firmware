#include "maths/kalman_filter.h"
#include "maths/matrix.h"
#include "maths/vector.h"

VOID kalmanUpdate(KalmanOutputData *output, vec3 acc)
{
    matMxN state = matMxNCreate(9, 9);
}