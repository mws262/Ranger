#include <mb_includes.h> 
#include <motorControl.h>
#include <fsm.h>
#include <unit_test.h>
#include "Trajectory.h"
#include "TrajData.h"
#include "RangerMath.h"	//for Sin()	and TWO_PI and PI
		   
#define DATA TRAJ_DATA_Test0  //used in the function test_trajectory() 

static float hip_kp, hip_kd, scissor_offset, scissor_rate, ank_kp, ank_kd, ank_hold, ank_flip;

/* Updates parameters (used in unit_test) from LABVIEW. */
void param_update_test(void){
	hip_kp = mb_io_get_float(ID_CTRL_HIP_KP);	  //16, 27
	hip_kd = mb_io_get_float(ID_CTRL_HIP_KD);	  //3, 3.8
	scissor_offset = 0.1;
	scissor_rate = 1.3;
	
	ank_kp = 7;
	ank_kd = 1;
	ank_hold = mb_io_get_float(ID_CTRL_ANK_REF_HOLD); 
	ank_flip = mb_io_get_float(ID_CTRL_ANK_REF_FLIP); 
}

/* testStates for testing the gyro angle */
enum testStates {
	INIT,
	IN,
	OUT,
};

static enum testStates test_state = INIT; 

void test_gyro_angle_init(void){
	test_state = INIT;
}

/* Unit test function for gyro angle correction.
 * Put Ranger in double stance, updates the gyro angle using the gyro angle calculated from geometry 
 * every time only one set of its feet are on ground. 
 * Expected results:
 * 		gyro angle becomes more accurate when Ranger is rocked back and forth   
 */
void test_gyro_angle(void){
	struct ControllerData ctrlHip;
	struct ControllerData ctrlAnkOut;
	struct ControllerData ctrlAnkInn;
	float hip_track = 0.2;

	angles_update();
	param_update_test();
	
	out_ank_track_abs(&ctrlAnkOut, ank_hold, 0.0, 0.0, ank_kp, ank_kd);
	inn_ank_track_abs(&ctrlAnkInn, ank_hold, 0.0, 0.0, ank_kp, ank_kd);
	hip_track_rel(&ctrlHip, hip_track, 0.0, hip_kp, hip_kd);	

	switch(test_state){
		case INIT:
			mb_io_set_float(ID_CTRL_TEST_W1, 1);
			if(FO_on_ground() && !FI_on_ground()){
				correct_gyro_angle();
				test_state = OUT;							
			}
			if(FI_on_ground() && !FO_on_ground()){
				correct_gyro_angle();
				test_state = IN;
			}
			break;
		case IN:
			mb_io_set_float(ID_CTRL_TEST_W1, 2);
			if(FO_on_ground() && !FI_on_ground()){
				correct_gyro_angle();
				test_state = OUT;
			}	
			break;
		case OUT:
			mb_io_set_float(ID_CTRL_TEST_W1, 3);
			if(FI_on_ground() && !FO_on_ground()){
				correct_gyro_angle();
				test_state = IN;
			}
			break;
	}

	//run controllers
	controller_ankleInner(&ctrlAnkInn);
	controller_ankleOuter(&ctrlAnkOut);
	controller_hip(&ctrlHip);
}


/* Tests gravity compensation with PD controller turned off (one leg on ground, one leg in air).
 * Parameters:
 *   - ID_CTRL_TEST_R0 = reference hip angle
 * Expected behavior: 
 * 	 - gravity compensation should be able hold the legs at the reference angle 
 *	 -(may need to manually move the leg to the reference angle first due to friction)	
 */
