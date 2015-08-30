//
//CAN_ID Table for Cornell Ranger Robot
//Automatically generated by ranger_parser.m
//	 use the ranger_parser.m MATLAB script to make any changes to can_id.h, board_id.h, error_id.h, init_txlist.h, init_rxlist.h, io_data.c, error_messages.h or csr_routing_table.c
//	 the script is located in ...\SVN\Trunk\Ranger\Control\Ranger_Configuration
//

#ifndef __H_CAN_ID__
#define __H_CAN_ID__ 

typedef enum can_ids{
	ID_TIMESTAMP = 0,	 // 0 2 1 // Timestamp // time since turning on of the robot in milliseconds
	ID_ERROR_MB = 1,	 // 0 1 0 // 
	ID_ERROR_CSR = 2,	 // 0 2 1 // 
	ID_ERROR_MCH = 3,	 // 0 3 1 // 
	ID_ERROR_MCFO = 4,	 // 0 4 1 // 
	ID_ERROR_MCFI = 5,	 // 0 5 1 // 
	ID_ERROR_MCSO = 6,	 // 0 6 1 // 
	ID_ERROR_MCSI = 7,	 // 0 7 1 // 
	ID_ERROR_UI = 8,	 // 0 8 1 // 
	ID_MB_STATUS = 9,	 // 0 1 3;4;5;6;7;8;9 // 
	ID_MCH_MOTOR_VELOCITY = 10,	 // 0 3 1 // angular rate (rad/sec) of hip motor
	ID_MCH_MOTOR_CURRENT = 11,	 // 0 3 1 // hip motor current
	ID_MCH_MOTOR_POSITION = 12,	 // 0 3 1 // angle in radians of the hip motor
	ID_MCH_MOTOR_TARGET_CURRENT = 13,	 // 0 1 3 // 
	ID_MCH_ANGLE = 14,	 // 0 3 1 // angle in radians of the hip joint (difference between this and the hip motor angle is the gear back lash)
	ID_MCH_BATT_POWER = 15,	 // 0 3 1 // power consumed by the hip board. 
	ID_MCH_SHUTDOWN = 16,	 // 0 1 3 // 1 = turn off the hip motor
	ID_MCH_SLEEP = 17,	 // 0 1 3 // 
	ID_MCH_COMMAND_CURRENT = 18,	 // 0 1 3 // desired current to the hip motor = ID_MCH_COMMAND_CURRENT+ ID_MCH_STIFFNESS*hip_angle + ID_MCH_DAMPNESS*hip_angle_rate 
	ID_MCH_STIFFNESS = 19,	 // 0 1 3 // see above (ID_MCH_COMMAND_CURRENT)
	ID_MCH_DAMPNESS = 20,	 // 0 1 3 // see above (ID_MCH_COMMAND_CURRENT)
	ID_MCH_ANG_RATE = 21,	 // 0 3 1 // slightly filtered derivative of ID_MCH_ANGLE
	ID_MCH_STATUS = 22,	 // 0 3 1 // 
	ID_MCH_EXECUTION_TIME = 23,	 // 0 3 1 // time taken to execute all the functions in one row of the scheduler (software_setup.c in hip board) ( should be less than the tick time of the board; eg 0.5 ms for hip motor board)
	ID_MCH_MAX_EXECUTION_TIME = 24,	 // 0 3 1 // maximum recorded value of the ID_MCH_EXECUTION_TIME since the start of the robot
	ID_MCH_EMPTY_TX1 = 25,	 // 0 1 3 // 
	ID_MCH_EMPTY_TX2 = 26,	 // 0 1 3 // 
	ID_MCH_BATT_CURRENT = 27,	 // 0 3 1 // current in the hip board
	ID_MCH_BATT_VOLTAGE = 28,	 // 0 3 1 // 
	ID_MCH_HBRIDGE_TEMP = 29,	 // 0 3 1 // 
	ID_MCH_EMPTY_RX1 = 30,	 // 0 3 1 // 
	ID_MCH_EMPTY_RX2 = 31,	 // 0 3 1 // 
	ID_MCH_EMPTY_RX3 = 32,	 // 0 3 1 // 
	ID_MCFO_MOTOR_POSITION = 33,	 // 0 4 1 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_MOTOR_VELOCITY = 34,	 // 0 4 1 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_MOTOR_CURRENT = 35,	 // 0 4 1 // can_id for Outer Feet board-- check similar can_id; starting with MCH
	ID_MCFO_MOTOR_TARGET_CURRENT = 36,	 // 0 1 4 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_RIGHT_ANKLE_ANGLE = 37,	 // 0 4 1 // ankle joint angle in radians (zero when foot is turned all the way up)
	ID_MCFO_RIGHT_LS = 38,	 // 0 4 1 // 1 = right foot is turned all the way up
	ID_MCFO_LEFT_LS = 39,	 // 0 4 1 // 1 = left foot is turned all the way up
	ID_MCFO_RIGHT_HS = 40,	 // 0 4 1 // 1 = right foot is touching the ground; 0= foot in air
	ID_MCFO_LEFT_HS = 41,	 // 0 4 1 // 1 = left foot is touching the ground; 0= foot in air
	ID_MCFO_SHUTDOWN = 42,	 // 0 1 4 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_SLEEP = 43,	 // 0 1 4 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_BATT_POWER = 44,	 // 0 4 1 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_COMMAND_CURRENT = 45,	 // 0 1 4 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_STIFFNESS = 46,	 // 0 1 4 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_DAMPNESS = 47,	 // 0 1 4 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_STATUS = 48,	 // 0 4 1 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_LEFT_HEEL_SENSE = 49,	 // 0 4 1 // analog value from foot contact sensor (high when foot is touching the ground; low when foot is in air)
	ID_MCFO_RIGHT_HEEL_SENSE = 50,	 // 0 4 1 // analog value from foot contact sensor (high when foot is touching the ground; low when foot is in air) 
	ID_MCFO_EXECUTION_TIME = 51,	 // 0 4 1 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_MAX_EXECUTION_TIME = 52,	 // 0 4 1 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_HS_STATE = 53,	 // 0 1 4 // ??? HS means heel strike
	ID_MCFO_EMPTY_TX2 = 54,	 // 0 1 4 // 
	ID_MCFO_BATT_CURRENT = 55,	 // 0 4 1 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_BATT_VOLTAGE = 56,	 // 0 4 1 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_HBRIDGE_TEMP = 57,	 // 0 4 1 // can_id for Outer Feet board -- check similar can_id; starting with MCH
	ID_MCFO_OUTER_HS = 58,	 // 0 4 1 // ??? HS means heel strike
	ID_MCFO_RIGHT_ANKLE_RATE = 59,	 // 0 4 1 // slightly filtered derivative of the ID_MCFO_RIGHT_ANKLE_ANGLE
	ID_MCFO_EMPTY_RX3 = 60,	 // 0 4 1 // 
	ID_MCFI_MOTOR_VELOCITY = 61,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_MOTOR_CURRENT = 62,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_MOTOR_POSITION = 63,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_MOTOR_TARGET_CURRENT = 64,	 // 0 1 5 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_MID_ANKLE_ANGLE = 65,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_RIGHT_LS = 66,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_LEFT_LS = 67,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_RIGHT_HS = 68,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_LEFT_HS = 69,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_BATT_POWER = 70,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_SHUTDOWN = 71,	 // 0 1 5 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_SLEEP = 72,	 // 0 1 5 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_COMMAND_CURRENT = 73,	 // 0 1 5 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_STIFFNESS = 74,	 // 0 1 5 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_DAMPNESS = 75,	 // 0 1 5 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_STATUS = 76,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_LEFT_HEEL_SENSE = 77,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_RIGHT_HEEL_SENSE = 78,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_EXECUTION_TIME = 79,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_MAX_EXECUTION_TIME = 80,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_HS_STATE = 81,	 // 0 1 5 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_EMPTY_TX2 = 82,	 // 0 1 5 // 
	ID_MCFI_BATT_CURRENT = 83,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_BATT_VOLTAGE = 84,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_HBRIDGE_TEMP = 85,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCH
	ID_MCFI_INNER_HS = 86,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_ANKLE_RATE = 87,	 // 0 5 1 // can_id for Inner Feet board -- check similar can_id; starting with MCFO
	ID_MCFI_EMPTY_RX3 = 88,	 // 0 5 1 // 
	ID_COL_STATUS = 89,	 // 0 9 1 // IDs for the currently non existent color sensing; camera board tried on Ranger in 2010
	ID_COL_FRONT_WHITE = 90,	 // 0 9 1 // see above
	ID_COL_FRONT_RED = 91,	 // 0 9 1 // see above
	ID_COL_FRONT_GREEN = 92,	 // 0 9 1 // see above
	ID_COL_FRONT_BLUE = 93,	 // 0 9 1 // see above
	ID_COL_EXECUTION_TIME = 94,	 // 0 9 1 // see above
	ID_COL_MAX_EXECUTION_TIME = 95,	 // 0 9 1 // see above
	ID_ERROR_COL = 96,	 // 0 9 1 // see above
	ID_COL_EMPTY2 = 97,	 // 0 9 1 // see above
	ID_COL_EMPTY3 = 98,	 // 0 9 1 // see above
	ID_MCSO_LEFT_ANKLE_ANGLE = 99,	 // 0 6 1;4 // IDs for the currently non existent outer steering motor controller board tried on ranger before 2010
	ID_MCSO_LEFT_ANKLE_RATE = 100,	 // 0 6 1 // see above
	ID_MCSO_EMPTY_TX1 = 101,	 // 0 1 6 // see above
	ID_MCSO_EMPTY_TX2 = 102,	 // 0 1 6 // see above
	ID_MCSO_STATUS = 103,	 // 0 6 1 // see above
	ID_MCSO_COLOR_BACK_WHITE = 104,	 // 0 6 1 // see above
	ID_MCSO_COLOR_BACK_RED = 105,	 // 0 6 1 // see above
	ID_MCSO_COLOR_BACK_GREEN = 106,	 // 0 6 1 // see above
	ID_MCSO_COLOR_BACK_BLUE = 107,	 // 0 6 1 // see above
	ID_MCSO_EXECUTION_TIME = 108,	 // 0 6 1 // see above
	ID_MCSO_MAX_EXECUTION_TIME = 109,	 // 0 6 1 // see above
	ID_MCSI_MOTOR_CURRENT = 110,	 // 0 7 1 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_MOTOR_TARGET_CURRENT = 111,	 // 0 1 7 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_STEER_ANGLE = 112,	 // 0 7 1 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_BATT_POWER = 113,	 // 0 7 1 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_SHUTDOWN = 114,	 // 0 1 7 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_SLEEP = 115,	 // 0 1 7 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_COMMAND_ANG = 116,	 // 0 1 7 // steering angle commanded to the steering board
	ID_MCSI_PROP_COEFF = 117,	 // 0 1 7 // (march 2013) position coefficient in the PI controller currently used for low-level steering (in the Steering board)
	ID_MCSI_INT_COEFF = 118,	 // 0 1 7 // (march 2013) integral coefficient in the PI controller currently used for low-level steering (in the Steering board)
	ID_MCSI_STATUS = 119,	 // 0 7 1 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_EXECUTION_TIME = 120,	 // 0 7 1 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_MAX_EXECUTION_TIME = 121,	 // 0 7 1 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_EMPTY_TX1 = 122,	 // 0 1 7 // 
	ID_MCSI_EMPTY_TX2 = 123,	 // 0 1 7 // 
	ID_MCSI_BATT_CURRENT = 124,	 // 0 7 1 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_BATT_VOLTAGE = 125,	 // 0 7 1 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_HBRIDGE_TEMP = 126,	 // 0 7 1 // can_id for steering board -- check similar can_id; starting with MCH
	ID_MCSI_EMPTY_RX1 = 127,	 // 0 7 1 // 
	ID_MCSI_EMPTY_RX2 = 128,	 // 0 7 1 // 
	ID_MCSI_EMPTY_RX3 = 129,	 // 0 7 1 // 
	ID_UI_SET_LCD_QUAD_1 = 130,	 // 0 1 8 // data displayed in the 1st quadrant of the LCD display ????
	ID_UI_SET_LCD_QUAD_2 = 131,	 // 0 1 8 // 
	ID_UI_SET_LCD_QUAD_3 = 132,	 // 0 1 8 // 
	ID_UI_SET_LCD_QUAD_4 = 133,	 // 0 1 8 // 
	ID_UI_SET_LED_1 = 134,	 // 0 1 8 // ids for the LEDs in the UI board. 
	ID_UI_SET_LED_2 = 135,	 // 0 1 8 // 
	ID_UI_SET_LED_3 = 136,	 // 0 1 8 // 
	ID_UI_SET_LED_4 = 137,	 // 0 1 8 // 
	ID_UI_SET_LED_5 = 138,	 // 0 1 8 // 
	ID_UI_SET_LED_6 = 139,	 // 0 1 8 // 
	ID_UI_SET_BUZZER_FREQ = 140,	 // 0 1 8 // produces the sound! (used to play alma mater)
	ID_UI_SET_BUZZER_AMPL = 141,	 // 0 1 8 // produces the sound! (used to play alma mater)
	ID_UI_ROLL = 142,	 // 0 8 1 // (euler angle from imu) for-aft angle of the robot from gravity vector
	ID_UI_PITCH = 143,	 // 0 8 1 // (euler angle from imu)
	ID_UI_YAW = 144,	 // 0 8 1 // (euler angle from imu)
	ID_UI_ANG_RATE_X = 145,	 // 0 8 1 // angular velocity in rad/sec (for-aft) from imu  (see the axes marked on the imu)
	ID_UI_ANG_RATE_Y = 146,	 // 0 8 1 // imu angle rate (not exactly equal to thederivative of ID_UI_PITCH)
	ID_UI_ANG_RATE_Z = 147,	 // 0 8 1 // imu rangle rate
	ID_UI_BUTTONS = 148,	 // 0 8 1 // 
	ID_UI_RC_0 = 149,	 // 0 8 1 // most left i/o pin on the UI board (used for steering command from RC; right joystick)
	ID_UI_RC_1 = 150,	 // 0 8 1 // (used to receive signal from RC; left joystick)
	ID_UI_RC_2 = 151,	 // 0 8 1 // (used to receive signal from RC; signal coming from a knob on the right of RC transmitter; used to switch between the camera and the RC)
	ID_UI_RC_3 = 152,	 // 0 8 1 // most right i/o pin on the UI board; camera gives its output here. 
	ID_UI_EXECUTION_TIME = 153,	 // 0 8 1 // can_id for the UI board -- check similar can_id; starting with MCH
	ID_UI_MAX_EXECUTION_TIME = 154,	 // 0 8 1 // can_id for the UI board -- check similar can_id; starting with MCH
	ID_UI_AUDIO = 155,	 // 0 1 8 // 
	ID_UI_EMPTY_TX2 = 156,	 // 0 1 8 // 
	ID_UI_STATUS = 157,	 // 0 8 1 // 
	ID_UI_EMPTY_RX4 = 158,	 // 0 8 1 // 
	ID_UI_EMPTY_RX5 = 159,	 // 0 8 1 // 
	ID_CSR_CAN1_LOAD = 160,	 // 0 2 0 // 
	ID_CSR_CAN2_LOAD = 161,	 // 0 2 0 // 
	ID_CSR_CAN3_LOAD = 162,	 // 0 2 0 // 
	ID_CSR_CAN4_LOAD = 163,	 // 0 2 0 // 
	ID_CSR_MCU_POWER = 164,	 // 0 2 0 // 
	ID_CSR_EMPTY_TX1 = 165,	 // 0 1 2 // 
	ID_CSR_EMPTY_TX2 = 166,	 // 0 1 2 // 
	ID_CSR_MCU_VOLTAGE = 167,	 // 0 2 1 // 
	ID_CSR_MCU_CURRENT = 168,	 // 0 2 1 // 
	ID_CSR_EMPTY_RX4 = 169,	 // 0 2 1 // 
	ID_CSR_EMPTY_RX5 = 170,	 // 0 2 1 // 
	ID_MB_EXECUTION_TIME = 171,	 // 0 1 0 // Time required to finish the schedule
	ID_ERROR_LABVIEW = 172,	 // 0 1 0 // Buffered errors for transmission to LabView/PC
	ID_ERROR_LCD = 173,	 // 0 1 8 // Buffered errors for transmission to Ranger LCD
	ID_E_MCH_ANG_RATE = 174,	 // 0 1 0 // estimated rate of the hip (STATE = dqh)
	ID_E_MCFO_RIGHT_ANKLE_RATE  = 175,	 // 0 1 0 // estimated rate of the outer ankles (STATE=dq0)
	ID_E_MCFI_ANKLE_RATE = 176,	 // 0 1 0 // estimated rate of the inner ankles (STATE = dq1)
	ID_EST_CONTACT_OUTER = 177,	 // 0 1 0 // Filtered sum of both outer foot contact sensors
	ID_EST_CONTACT_INNER = 178,	 // 0 1 0 // Filtered sum of both inner foot contact sensors
	ID_EST_LAST_STEP_LENGTH = 179,	 // 0 1 0 // Stores the step length as computed once per step in double stance
	ID_EST_STATE_TH0 = 180,	 // 0 1 0 // abs angle outer legs
	ID_EST_STATE_TH1 = 181,	 // 0 1 0 // abs angle inner legs
	ID_EST_STATE_PHI0 = 182,	 // 0 1 0 // abs angle outer feet
	ID_EST_STATE_PHI1 = 183,	 // 0 1 0 // abs angle inner feet
	ID_EST_STATE_DTH0 = 184,	 // 0 1 0 // abs rate outer legs
	ID_EST_STATE_DTH1 = 185,	 // 0 1 0 // abs rate inner legs
	ID_EST_STATE_DPHI0 = 186,	 // 0 1 0 // abs rate outer feet
	ID_EST_STATE_DPHI1 = 187,	 // 0 1 0 // abs rate inner feet
	ID_EST_TEST_R0 = 188,	 // 0 0 1 // test read from labview
	ID_EST_TEST_R1 = 189,	 // 0 0 1 // test read from labview
	ID_EST_TEST_R2 = 190,	 // 0 0 1 // test read from labview
	ID_EST_TEST_R3 = 191,	 // 0 0 1 // test read from labview
	ID_EST_TEST_R4 = 192,	 // 0 0 1 // test read from labview
	ID_EST_TEST_R5 = 193,	 // 0 0 1 // test read from labview
	ID_EST_TEST_R6 = 194,	 // 0 0 1 // test read from labview
	ID_EST_TEST_R7 = 195,	 // 0 0 1 // test read from labview
	ID_EST_TEST_R8 = 196,	 // 0 0 1 // test read from labview
	ID_EST_TEST_R9 = 197,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W0 = 198,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W1 = 199,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W2 = 200,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W3 = 201,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W4 = 202,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W5 = 203,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W6 = 204,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W7 = 205,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W8 = 206,	 // 0 1 0 // test write to labview
	ID_EST_TEST_W9 = 207,	 //    // 
	ID_CTRL_HIP_KP = 208,	 // 16 0 1 // 
	ID_CTRL_HIP_KD = 209,	 // 3 0 1 // 
	ID_CTRL_ANK_FLIP_KP = 210,	 // 7 0 1 // 
	ID_CTRL_ANK_FLIP_KD = 211,	 // 1 0 1 // 
	ID_CTRL_ANK_PUSH_KP = 212,	 // 20 0 1 // 
	ID_CTRL_ANK_PUSH_KD = 213,	 // 3 0 1 // 
	ID_CTRL_ANK_HOLD_KP = 214,	 // 7 0 1 // 
	ID_CTRL_ANK_HOLD_KD = 215,	 // 1 0 1 // 
	ID_CTRL_HIP_SCISSOR_RATE = 216,	 // 1.3 0 1 // 
	ID_CTRL_HIP_SCISSOR_OFFSET = 217,	 // 0.1 0 1 // 
	ID_CTRL_HIP_REF_HOLD = 218,	 // 0.2 0 1 // 
	ID_CTRL_HIP_TRANS_ANGLE = 219,	 // 0.08 0 1 // 
	ID_CTRL_ANK_REF_PUSH = 220,	 // -0.9 0 1 // 
	ID_CTRL_HIP_GRAVITY_COMPENSATION = 221,	 // 1 0 1 // Enables (1) or disables (0) gravity compensation in hip controller
	ID_CTRL_HIP_SPRING_COMPENSATION = 222,	 // 1 0 1 // Enables (1) or disables (0) spring compensation in hip controller
	ID_CTRL_TEST_R0 = 223,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R1 = 224,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R2 = 225,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R3 = 226,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R4 = 227,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R5 = 228,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R6 = 229,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R7 = 230,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R8 = 231,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R9 = 232,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W0 = 233,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W1 = 234,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W2 = 235,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W3 = 236,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W4 = 237,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W5 = 238,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W6 = 239,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W7 = 240,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W8 = 241,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W9 = 242,	 //    // 
	ID_LV_START,	 //    // 
	ID_LV_CH_0,	 //    // 
	ID_LV_CH_1,	 //    // 
	ID_LV_CH_2,	 //    // 
	ID_LV_CH_3,	 //    // 
	ID_LV_CH_4,	 //    // 
	ID_LV_CH_5,	 //    // 
	ID_LV_CH_6,	 //    // 
	ID_LV_CH_7,	 //    // 
	ID_LV_CH_8,	 //    // 
	ID_LV_CH_9,	 //    // 
	ID_LV_CH_10,	 //    // 
	ID_LV_CH_11,	 //    // 
	ID_LV_CH_12,	 //    // 
	ID_LV_CH_13,	 //    // 
	ID_LV_CH_14,	 //    // 
	ID_LV_CH_15,	 //    // 
	ID_LV_CH_16,	 //    // 
	ID_LV_CH_17,	 //    // 
	ID_LV_CH_18,	 //    // 
	ID_LV_CH_19,	 //    // 
	ID_LV_CH_20,	 //    // 
	ID_LV_CH_21,	 //    // 
	ID_LV_CH_22,	 //    // 
	ID_LV_CH_23,	 //    // 
	ID_LV_CH_24,	 //    // 
	ID_LV_CH_25,	 //    // 
	ID_LV_CH_26,	 //    // 
	ID_LV_CH_27,	 //    // 
	ID_LV_CH_28,	 //    // 
	ID_LV_CH_29,	 //    // 
	ID_LV_CH_30,	 //    // 
	ID_LV_CH_31,	 //    // 
	ID_LV_CH_32,	 //    // 
	ID_LV_CH_33,	 //    // 
	ID_LV_CH_34,	 //    // 
	ID_LV_CH_35,	 //    // 
	ID_LV_CH_36,	 //    // 
	ID_LV_CH_37,	 //    // 
	ID_LV_CH_38,	 //    // 
	ID_LV_CH_39,	 //    // 
	ID_LV_CH_40,	 //    // 
	ID_LV_CH_41,	 //    // 
	ID_LV_CH_42,	 //    // 
	ID_LV_CH_43,	 //    // 
	ID_LV_CH_44,	 //    // 
	ID_LV_CH_45,	 //    // 
	ID_LV_CH_46,	 //    // 
	ID_LV_CH_47,	 //    // 
	ID_LV_VERSION,	 //    // 
	ID_LAST
} CAN_ID;


#endif

