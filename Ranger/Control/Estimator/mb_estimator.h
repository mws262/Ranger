#ifndef __MB_ESTIMATOR_H__
#define __MB_ESTIMATOR_H__

static struct FilterCoeff FC;
static struct FilterData FD_hip_rate, FD_hip_motor_rate;
static int f_init = 0; 

struct FilterCoeff {
	float a1;
	float a2;
	float b0;
	float b1;
	float b2;
};

struct FilterData {
	float z0; // Measurement at time k
	float z1; // Measurement at time k-1
	float z2; // Measurement at time k-2
	float y0; // Estimate at time k
	float y1; // Estimate at time k-1
	float y2; // Estimate at time k-2
	unsigned long t0; // time k
	unsigned long t1; // time k-1
	unsigned long t2; // time k-2
};

void mb_estimator_update(void);
void filter_init(void);
void filter_hip_rate(void);
void filter_hip_motor_rate(void) ;
void setFilterCoeff(struct FilterCoeff*, float);
void setFilterData(struct FilterData*, float) ;
float runFilter_new(struct FilterCoeff*, struct FilterData*, float, unsigned long);

#endif  // __MB_ESTIMATOR_H__

