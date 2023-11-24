#include "opcontrol.hpp"

void opcontrol(){

    bool flapState = false;
    bool fourBarState = false;
    bool cataState = false;
    int intakeState = 0;

    while(true){
        double left = -controller.get_analog(ANALOG_LEFT_Y);
		double right = controller.get_analog(ANALOG_RIGHT_Y);

        leftWheelsFront.move(left);
        leftWheelsBack.move(left);  
        rightWheelsFront.move(right);  
        rightWheelsBack.move(right);

        pros::lcd::print(2, "intake: %d", intakeState);
        pros::lcd::print(3, "flaps: %s", flapState ? "true" : "false");
        pros::lcd::print(4, "fourbar: %s", fourBarState ? "true" : "false");
        pros::lcd::print(5, "catapult: %s", cataState ? "true" : "false");

        handle_flaps(flapState);
        handle_intake(intakeState);
        handle_four_bar(fourBarState);
        handle_catapult(cataState);
        pros::delay(TASK_DELAY);
    }
}

void handle_flaps(bool &flapState){
    if (controller.get_digital_new_press(FLAP_TOGGLE_BUTTON)) 
        flapState = !flapState;
    piston.set_value(flapState);
}

void handle_intake(int &intakeState){
    if (controller.get_digital(INTAKE_FOWARD_MANUAL_BUTTON))
        intakeState = 1;
    else if (controller.get_digital(INTAKE_BACKWARD_MANUAL_BUTTON))
        intakeState = -1;
    else 
        intakeState = 0;

    double rotationValue = -intakeState * 127;
    intakeMotor.move(rotationValue);
}
 
void handle_four_bar(bool &fourBarState){
 
    if (controller.get_digital(FOUR_BAR_MANUAL_UP)){
        fourBarMotorRight.move(-127);
        fourBarMotorLeft.move(127);
    }
    else if (controller.get_digital(FOUR_BAR_MANUAL_DOWN)){
        fourBarMotorRight.move(127);
        fourBarMotorLeft.move(-127);
    }
    else {
        hold();
    }
}

void hold () {
    fourBarMotorLeft.brake();
    fourBarMotorRight.brake();
}

void handle_catapult(bool &cataState){
    if (controller.get_digital_new_press(CATAPULT_TOGGLE_BUTTON))
        cataState = !cataState;
    
    if (cataState)
        cataMotor.move(-102);
}