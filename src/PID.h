#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double _Kp, double _Kd, double _Ki);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the steering angle from the error and coefficients
  */
  double SteeringAngle();

  /*
   * Reset the coefficents. Used during twiddling.
   */
  void SetCoefficients(double _Kp, double _Kd, double _Ki);
};

#endif /* PID_H */
