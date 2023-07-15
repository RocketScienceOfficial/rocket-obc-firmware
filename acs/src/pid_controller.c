#include "acs/pid_controller.h"

VOID pidControllerInit(PIDControllerData *pid, FLOAT kp, FLOAT ki, FLOAT kd, FLOAT dt)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->dt = dt;
    pid->lastError = 0;
    pid->integral = 0;
}

FLOAT pidControllerUpdate(PIDControllerData *pid, FLOAT expected, FLOAT actual)
{
    FLOAT error = expected - actual;

    pid->integral += error * pid->dt;

    FLOAT p = pid->kp * error;
    FLOAT i = pid->ki * pid->integral;
    FLOAT d = pid->kd * (error - pid->lastError) / pid->dt;

    pid->lastError = error;

    return p + i + d;
}