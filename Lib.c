#pragma systemFile

//All variables that are used during PID
typedef struct {
	float kp;
	float integral;
	float kd;
	float ki;
	float des;
	float error;
	float der;
	float threshold;
	int time;
	int motor_value;
	float i_threshold;
} pid_;

//Set the threshold of the target value to exit
void
pid_threshold(pid_ *this, int threshold) {
	this->threshold = threshold;
}

//Set the threshold that I will run within
void
pid_i_threshold(pid_ *this, int threshold) {
	this->i_threshold = threshold;
}

//Set kp, ki and kd
void
pid_init(pid_ *this, float p, float i, float d) {
	this->kp = p;
	this->ki = i;
	this->kd = d;
}

//Calculate PID
void
update_pid(pid_ *this, float current, float setpoint) {
	long dt = nSysTime-this->time;
	float error = setpoint-current;
	this->der = (dt==0) ? 0.0 : (error-this->error)/dt;

	if (abs(error) > this->i_threshold) {
		this->integral = 0;
	}
	else {
		this->integral += error*dt;
	}

	this->motor_value = (this->error*this->kp)+(this->integral*this->ki)+(this->der*this->kd);

	this->error = error;
	this->time = nSysTime;
}

//Hold the current function until the target position is reached
void
wait_for(pid_ *this) {
	while (true) {
		delay(20);
		if (abs(this->error) < this->threshold) {
			return;
		}
	}
}

void
wait_for(pid_ *this, pid_ *that) {
	while (true) {
		delay(20);
		if (abs(this->error) < this->threshold && abs(that->error) < that->threshold) {
			return;
		}
	}
}