void test_gravity_compensation(void){
	struct ControllerData ctrlHip;
	struct ControllerData ctrlAnkOut;
	struct ControllerData ctrlAnkInn;
	float on_ground = 0; // 0=outer leg on ground, 1=inner leg on ground

	float th1_test = mb_io_get_float(ID_CTRL_TEST_R0);
	float th0_test = mb_io_get_float(ID_CTRL_TEST_R0);

	float leg_m = 2.5;
	float leg_r = 0.15;
	float g = 9.8;
	float u = leg_m * g * leg_r;

   	//calculate gravity compensation 
	if(FI_on_ground() && !FO_on_ground()){
		//only inner feet on ground
		u = -u * Sin(th0_test); 
	}else if(!FI_on_ground() && FO_on_ground()){
		//only outer feet on ground 	
		u =  u * Sin(th1_test);	
	}else{
		u = 0;
	}

	angles_update();
	param_update_test();
	
	//sets correct ankle angles 	
	if(on_ground == 0){
		out_ank_track_abs(&ctrlAnkOut, ank_hold, 0.0, 0.0, ank_kp, ank_kd);
		inn_ank_track_abs(&ctrlAnkInn, ank_flip, 0.0, 0.0, ank_kp, ank_kd);
	}else{
		out_ank_track_abs(&ctrlAnkOut, ank_flip, 0.0, 0.0, ank_kp, ank_kd);
		inn_ank_track_abs(&ctrlAnkInn, ank_hold, 0.0, 0.0, ank_kp, ank_kd);	
	}

	//turns off pd controller, only include gravity compensation
	ctrlHip.kp = 0.0;
	ctrlHip.kd = 0.0;
	ctrlHip.xRef = 0.0;
	ctrlHip.vRef = 0.0;
	ctrlHip.uRef = u;	

	//run controllers
	controller_ankleInner(&ctrlAnkInn);
	controller_ankleOuter(&ctrlAnkOut);
	controller_hip(&ctrlHip);
}

/* Test hip controller with outer feet on ground and inner feet flipped up. 
 * Parameters:
 *   - ID_CTRL_TEST_W1 = reference hip angle
 */
void test_hip_outer(void){
	struct ControllerData ctrlHip;
	struct ControllerData ctrlAnkOut;
	struct ControllerData ctrlAnkInn;

	float hip_track = mb_io_get_float(ID_CTRL_TEST_R0);

	angles_update();
	param_update_test();
	
	out_ank_track_abs(&ctrlAnkOut, ank_hold, 0.0, 0.0, ank_kp, ank_kd);
	inn_ank_track_abs(&ctrlAnkInn, ank_flip, 0.0, 0.0, ank_kp, ank_kd);
	// tests hip scissor tracking function
	hip_scissor_track_outer(&ctrlHip, scissor_offset, scissor_rate, hip_kp, hip_kd); 
	// tests hip relative angle tracking function
	//mb_io_set_float(ID_CTRL_TEST_W1, hip_track);
	//hip_track_rel(&ctrlHip, hip_track, 0.0, hip_kp, hip_kd);	

	controller_hip(&ctrlHip);
	controller_ankleInner(&ctrlAnkInn);
	controller_ankleOuter(&ctrlAnkOut);
} 


/* Inner leg standing, swing outer leg */
void test_hip_inner(void){
	struct ControllerData ctrlHip;
	struct ControllerData ctrlAnkOut;
	struct ControllerData ctrlAnkInn;

	float hip_track = mb_io_get_float(ID_CTRL_TEST_R0);

	angles_update();
	param_update_test();
	
	out_ank_track_abs(&ctrlAnkOut, ank_flip, 0.0, 0.0, ank_kp, ank_kd);
	inn_ank_track_abs(&ctrlAnkInn, ank_hold, 0.0, 0.0, ank_kp, ank_kd);
	// tests inner hip scissor tracking function
	hip_scissor_track_inner(&ctrlHip, scissor_offset, scissor_rate, hip_kp, hip_kd); 
	//simple_scissor_track_inner(&ctrlHip, hip_kp, hip_kd);
	// tests hip relative angle tracking function
	//mb_io_set_float(ID_CTRL_TEST_W1, hip_track);
	//hip_track_rel(&ctrlHip, hip_track, 0.0, hip_kp, hip_kd);	

	controller_hip(&ctrlHip);
	controller_ankleInner(&ctrlAnkInn);
	controller_ankleOuter(&ctrlAnkOut);
}


