#pragma once

// const canvas size (window can be rescaled though)
//#define WINDOW_WIDTH  2560  //可以通过sf::VideoMode::getDesktopMode()获得桌面的分辨率，我的是2560x1600
//#define WINDOW_HEIGHT 1600
#define WINDOW_WIDTH  1600   // 1280 
#define WINDOW_HEIGHT 1200   // 960

#define MAP_PLAYER_BOARDER 50.f  // 在大地图行走，距离边缘不能超过这个距离
#define MAP_FONT_BOARDER 5.f
#define MAP_TEXT_DISTANCE 30

#define PLAYER_SPEED 300.f

#define FONT_SIZE_MIDDLE 36u
#define FONT_SIZE_LARGE  48u
