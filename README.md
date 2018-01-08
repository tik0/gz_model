# gz_model

This is the nativ version of the gz tool forked from [gazebo-7.9.0](https://bitbucket.org/osrf/gazebo/src/3517e5b44f710b9997035c58d91f183e919fe7ff?at=gazebo7) (everthing tested under Ubuntu 16.04).
You might need to install `libgazebo7-dev`.
Unfortunately, necessary headers are missing in the dev package, so you need to check out gazebo and copy the headers manually:

```
sudo cp <gazebo repository location>/gazebo/gui/viewers/*.hh /usr/include/gazebo-7/gazebo/gui/viewers/
```

## Build

```
mkdir build
cmake ..
make -j4
```
