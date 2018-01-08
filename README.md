# gz_model


## Set (x,y,Yaw) by CSV

```
$ export GAZEBO_MASTER_URI=http://localhost:11346
$ ./gz --inputfile /opt/repositories/mvae_mapping/src/mvae_sensor_train/trajectories/positions.csv --model unit_box --z 0.5 --R 0 --P 0 --delay 200000
```

## Native gz tool

Look at the branch [gz-7.9.0](https://github.com/tik0/gz_model/tree/gz-7.9.0) to build the native tool
