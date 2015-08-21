#include <mb_includes.h> 
#include <motorControl.h>
#include <fsm.h>
#include <RangerMath.h>

#define PI 3.141592653589793
#define SCISSOR_OFFSET 0.1
#define	SCISSOR_RATE 1.3

/* States for the FSM */
enum States {
	SET_UP,
	OUT_SWING,
	OUT_PUSH,
	INN_SWING,
	INN_PUSH,
	HOLD_DOUBLE,
	FLIGHT,
};

static enum States current_state = SET_UP; //keeps track of the current state the FSM is in 

/* Paramers used in the FSM */
float ANK_FLIP_KP; //medium high
float ANK_FLIP_KD;
float ANK_PUSH_KP; //high, more push off
float ANK_PUSH_KD;
float ANK_HOLD_KP; //low, moderately hold the feet, easier to push off
float ANK_HOLD_KD;
float HIP_KP;
float HIP_KD; 
float HIP_REF_HOLD; //relative reference angle for the hip 
float HIP_REF_TRANS_ANGLE;  
float ANK_REF_HOLD; //absolute refernce angles for the ankles
float ANK_REF_PUSH;
float ANK_REF_FLIP;

/* Read in values for parameters from Labview. */
void param_update(void){
	HIP_REF_HOLD = mb_io_get_float(ID_CTRL_HIP_REF_HOLD);
	HIP_REF_TRANS_ANGLE = mb_io_get_float(ID_CTRL_HIP_TRANS_ANGLE);
	ANK_REF_HOLD = mb_io_get_float(ID_CTRL_ANK_REF_HOLD); 
	ANK_REF_PUSH = mb_io_get_float(ID_CTRL_ANK_REF_PUSH);
	ANK_REF_FLIP = mb_io_get_float(ID_CTRL_ANK_REF_FLIP); 
}

static int flight_count = 0; //keeps track of the number of cycles the robot is in the air

/* Sets the initial state of the FSM. */
void fsm_init(void){
	current_state = SET_UP;
	flight_count = 0;
}

/* Runs the current state the FSM is in. 
 * Parameters:
 *		ID_CTRL_TEST_W1 = current_state;	
 */
