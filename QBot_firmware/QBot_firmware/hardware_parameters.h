#pragma once
//------------------ general ------------------
static constexpr int steps_per_rot = 200;
static constexpr int gripper_speed = 200;
static constexpr int slider_speed = 250;
static constexpr int max_angle = 418;

static constexpr int baudrate = 115200;

//----------------- gripper F -----------------
static constexpr int gripperF_step = 10;
static constexpr int gripperF_dir = 11;
static constexpr int gripperF_en = 22;

//----------------- gripper B -----------------
static constexpr int gripperB_step = 0;
static constexpr int gripperB_dir = 0;
static constexpr int gripperB_en = 0;

//----------------- gripper L -----------------
static constexpr int gripperL_step = 0;
static constexpr int gripperL_dir = 0;
static constexpr int gripperL_en = 0;

//----------------- gripper R -----------------
static constexpr int gripperR_step = 12;
static constexpr int gripperR_dir = 13;
static constexpr int gripperR_en = 9;

//----------------- slider y -----------------
static constexpr int sliderY_step = 0;
static constexpr int sliderY_dir = 0;
static constexpr int sliderY_en = 0;

//----------------- slider x -----------------
static constexpr int sliderX_step = 10;
static constexpr int sliderX_dir = 11;
static constexpr int sliderX_en = 0;




