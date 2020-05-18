#pragma once


#define DEBUG true

//#define WINDOW_WIDTH  2560  //可以通过sf::VideoMode::getDesktopMode()获得桌面的分辨率，我的是2560x1600
//#define WINDOW_HEIGHT 1600
#define WINDOW_WIDTH  1600          // 1280 
#define WINDOW_HEIGHT 1200          // 960
#define FPS 60.f                    // clw note：这里只是update的频率，draw是实时的
#define STATISTICS_UPDATE_TIME 0.1f  // 地图上的fps、坐标位置等调试信息的更新周期

#define MAP_PLAYER_BOARDER 50.f     // 在大地图行走，距离边缘不能超过这个距离
#define MAP_FONT_BOARDER 10.f
#define MAP_TEXT_DISTANCE 30

#define FRAME_DURATION 0.25          // 玩家行走时，一帧持续的时间
#define PLAYER_SPEED 300.f

#define FONT_SIZE_MIDDLE 36u
#define FONT_SIZE_LARGE  48u