/* Robot inner leg standing, swings outer leg
 * Hip motor tracks Sine-curve reference
 */
void hip_motor_test(void){
	struct ControllerData ctrlHip;
	struct ControllerData ctrlAnkOut;
	struct ControllerData ctrlAnkInn;
	float qUpp = 0.2;  // Maximum ankle angle
	float qLow = -0.2;  // minimum ankle angle					   

	float time = 0.001 * mb_io_get_float(ID_TIMESTAMP);

	float period;  // period for reference function
	float arg;  // input for trig functions
	float xRef;  // reference ankle angle
	float vRef;  // reference angle rate
	float kp;   // proportional gaint
	float kd;   // derivative gain		
	
	period = 2;

	arg = TWO_PI*time/period;
	xRef = 0.5*(qLow + qUpp) + 0.5*(qUpp-qLow)*Sin(arg);

	vRef = (qUpp-qLow)*(PI/period)*Cos(arg);

	//mb_io_set_float(ID_CTRL_TEST_W7,xRef);
	//mb_io_set_float(ID_CTRL_TEST_W8,vRef);

	//kp=28, kd=4, give good tracking for the ankles 
	kp = 16; //mb_io_get_float(ID_CTRL_TEST_R0); 
	kd = 3; //mb_io_get_float(ID_CTRL_TEST_R1);

	ctrlHip.kp = kp;
	ctrlHip.kd = kd;
	ctrlHip.xRef = xRef;
	ctrlHip.vRef = vRef;
	//ctrlHip.uRef = 0.0;
	ctrlHip.uRef = hip_gravity_compensation();	//does not help much
	
	angles_update();
	param_update_test();
	out_ank_track_abs(&ctrlAnkOut, ank_flip, 0.0, 0.0, ank_kp, ank_kd);
	inn_ank_track_abs(&ctrlAnkInn, ank_hold, 0.0, 0.0, ank_kp, ank_kd);
	
	controller_ankleInner(&ctrlAnkInn);
	controller_ankleOuter(&ctrlAnkOut);
	controller_hip(&ctrlHip);
}


/* Robot Hanging in the air
 * Hip motor off
 * Ankle motors track Sin(Sin(t)) reference (with useful paramters)
 * Parameters:
 *  // - ID_CTRL_TEST_R0 = kp
 *	// - ID_CTRL_TEST_R1 = kd
 *   - ID_CTRL_TEST_W7 = reference angle
 *	 - ID_CTRL_TEST_W8 = reference angular rate
 */
void ankle_motor_test(void){
	struct ControllerData ctrlAnkOut;
	struct ControllerData ctrlAnkInn;

	float qUpp = 2.4;  // Maximum ankle angle
	float qLow = 0.2;  // minimum ankle angle					   

	float time = 0.001 * mb_io_get_float(ID_TIMESTAMP);

	float frequency;  // period for reference function
	float arg;  // input for trig functions
	float xRef;  // reference ankle angle
	float vRef;  // reference angle rate
	float kp;   // proportional gaint
	float kd;   // derivative gain		

	frequency = mb_io_get_float(ID_CTRL_TEST_R2);

	arg = 2.0*PI*time*frequency;  
	xRef = 0.5*(qLow + qUpp) + 0.5*(qUpp-qLow)*Sin(arg);
	vRef = (qUpp-qLow)*(PI*frequency)*Cos(arg);

	mb_io_set_float(ID_CTRL_TEST_W0,xRef);
	mb_io_set_float(ID_CTRL_TEST_W1,vRef);

	//kp=7, kd=1, give good tracking for the ankles 
	kp = mb_io_get_float(ID_CTRL_TEST_R0); 
	kd = mb_io_get_float(ID_CTRL_TEST_R1);

	// Run a PD-controller on the outer foot angles:
	ctrlAnkOut.kp = kp;
	ctrlAnkOut.kd = kd;
	ctrlAnkOut.xRef = xRef;
	ctrlAnkOut.vRef = vRef;
	ctrlAnkOut.uRef = 0.0;
	controller_ankleOuter(&ctrlAnkOut);	

	// Run a PD-controller on the inner foot angles:
	ctrlAnkInn.kp = kp;
	ctrlAnkInn.kd = kd;
	ctrlAnkInn.xRef = xRef;
	ctrlAnkInn.vRef = vRef;
	ctrlAnkInn.uRef = 0.0;
	controller_ankleInner(&ctrlAnkInn);

	// Do nothing with the outer ankles or hip:

	mb_io_set_float(ID_MCFO_COMMAND_CURRENT, 0.0);
	mb_io_set_float(ID_MCFO_STIFFNESS, 0.0);
	mb_io_set_float(ID_MCFO_DAMPNESS, 0.0);

	mb_io_set_float(ID_MCH_COMMAND_CURRENT, 0.0);
	mb_io_set_float(ID_MCH_STIFFNESS, 0.0);
	mb_io_set_float(ID_MCH_DAMPNESS, 0.0);
}