void fsm_run(void){
	struct ControllerData ctrlHip;
	struct ControllerData ctrlAnkOut;
	struct ControllerData ctrlAnkInn;
	
	ANK_FLIP_KP = mb_io_get_float(ID_CTRL_ANK_FLIP_KP);
	ANK_FLIP_KD = mb_io_get_float(ID_CTRL_ANK_FLIP_KD);
	ANK_PUSH_KP = mb_io_get_float(ID_CTRL_ANK_PUSH_KP);
	ANK_PUSH_KD = mb_io_get_float(ID_CTRL_ANK_PUSH_KD);
	ANK_HOLD_KP =  mb_io_get_float(ID_CTRL_ANK_HOLD_KD);
	ANK_HOLD_KD =  mb_io_get_float(ID_CTRL_ANK_HOLD_KD);
	HIP_KP = mb_io_get_float(ID_CTRL_HIP_KP);
	HIP_KD= mb_io_get_float(ID_CTRL_HIP_KD); 


	angles_update();  //updates all angle parameters used in the code 
	fsm_update(); 	//updates FSM current state

	switch (current_state){
	case SET_UP: /*initial setup done in the air*/
		mb_io_set_float(ID_CTRL_TEST_W1, 0);
		// hold outer feet and flip up inner feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_FLIP, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);
		// hip tracks zero angle
		hip_track_rel(&ctrlHip, 0.0, 0.0, HIP_KP, HIP_KD);
		break;
	case OUT_SWING:	/*swing inner leg*/
		mb_io_set_float(ID_CTRL_TEST_W1, 1);
		// hold outer feet and flip up inner feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_FLIP, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);
		// adjust hip angle, outer on ground
		hip_scissor_track_outer(&ctrlHip, SCISSOR_OFFSET, SCISSOR_RATE,HIP_KP, HIP_KD);
		break;
	case OUT_PUSH:	/*push off outer feet and land inner feet*/					   
		mb_io_set_float(ID_CTRL_TEST_W1, 2);
		// push down outer feet and hold inner feet
	    out_ank_track_abs(&ctrlAnkOut, ANK_REF_PUSH, 0.0, 0.0, ANK_PUSH_KP, ANK_PUSH_KD);
	 	inn_ank_track_abs(&ctrlAnkInn, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
	    hip_track_rel(&ctrlHip, HIP_REF_HOLD, 0.0, HIP_KP, HIP_KD);
		break;
	case INN_SWING:	/*swing outer leg*/
		mb_io_set_float(ID_CTRL_TEST_W1, 3);
		// flip up outer feet and hold inner feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_FLIP, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		hip_scissor_track_inner(&ctrlHip, SCISSOR_OFFSET, SCISSOR_RATE, HIP_KP, HIP_KD);
		break;
	case INN_PUSH:	/*push off inner feet and land outer feet*/
		mb_io_set_float(ID_CTRL_TEST_W1, 4);
		//push down inner feet and hold outer feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_PUSH, 0.0, 0.0, ANK_PUSH_KP, ANK_PUSH_KD);
		hip_track_rel(&ctrlHip, -HIP_REF_HOLD, 0.0, HIP_KP, HIP_KD);
		break;
	case HOLD_DOUBLE: /*double stance*/
		mb_io_set_float(ID_CTRL_TEST_W1, 5);
		// hold outer feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		// hold inner feet
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		// adjust hip angle
		if(th0>0){
			hip_track_rel(&ctrlHip, -HIP_REF_HOLD, 0.0, HIP_KP, HIP_KD); //outer leg is in front
		}else{
			hip_track_rel(&ctrlHip, HIP_REF_HOLD, 0.0, HIP_KP, HIP_KD);	//inner leg is in front
		}
		break;
	case FLIGHT: /*flight mode*/ 
		mb_io_set_float(ID_CTRL_TEST_W1, 6);
		disable_motors();
		break;	
	default: /*state doesn't exist*/
		break;
	}

	// run the PD controllers if current state is not the flight state 
	if(current_state != FLIGHT){
		controller_hip(&ctrlHip);
		controller_ankleInner(&ctrlAnkInn);
		controller_ankleOuter(&ctrlAnkOut);
	}
}


/* Updates current state of the FSM. */
void fsm_update(void){
 	// Enters the flight state if all feet are off ground for 200 continuous cycles 
	if(current_state != SET_UP){ //not in the setup state
		if(!FI_on_ground() && !FO_on_ground()){
			flight_count++;
		}else{
			flight_count = 0;
		}
		if(flight_count >= 200 ){
			current_state = FLIGHT;
		}
	}

	switch(current_state){
	case SET_UP:
		// ready to walk when placed on ground					
		if(FO_on_ground()){
			current_state = OUT_SWING;	
		}
		break;
	case OUT_SWING:	/*swing inner leg*/
		if(th0<-HIP_REF_TRANS_ANGLE){ //inner leg in front, outer leg in the back 
			current_state = OUT_PUSH;
		} 		
		/*if( th0<-trans_angle && dth0<0 ){ //falling forward, push off
			current_state = OUT_PUSH;	
		}else if( th0>trans_angle && dth0>0 ){ //falling backward, emergency!
			current_state = HOLD_DOUBLE;
		}*/
		break;
	case OUT_PUSH: /*land inner leg*/
		if(FI_on_ground()){ //inner feet have landed 
		 	correct_gyro_angle(); //all feet are on ground
			current_state = INN_SWING;
		}
		break;
	case INN_SWING: /*swing outer leg*/
		if(th0>HIP_REF_TRANS_ANGLE){ //outer leg in front, inner leg in the back 
			current_state = INN_PUSH;	
		}
		/*if( th1<-trans_angle && dth1<0 ){ //falling forward, push off
			current_state = INN_PUSH;	
		}else if( th1>trans_angle && dth1>0 ){ //falling backward, emergency!
			current_state = HOLD_DOUBLE;
		} */
		break;
	case INN_PUSH: /*land outer leg*/
		if(FO_on_ground()){ //outer feet have landed
			correct_gyro_angle(); 
			current_state = OUT_SWING;
		}
		break;
	case HOLD_DOUBLE: /*double stance*/ 
		current_state = HOLD_DOUBLE; //absorbing state, no exit transition
		break;
	case FLIGHT:
		current_state = FLIGHT;
		break;
	default: /*state doesn't exist*/
		break;
	}
}			   

/*******************************************************/
/***************FSM TEST FUNCTIONS**********************/
/*******************************************************/

enum testStates {
	zero,
	one,
	two,
	three,
	four,
	five,
};

static enum testStates test_state = zero; 
int setup = 1;

/* Initializes the test FSM. */
void test_init(void){
	test_state = zero;
	setup = 1;
	flight_count = 0;
}

/* Makes Ranger walk with simpler transition conditions.
 * Parameters:
 *	- ID_CTRL_TEST_W1 = current state of the FSM		
 */
void test_fsm(void){
	struct ControllerData ctrlHip;
	struct ControllerData ctrlAnkOut;
	struct ControllerData ctrlAnkInn;
		
/*	KP&KD values that work:
  	hip: kp=27 kd=3.8 or kp=16 kd=3
	ank: push_kp=20 push_kd=3; kp=7 kd=1
*/

	ANK_FLIP_KP = mb_io_get_float(ID_CTRL_ANK_FLIP_KP);	 //medium high
	ANK_FLIP_KD = mb_io_get_float(ID_CTRL_ANK_FLIP_KD);
	ANK_PUSH_KP = mb_io_get_float(ID_CTRL_ANK_PUSH_KP);	 //high, more push off
	ANK_PUSH_KD = mb_io_get_float(ID_CTRL_ANK_PUSH_KD);
	ANK_HOLD_KP =  mb_io_get_float(ID_CTRL_ANK_HOLD_KP); //low, moderately hold the feet, easier to push off
	ANK_HOLD_KD =  mb_io_get_float(ID_CTRL_ANK_HOLD_KD);
	HIP_KP = mb_io_get_float(ID_CTRL_HIP_KP);
	HIP_KD= mb_io_get_float(ID_CTRL_HIP_KD);

	angles_update();

	// Enters flight state if all feet are off ground 
	if(!setup){	//not in the setup state
		if(!FI_on_ground() && !FO_on_ground()){
			flight_count++;
		}else{
			flight_count = 0;
		}
		if(flight_count >= 200 ){
			test_state = five;
		}
	}
  
	switch(test_state){
	case zero: //initial setup done in the air 
		mb_io_set_float(ID_CTRL_TEST_W1, 0);
		// hold outer feet and flip up inner feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_FLIP, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);
		// hip tracks zero angle
		hip_track_rel(&ctrlHip, 0.0, 0.0, HIP_KP, HIP_KD);
		 
		// ready to walk when placed on ground					
		if(FO_on_ground()){
			test_state = one;
			setup = 0;		
		}
		break;		
	case one:  //swing innner leg 
		mb_io_set_float(ID_CTRL_TEST_W1, 1);
		// hold outer feet and flip up inner feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_FLIP, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);
		hip_scissor_track_outer(&ctrlHip, SCISSOR_OFFSET, SCISSOR_RATE, HIP_KP, HIP_KD); 

		if(th0<-HIP_REF_TRANS_ANGLE){ //inner leg in front, outer leg in the back 
			test_state = two;
		} 
		break;
	case two: //push off outer feet 
		mb_io_set_float(ID_CTRL_TEST_W1, 2);
		// push down outer feet and hold inner feet
	    out_ank_track_abs(&ctrlAnkOut, ANK_REF_PUSH, 0.0, 0.0, ANK_PUSH_KP, ANK_PUSH_KD);
	 	inn_ank_track_abs(&ctrlAnkInn, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
	    hip_track_rel(&ctrlHip, HIP_REF_HOLD, 0.0, HIP_KP, HIP_KD);
		
		if(FI_on_ground()){ 
			correct_gyro_angle();
			test_state = three;
		}		
		break;
	case three: //swing outer leg
		mb_io_set_float(ID_CTRL_TEST_W1, 3);
		// flip up outer feet and hold inner feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_FLIP, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		hip_scissor_track_inner(&ctrlHip, SCISSOR_OFFSET, SCISSOR_RATE, HIP_KP, HIP_KD);
		
		if(th0>HIP_REF_TRANS_ANGLE){ //outer leg in front, inner leg in the back 
			test_state = four;	
		}
		break;
	case four: //push off inner feet
		mb_io_set_float(ID_CTRL_TEST_W1, 4);
		//push down inner feet and hold outer feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_PUSH, 0.0, 0.0, ANK_PUSH_KP, ANK_PUSH_KD);
		hip_track_rel(&ctrlHip, -HIP_REF_HOLD, 0.0, HIP_KP, HIP_KD);

		if(FO_on_ground()){ 
			correct_gyro_angle(); 
			test_state = one;
		}	
		break;
	case five: //flight mode
	 	mb_io_set_float(ID_CTRL_TEST_W1, 5);
		disable_motors();
		break;
	}

	if(test_state != five){
		controller_hip(&ctrlHip);
		controller_ankleInner(&ctrlAnkInn);
		controller_ankleOuter(&ctrlAnkOut);
	}
}

/* Corrects the gyro angle (integrated from gyro rate) every step Ranger takes
 * using the gyro angle (calculated from geometry). 
 * Sets the corrected gyro angle in the estimator code. 
 * Requires: 
 *		Both feet on ground & the function angles_update() called 
 */
void correct_gyro_angle(void){
	float l = 0.96;  // robot leg length
	float d = 0.14;  // robot foot joint eccentricity
	float Phi = 1.8;  // ankle joint orientation constant

	float Slope = 0.0;  // Ground slope (assume linear)
	float x, y, stepLength, qr_geo, qr_int, qr_new;

	/* Ranger geometry:
	 * [x;y] = vector from outer foot virtual center to the inner foot
	 * virtual center, in a frame that is rotated such that qr = 0
	 * These functions were determined using computer math. The code can
	 * be found in:
	 * templates/Estimator/legAngleEstimator/Derive_Eqns.m
	 */
	x = l*Sin(qh) - d*Sin(Phi - q1 + qh) + d*Sin(Phi - q0);
	y = l + d*Cos(Phi - q1 + qh) - l*Cos(qh) - d*Cos(Phi - q0);

	stepLength = Sqrt(x*x + y*y);
	qr_geo = Atan(y/x) + Slope;	 //gyro angle calculated from geometry 
	qr_int = get_prev_gyro_angle();	//gyro angle integrated from gyro rate (from the preivous ms)
	qr_new = 0.95*qr_int + 0.05*qr_geo;	//new gyro angle that's a weighted average of the two above
										//NOTE: ratio of 0.9:0.1 (used in Anoop's code) makes Ranger fall forward 

	set_gyro_angle(qr_new);	//updates the gyro angle in the estimator code
	
	return;
}


/* Simulates movement of ankles when Ranger walks. 
 * Parameters:
 *	- ID_CTRL_TEST_W1 = current state of the FSM		
 */
void test_fsm_ank(void){
	struct ControllerData ctrlAnkOut;
	struct ControllerData ctrlAnkInn;
	float trans_angle = 0.08;

	ANK_FLIP_KP = mb_io_get_float(ID_CTRL_ANK_FLIP_KP);
	ANK_FLIP_KD = mb_io_get_float(ID_CTRL_ANK_FLIP_KD);
	ANK_PUSH_KP = mb_io_get_float(ID_CTRL_ANK_PUSH_KP);
	ANK_PUSH_KD = mb_io_get_float(ID_CTRL_ANK_PUSH_KD);
	ANK_HOLD_KP =  mb_io_get_float(ID_CTRL_ANK_HOLD_KP);
	ANK_HOLD_KD =  mb_io_get_float(ID_CTRL_ANK_HOLD_KD);
	HIP_KP = mb_io_get_float(ID_CTRL_HIP_KP);
	HIP_KD= mb_io_get_float(ID_CTRL_HIP_KD);

	angles_update();

	switch(test_state){
	case one: //OUT_SWING
		mb_io_set_float(ID_CTRL_TEST_W1, 1);
		// hold outer feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		// flip up inner feet
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_FLIP, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);

		if( th0<-trans_angle){ //inner leg in front, outer leg in the back 
			test_state = two;	
		}
		break;
	case two: //OUT_PUSH		
		mb_io_set_float(ID_CTRL_TEST_W1, 2);		
		// push down outer feet
	    out_ank_track_abs(&ctrlAnkOut, ANK_REF_PUSH, 0.0, 0.0, ANK_PUSH_KP, ANK_PUSH_KD);
		// hold inner feet
	 	inn_ank_track_abs(&ctrlAnkInn, ANK_REF_HOLD, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);
		if(q0 > 2.2){ //inner feet have landed 
		 	test_state = three;
		}
		break;
	case three: //INN_SWING
		mb_io_set_float(ID_CTRL_TEST_W1, 3);		
		// flip up outer feet 
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_FLIP, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);
		// hold inner feet
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_HOLD, 0.0, 0.0, ANK_HOLD_KP, ANK_HOLD_KD);
		if(th0>trans_angle){ //inner leg in back, outer leg in front
			test_state = four;
		}
		break; 
	case four: //INN_PUSH
		mb_io_set_float(ID_CTRL_TEST_W1, 4);
		// hold outer feet
		out_ank_track_abs(&ctrlAnkOut, ANK_REF_HOLD, 0.0, 0.0, ANK_FLIP_KP, ANK_FLIP_KD);
		// push down inner feet
		inn_ank_track_abs(&ctrlAnkInn, ANK_REF_PUSH, 0.0, 0.0, ANK_PUSH_KP, ANK_PUSH_KD);
		if(q1 > 2.2){
			test_state = one;
		}
		break;
	}

	controller_ankleInner(&ctrlAnkInn);
	controller_ankleOuter(&ctrlAnkOut);
}

