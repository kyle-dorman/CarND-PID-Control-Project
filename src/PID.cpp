#include "PID.h"

PID::PID() {}

PID::~PID() {}

void PID::Init(double _Kp, double _Kd, double _Ki) {
	SetCoefficients(_Kp, _Kd, _Ki);
  ResetError();
}

void PID::UpdateError(double cte) {
	d_error = cte - p_error;
	p_error = cte;
	i_error += cte;
}

double PID::SteeringAngle() {
	double steer_value = 0.0 - Kp * p_error - Kd * d_error - Ki * i_error;
  if (steer_value > 1.0) {
    steer_value = 1.0;
  } else if (steer_value < -1.0) {
    steer_value  = -1.0;
  }
  return steer_value;
}

void PID::SetCoefficients(double _Kp, double _Kd, double _Ki) {
	Kp = _Kp;
  Ki = _Ki;
  Kd = _Kd;
}

void PID::ResetError() {
  p_error = 0;
  i_error = 0;
  d_error = 0;
}