/* Runs a simple test of the motor controllers, connecting the LabView parameter
 * to the set-points of the hip and ankle controllers.
 */
 void test_motor_control() {
 		struct ControllerData ctrlHip;
		struct ControllerData ctrlAnkOut;
		struct ControllerData ctrlAnkInn;

	// Run a PD-controller on the hip angle:
		ctrlHip.kp = mb_io_get_float(ID_CTRL_TEST_R0);
		ctrlHip.kd = mb_io_get_float(ID_CTRL_TEST_R1);
		ctrlHip.xRef = mb_io_get_float(ID_CTRL_TEST_R2);
		ctrlHip.vRef = mb_io_get_float(ID_CTRL_TEST_R3);
		ctrlHip.uRef = mb_io_get_float(ID_CTRL_TEST_R4);
		controller_hip(&ctrlHip);

	// Run a PD-controller on the outer foot angles:
		ctrlAnkOut.kp = mb_io_get_float(ID_CTRL_TEST_R5);
		ctrlAnkOut.kd = mb_io_get_float(ID_CTRL_TEST_R6);
		ctrlAnkOut.xRef = mb_io_get_float(ID_CTRL_TEST_R7);
		ctrlAnkOut.vRef = mb_io_get_float(ID_CTRL_TEST_R8);
		ctrlAnkOut.uRef = mb_io_get_float(ID_CTRL_TEST_R9);
		controller_ankleOuter(&ctrlAnkOut);

	// Run a PD-controller on the inner foot angles:
		ctrlAnkInn.kp = mb_io_get_float(ID_CTRL_TEST_R5);
		ctrlAnkInn.kd = mb_io_get_float(ID_CTRL_TEST_R6);
		ctrlAnkInn.xRef = mb_io_get_float(ID_CTRL_TEST_R7);
		ctrlAnkInn.vRef = mb_io_get_float(ID_CTRL_TEST_R8);
		ctrlAnkInn.uRef = mb_io_get_float(ID_CTRL_TEST_R9);
		controller_ankleInner(&ctrlAnkInn);
 }



