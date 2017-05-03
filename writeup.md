## Writeup

### Describe the effect each of the P, I, D components had in your implementation.

P: Proportional gain. This parameter tunes how strongly the wheel turns relative to the current cross track error. During testing, if it seemed like the car wasn't turning sharp enough on a turn, I would increase the proportional gain. 

I: Integral gain. This parameter tunes for systematic bias (total error). i.e. the car's steady state is not perfectly at 0. In the simulator this wasn't a problem because the car was perfectly tuned. 

D: Differential gain. This param tunes for changes in gain between the most recent time step and the previous time step. During testing, if it seemed like the car was taking a long time to return to the cetner of the track because it was oscalating, I would increase this the differential gain. 

### Describe how the final hyperparameters were chosen.

I used a mixture of manual tuning and twiddle to pick the best parameters for the steering angle. Twiddle allowed me to test quickly as it jumped back and forth across the the testing space but some manual tuning was required because the car would jump off the track if the params got too crazy. Testing on a real car is obviously way more diffuclut because you have to worry about hitting things. Additionally the simulator allows you to change steering angles how ever you want but in the real world there is a maximum change allowed which would add another level of complexity. 