#include <Windows.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include "DxLib.h"

/***** main.cpp *****/


#define WIN_CENTER_X (640/2)
#define WIN_CENTER_Y (480/2)

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
    ChangeWindowMode(TRUE),    
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8), 
    DxLib_Init(), 
    SetDrawScreen( DX_SCREEN_BACK );
    int center_x = 0;
    int center_y = 0;
    double percent = 10;
    int gaugeImageHandle = LoadGraph("../../../../Downloads/");
    if(gaugeImageHandle == -1){
        std::cerr<<"画像の読み込みに失敗" << std::endl;
        return -1;
    }
  // 何かキーが押されるかウインドウが閉じられるまでループ
    while( !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() ){
        DrawCircleGauge(center_x,center_y,percent,gaugeImageHandle);
    }
    
    // ＤＸライブラリの後始末
    DxLib_End() ;

    // ソフトの終了
    return 0 ;
}     