/* Helps figure out sign convention for the motors. */
 void test_sign(void){
		struct ControllerData ctrlHip;
		struct ControllerData ctrlAnkOut;
		struct ControllerData ctrlAnkInn;

	// Run a PD-controller on the hip angle:
		ctrlHip.kp = 0;
		ctrlHip.kd = 0;
		ctrlHip.xRef = 0;
		ctrlHip.vRef = 0;
		ctrlHip.uRef = mb_io_get_float(ID_CTRL_TEST_R0);
		controller_hip(&ctrlHip);

	// Run a PD-controller on the outer foot angles:
		ctrlAnkOut.kp = 0;
		ctrlAnkOut.kd = 0;
		ctrlAnkOut.xRef = 0;
		ctrlAnkOut.vRef = 0;
		ctrlAnkOut.uRef = mb_io_get_float(ID_CTRL_TEST_R1);
		controller_ankleOuter(&ctrlAnkOut);

	// Run a PD-controller on the inner foot angles:
		ctrlAnkInn.kp = 0;
		ctrlAnkInn.kd = 0;
		ctrlAnkInn.xRef = 0;
		ctrlAnkInn.vRef = 0;
		ctrlAnkInn.uRef = mb_io_get_float(ID_CTRL_TEST_R2);
		controller_ankleInner(&ctrlAnkInn); 
 }

 static float param_joint_ankle_flip = 0.3;
 static float param_joint_ankle_push = 2.5;
 static float param_joint_ankle_hold = 1.662;
 static int counter = 0;  
 /* Makes the inner foot periodically tracks hold/push/flip. */
 void test_inner_foot(void) { 
		struct ControllerData ctrlAnkInn;

	// Set up the angle reference, so that inner foot simulates when it's walking		
		if(counter <= 2000){
			ctrlAnkInn.xRef = param_joint_ankle_hold;
		}else if (counter <= 4000){
			ctrlAnkInn.xRef = param_joint_ankle_push;
		}else if (counter <= 6000){
			ctrlAnkInn.xRef = param_joint_ankle_flip;
		}else{
			counter = 0;
		}		
		counter ++;

	// Run a PD-controller on the inner foot angles:
		ctrlAnkInn.kp = 3; 
 		ctrlAnkInn.kd = 0.5;
		ctrlAnkInn.vRef = 0;
		ctrlAnkInn.uRef = 0;
		controller_ankleInner(&ctrlAnkInn);
 }

 /* Runs a simple test of the frequency controllers */
 void test_freq_control(void) {
		struct ControllerData ctrlAnkOut;
		struct ControllerData ctrlAnkInn;		
	
	// Set up the angle reference, so that outer&inner foot trace a step-function
	// (swing periodically between 0.5 and 1.5 rad)  			
		if(counter <= 1000){
			ctrlAnkInn.xRef = 1.5;
			ctrlAnkOut.xRef = 1.5;
		}else if (counter <= 2000){
			ctrlAnkInn.xRef = 0.5;
			ctrlAnkOut.xRef = 0.5;
		}else{
			counter = 0;
		}		
		counter ++;
					
	// Run a PD-controller on the outer foot angles:
		ctrlAnkOut.kp = 3;
		ctrlAnkOut.kd = 0.5;
		ctrlAnkOut.vRef = 0;
		ctrlAnkOut.uRef = 0;
		controller_ankleOuter(&ctrlAnkOut);

	// Run a PD-controller on the inner foot angles:
		ctrlAnkInn.kp = 3;
		ctrlAnkInn.kd = 0.5;
		ctrlAnkInn.vRef = 0;
		ctrlAnkInn.uRef = 0;
		controller_ankleInner(&ctrlAnkInn);
		
 }

 /* Runs a simple test that makes the inner leg traces a Sin wave 
  *	while making both inner and outer feet stay flat
  */
 void test_trajectory(void) {
 		struct ControllerData ctrlHip;
		struct ControllerData ctrlAnkOut;
		struct ControllerData ctrlAnkInn;
		float in_angle;
		float torque;

	// Compute the position/slope/curvature of a 5th order polynomial at a given time 
		int row = sizeof(DATA)/sizeof(DATA[0][0])/4;
		float max_t = DATA[row-1][0];
		poly_coeff *COEFFS = data_to_coeff(DATA, row);
		
		float sys_t = mb_io_get_float(ID_TIMESTAMP)/1000;//converts the system_time from ms to s
		float phi;
		poly_coeff c;
		float y, yd; //, ydd;
		float t = Fmod(sys_t, max_t);
		int index = getIndex(t, COEFFS, row-1); //length of the the COEFFS array is row-1
		if(index == -1){
			//input time less than the time interval
			index = 0;
			c = COEFFS[index];
			phi = 0;
		}else if(index == -2){
			//input time greater than the time interval
			index = row-2; 
			c = COEFFS[index];
			phi = 1;
		}else{
			//found the time interval, evaluate the polynomial at time t
			c = COEFFS[index];
			phi = (t-c.t0)/(c.t1-c.t0);
		}
		
		y = getY(c, phi);
		yd = getYd(c, phi);
		//ydd = getYdd(c, phi);	
	
	// Calculate the toque needed to compensate for gravity pull
		in_angle = mb_io_get_float(ID_MCH_ANGLE);  // gets the hip angle (angle b/t inner&outer legs; pos when inner leg is in front)
		torque = leg_m * g * leg_r * Sin(in_angle); 

	// Run a PD-controller on the hip angle:
		ctrlHip.kp = 6;
		ctrlHip.kd = 2;
		
		ctrlHip.xRef = y;
		//mb_io_set_float(ID_CTRL_TEST_W0, ctrlHip.xRef);
		ctrlHip.vRef = yd;
		ctrlHip.uRef = torque;
		controller_hip(&ctrlHip);

	// Run a PD-controller on the outer foot angles: make the feet stay flat wrt the ground
		ctrlAnkOut.kp = 3;
		ctrlAnkOut.kd = 0.5;
		ctrlAnkOut.xRef = FO_flat_angle();
		ctrlAnkOut.vRef = 0.0;
		ctrlAnkOut.uRef = 0.0;
		controller_ankleOuter(&ctrlAnkOut);

	
	// Run a PD-controller on the inner foot angles: make the feet stay flat wrt the ground
		ctrlAnkInn.kp = 3;
		ctrlAnkInn.kd = 0.5;
		ctrlAnkInn.xRef = FI_flat_angle(); 	
		ctrlAnkInn.vRef = 0.0;
		ctrlAnkInn.uRef = 0.0;
		controller_ankleInner(&ctrlAnkInn);
 }

 /* A test that makes the inner leg track a Sin wave generated uSing Sin function from RangerMath. */ 
 void track_Sin(void){
 	 	struct ControllerData ctrlHip;
		float sys_t = mb_io_get_float(ID_TIMESTAMP)/1000;//converts the system_time from ms to s
		float t = Fmod(sys_t, 2*PI);

	// Run a PD-controller on the hip angle:
		ctrlHip.kp = 6;
		ctrlHip.kd = 2;
				
		if(t > PI){
		 	t = -2*PI + t;
		}
		ctrlHip.xRef = Sin(t)/4;
		mb_io_set_float(ID_CTRL_TEST_W0, ctrlHip.xRef);
		ctrlHip.vRef = Cos(t)/4;
		ctrlHip.uRef = 0.0;
		controller_hip(&ctrlHip);
 }


 /* Returns the relative angle that makes the outer foot stay flat wrt ground. */
 float FO_flat_angle(void){
  		float out_angle, FO_angle;
		out_angle = get_out_angle();  // gets the absolute angle (of the outer leg) wrt ground; pos when outer leg is forward  
		
		FO_angle = (PI/2 - out_angle + 0.2); //adds an offset of 0.15 rad to make the feet more stable 

		if(out_angle > 0){
			//outer leg is in back, add offset to outer ankle angle
			FO_angle += 0.4;  
		}

		return FO_angle;
 }


 /* Returns the relative angle that makes the inner foot stay flat wrt ground. */
 float FI_flat_angle(void){
 		float in_angle, out_angle, FI_angle;
		out_angle = get_out_angle();  // gets the absolute angle (of the outer leg) wrt ground; pos when outer leg is forward  
		in_angle = get_in_angle();

		FI_angle = (PI/2 + (in_angle - out_angle) + 0.2); //adds an offset of 0.15 rad	to make the fee more stable
		
		if(out_angle < 0){
			//inner leg is in back, add offset to inner ankle angle 
			FI_angle += 0.4; 
		}
		
		return FI_angle;	
 } 

