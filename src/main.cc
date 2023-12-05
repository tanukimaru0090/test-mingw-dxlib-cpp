#include "DxLib.h"
#include <Windows.h>
#include <array>
#include <fstream>
#include <string>
#include <vector>
class Key {
public:
  Key() {}
  void update() {
    char temp[256] = {0};
    GetHitKeyStateAll(temp);
    for (int i = 0; i < 256; i++) {
      if (temp[i] != 0) {
        if (cntRelease[i] > 0) {
          cntRelease[i] = 0; // キーが押されたときに離されカウンタをゼロにする
        }
        cntPress[i]++;
      } else {
        if (cntPress[i] > 0) {
          cntRelease
              [i]++; // キーが離されたときに離されカウンタをインクリメントする
        }
        cntPress[i] = 0;
      }
    }
  }
  bool isAvailableCode(int keyCode) {
    if (!(0 <= keyCode && keyCode < 256)) {
      return false;
    }
    return true;
  }
  int isKeyPressd(int keyCode) {
    if (!isAvailableCode(keyCode)) {
      return -1;
    }
    return cntPress[keyCode];
  }
  int isKeyReleased(int keyCode) {
    if (!isAvailableCode(keyCode)) {
      return -1;
    }
    return cntRelease[keyCode];
  }

private:
  std::array<int, 256> cntPress;
  std::array<int, 256> cntRelease;
};

struct ReplayPosData {
  int x, y;
  float angle;
};
const std::string &replayPath = "assets/replay.dat";
const int WINDOW_WIDITH = 640;
const int WINDOW_HEIGHT = 480;
const int SQUARESiIE_X = 100;
const int SQUARESIZE_Y = 100;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

  int replayIndex = 0;
  int squareX = (WINDOW_WIDITH - SQUARESiIE_X) / 2;
  int squareY = (WINDOW_HEIGHT - SQUARESIZE_Y) / 2;
  bool isReplaying = false;
  std::vector<ReplayPosData> replayData;
  SetWindowSize(WINDOW_WIDITH, WINDOW_HEIGHT);
  ChangeWindowMode(TRUE);
  SetDrawScreen(DX_SCREEN_BACK);
  if (DxLib_Init() == -1) // ＤＸライブラリ初期化処理
  {
    return -1; // エラーが起きたら直ちに終了
  }
  Key key;
  while (ProcessMessage() == 0) {
    key.update();
    if (key.isKeyPressd(KEY_INPUT_R) == TRUE) {
      // リプレイのデータをファイルに書き込む
      std::ofstream ofs(replayPath, std::ios::binary);
      if (ofs) {
        size_t size = replayData.size();
        ofs.write(reinterpret_cast<char *>(&size), sizeof(int));
        // リプレイの各状態を書き込む
        for (const auto &state : replayData) {
          ofs.write(reinterpret_cast<const char *>(&state),
                    sizeof(ReplayPosData));
        }

        // ファイルを閉じる
        ofs.close();
      }
    } else if (key.isKeyPressd(KEY_INPUT_P) == TRUE) {

      // リプレイのデータをファイルから読み込む
      std::ifstream ifs(replayPath, std::ios::binary);
      if (ifs) {
        // リプレイのサイズを読み込む
        int size = 0;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(int));
        // リプレイのベクターをクリアしてリサイズする
        replayData.clear();
        replayData.resize(size);
        // リプレイの各状態を読み込む
        for (auto &state : replayData) {
          ifs.read(reinterpret_cast<char *>(&state), sizeof(ReplayPosData));
        }

        // ファイルを閉じる
        ifs.close();

        // リプレイの再生を開始する
        isReplaying = true;
        replayIndex = 0;
      }
    }
    if (!isReplaying) {
      if (CheckHitKey(KEY_INPUT_UP) == TRUE) {
        squareY -= 10;
      } else if (CheckHitKey(KEY_INPUT_DOWN) == TRUE) {
        squareY += 10;
      } else if (CheckHitKey(KEY_INPUT_LEFT) == TRUE) {
        squareX -= 10;
      }else if (CheckHitKey(KEY_INPUT_RIGHT) == TRUE) {
        squareX += 10;
      }
      ReplayPosData r;
      r.x = squareX;
      r.y = squareY;
      r.angle = 0;
      replayData.push_back(r);
    } else {

      if (replayIndex >= 0 && replayIndex < replayData.size()) {
        ReplayPosData state = replayData[replayIndex];
        squareX = state.x;
        squareY = state.y;
        replayIndex++;
      }
    }
    ClearDrawScreen();
    DrawFormatString(0, 50, GetColor(255, 255, 255), "replay:%d", isReplaying);
    DrawBox(squareX, squareY, squareX + SQUARESiIE_X, squareY + SQUARESIZE_Y,
            GetColor(255, 255, 255), TRUE);
    ScreenFlip();
  }
  DxLib_End(); // ＤＸライブラリ使用の終了処理

  return 0; // ソフトの終了
}