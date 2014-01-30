#include "JohannKosselSolution.h"
#include <fastmath.h>

#define SQ(x) powf(x, 2)

JohannKosselSolution::JohannKosselSolution(Config* config)
{
    // arm_length is the length of the arm from hinge to hinge
    arm_length         = config->value(arm_length_checksum)->by_default(250.0f)->as_number();
    // arm_radius is the horizontal distance from hinge to hinge when the effector is centered
    arm_radius         = config->value(arm_radius_checksum)->by_default(124.0f)->as_number();

    init();
}

void JohannKosselSolution::init() {
    arm_length_squared = SQ(arm_length);

    // Effective X/Y positions of the three vertical towers.
    float DELTA_RADIUS = arm_radius;

    float SIN_60   = 0.8660254037844386F;
    float COS_60   = 0.5F;

    DELTA_TOWER1_X = -SIN_60 * DELTA_RADIUS; // front left tower
    DELTA_TOWER1_Y = -COS_60 * DELTA_RADIUS;

    DELTA_TOWER2_X =  SIN_60 * DELTA_RADIUS; // front right tower
    DELTA_TOWER2_Y = -COS_60 * DELTA_RADIUS;

    DELTA_TOWER3_X = 0.0F; // back middle tower
    DELTA_TOWER3_Y = DELTA_RADIUS;
}

void JohannKosselSolution::cartesian_to_actuator( float cartesian_mm[], float actuator_mm[] ){

    actuator_mm[ALPHA_STEPPER] = sqrtf(this->arm_length_squared
                                - SQ(DELTA_TOWER1_X - cartesian_mm[X_AXIS])
                                - SQ(DELTA_TOWER1_Y - cartesian_mm[Y_AXIS])
                                ) + cartesian_mm[Z_AXIS];
    actuator_mm[BETA_STEPPER ] = sqrtf(this->arm_length_squared
                                - SQ(DELTA_TOWER2_X - cartesian_mm[X_AXIS])
                                - SQ(DELTA_TOWER2_Y - cartesian_mm[Y_AXIS])
                                ) + cartesian_mm[Z_AXIS];
    actuator_mm[GAMMA_STEPPER] = sqrtf(this->arm_length_squared
                                - SQ(DELTA_TOWER3_X - cartesian_mm[X_AXIS])
                                - SQ(DELTA_TOWER3_Y - cartesian_mm[Y_AXIS])
                                ) + cartesian_mm[Z_AXIS];
}

void JohannKosselSolution::actuator_to_cartesian( float actuator_mm[], float cartesian_mm[] ){
    // unimplemented
}

bool JohannKosselSolution::set_optional(char parameter, float value) {

    switch(parameter) {
        case 'L': // sets arm_length
            arm_length= value;
            break;
        case 'R': // sets arm_radius
            arm_radius= value;
            break;
        default:
            return false;
    }
    init();
    return true;
}

bool JohannKosselSolution::get_optional(char parameter, float *value) {
    if(value == NULL) return false;

    switch(parameter) {
        case 'L': // get arm_length
            *value= this->arm_length;
            break;
        case 'R': // get arm_radius
            *value= this->arm_radius;
            break;
        default:
            return false;
    }

    return true;
};
