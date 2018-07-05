# Almondとは
豊田高専・仲野研究室で開発されている小型車輪部門用に出場するロボットに
搭載するBLDCモータードライバーのファームウェアの開発コードである。

# 機能
+ BLDCモーターの制御
    + BLDCドライバ`DRV8332`を制御することで動作する。
    + ホールセンサによる矩形制御
    + 電流センサによるベクトル制御 (ただし非実装)
+ UARTを介したモニタリング
    + 簡易的なShellが動作する。
    + 秋月のusb-serial変換基盤と接続できる
+ SPIを介したモーターの制御

# 実行環境
+ マイコン`dspic33fj32mc204`で動作する。

# 開発環境
+ MPLAB Xが動作し、usbポートが存在すること。
+ 書き込み機として`picket3`が存在すること。
+ モニタリングを行う際は5V~3.3Vロジックの`UART`接続が必要である。

# 製作者
[terurin](https://github.com/terurin)

# ライセンス
`license.md`を参照すること
