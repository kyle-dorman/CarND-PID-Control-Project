#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include "twiddle.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

// reset
void reset_simulator(uWS::WebSocket<uWS::SERVER>& ws)
{
  std::string msg("42[\"reset\", {}]");
  ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
}

int main()
{
  uWS::Hub h;

  bool USE_TWIDDLE = false;
  bool DEBUG = false;
  int PRINT_FREQUENCY = 1000;
  int iteration = 0;
  int last_reset = -1;
  int best_iterations_distance = 100;

  std::vector<double> coefficients = {2.08, 155.058, 0.0211956};
  PID pid;
  pid.Init(coefficients[0], coefficients[1], coefficients[2]);

  Twiddle twiddle;
  if (USE_TWIDDLE) {
    twiddle.Init(coefficients, 3000, 500, 0.1);  
  }

  h.onMessage([USE_TWIDDLE, DEBUG, PRINT_FREQUENCY, &iteration, &last_reset, &best_iterations_distance, &twiddle, &pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());

          if (USE_TWIDDLE && std::abs(cte) > 3.0) {
            if (iteration != last_reset) {
              std::cout << "-----------RESET-----------" << std::endl;
              last_reset = iteration;
              // maintainCoeffiecent for twiddle if this is better than a previous run
              bool maintainCoeffiecent = twiddle.Iteration() > best_iterations_distance;
              if (maintainCoeffiecent == false) {
                std::cout << "WORSE " << twiddle.Iteration() << " " << best_iterations_distance << std::endl;
              } else {
                std::cout << "BETTER " << twiddle.Iteration() << " " << best_iterations_distance << std::endl;
                best_iterations_distance = twiddle.Iteration();
              }
              twiddle.Next(maintainCoeffiecent);
              pid.ResetError();
              reset_simulator(ws);
            }
          } else {
            pid.UpdateError(cte);
            double steer_value = pid.SteeringAngle();

            if (USE_TWIDDLE) {
              twiddle.UpdateError(cte, angle);
              std::vector<double> coefficients = twiddle.Coefficients();
              double kp = coefficients.size() > 0 ? coefficients[0] : 0.0;
              double kd = coefficients.size() > 1 ? coefficients[1] : 0.0;
              double ki = coefficients.size() > 2 ? coefficients[2] : 0.0;
              pid.SetCoefficients(kp, kd, ki);
              if (twiddle.Iteration() > 2998) {
                best_iterations_distance = 2998;
              }
            }
            
            iteration++;
            if (DEBUG && iteration % PRINT_FREQUENCY == 0) {
              std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;  
            }
            
            json msgJson;
            msgJson["steering_angle"] = steer_value;
            msgJson["throttle"] = 0.6;
            auto msg = "42[\"steer\"," + msgJson.dump() + "]";
            if (DEBUG && iteration % PRINT_FREQUENCY == 0) {
              std::cout << msg << std::endl;
            }
            ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          }
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
