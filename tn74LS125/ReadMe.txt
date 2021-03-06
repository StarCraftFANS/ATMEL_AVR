﻿
                       キーボードマニア・ Part III
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~

■ テストルーチンの動作概要

  PS2 keyboard の文字入力に反応して、音階を発生します。
  出力は、 PORTB.2 にオーディオ帯域の発振音を出します。

         ATtiny2313
         ___    ___
RESET    [1  |__| 20] Vcc
PD0      [2       19] PB7(SCK)
PD1      [3       18] PB6(MISO)
XTAL2    [4       17] PB5(MOSI)
XTAL1    [5       16] PB4(RST)
PD2      [6       15] PB3
PD3      [7       14] PB2(AudioOut) ------------> Audio OutPut
PD4      [8       13] PB1(KBDATA)   <------------ PS2 KeyBoard DATA
PD5      [9       12] PB0(KBCLK)    <------------ PS2 KeyBoard CLK
GND      [10      11] PD6(NC)
         ~~~~~~~~~~~

   ---------------------------------------
   XTAL:    XTAL1,2 => Crystal 12MHz
   PB2:     Audio Output
   PB1:     PS2 Keyboard DATA
   PB0:     PS2 Keyboard CLK
   ---------------------------------------

;PS/2 KeyBoard connector will be one of the following:
;        6pin MINIDIN
;        ------------
;         
;             _
;          (5) (6)        1 DATA
;             |           2 
;        (3)     (4)      3 GND
;                         4 +5V
;          (1) (2)        5 CLOCK
;                         6
;
; これはPS/2 KeyBoard側のMiniDINコネクタのピンを俯瞰した図です.
; 鏡像対称なので、GND と +5V の接続ミスには充分ご注意ください.
; DATA / CLK は今のところ直結で動作していますが、製品によっては
; AVR側にPullUp抵抗が必要なものが存在するかもしれません。
; （AVR内部でのPullUp処理は今のところ入れていません）


   PB2出力は適当なＲＣフィルタを通して高調波(12MHz/256)を
   濾過したのちに電解コンデンサ4.7uF程度でデカップリングして
   必要なら抵抗比で分圧してレベルをLineOutに落とした後
   オーディオアンプ等に接続します。

   一例）
                   4.7uF 
                   +  -
   PB2 --1kΩ---・---||-----1kΩ---・------ Line Out
                |                 |
        0.01uF ===              330Ω
                |                 |
   GND ---------・-----------------・------ GND



■ FUSE設定
ヒューズの設定は，外付けの 12MHz のクリスタルに合わせ、以下のように設定します。

Low: 11111111 (0xFF)
     ||||++++-- CKSEL[3:0] システムクロック選択
     ||++-- SUT[1:0] 起動時間
     |+-- CKOUT (0:PD2にシステムクロックを出力)
     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)

High:11-11011 (0xDB)
     |||||||+-- RSTDISBL (RESETピン 1:有効, 0:無効(PA2))
     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
     |||+-- WDTON (WDT 0:常時ON, 1:通常)
     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
     |+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
     +-- DWEN (On-Chipデバッグ 1:無効, 0:有効)

Ext: -------1 (0xFF)
            +-- SPMEN (SPM命令 1:無効, 0:有効)



■ ソースファイルの説明.

main.c    : メイン処理.
task.c    : 簡易タスク.
timer.c   : タイマー１.
reboot.c  : 再起動.
ps2keyb.c : PS2キーボード.
sound.c   : サウンド.
tonemain.c: 鍵盤メイン処理.


■ ヘッダファイルの説明.

config.h  : ポート設定関連の定義.
taskdef.h : 簡易タスクのアプリ側ID定義.

task.h    : 簡易タスク.
timer.h   : タイマー１.
ps2keyb.h : PS2キーボード.
keycode.h : SCAN CODE->ASCII CODE変換.
sinewave.h: sine波形テーブル
sound.h   : サウンド.
keyname.h : キーコード名称定義.
tonecode.h: 鍵盤テーブル.


■ taskの説明

taskは、単機能のスレッド管理です。以下の特徴があります。

	・スケジュールはラウンドロビンのみ。
	・ノン・プリエンプティブ。
	・あらかじめtask_idと、その実行関数を定義（自分でmapping）しておくこと。(taskdef.h)
	・割り込みルーチンとは基本的に無関係.(フォアグラウンド動作)。
	・タスク毎のコンテキストは、関数ポインタだけである。
	  ・レジスタコンテキストなどは一切持たない。
	  ・関数ポインタを自他により切り替える行為はＯＫ。
	  ・スタックエリアは全タスクに対して１個であり共有。
	・タスクをまわすためには dispatch_task() の実装部を呼び出す。
	・スリープ以外に、たいしたサービス処理は無い。
	・タスクキューやリスト構造を持たないので、あまり多い数のタスクをこなせない。
	  想定数＝せいぜい１６個以内。

レジスタコンテクストを持たないため、関数の途中で処理中断することが出来ません。
したがって、全てのタスク（登録関数）は、一定時間内に処理を終えてreturnする必要があります。

     ---＊---

メインルーチンは、現状では dispatch_task() を呼び出す無限ループの実装になっていますが
これを普通の複雑な手続きに変更して、I/O待ちあるいは各部のサブループにdispatch_task()
を埋め込むような構造にすれば、メインルーチンのみ、途中の待ち状態でも他のタスクを走行さ
せることが出来ると思います。（それ以外のタスクは、あいかわらず中断不可）

     ---＊---

タイマー割り込みによって、dispatch_task()を呼び出すような方法も考えられますが、他の
割り込み処理との競合問題や、フォアグラウンド側との競合関係を解決する必要があり、問題は
より複雑になります。
また、タイマー割り込みが長時間の割り込み禁止状態を引き起こすので、何らかの工夫が必要
になるかもしれません（クリティカル処理終了後は、割り込み許可状態にする等）


■ ＡＴｍｅｇａ版との違い

メモリー容量と速度上の制約により、
・morse を外してあります。
・kbd_getchar の実装を外してあります。
・kbd_presstable の実装を外してあります。
・sleepカウンタのサイズをuchar に変更しました。

リソースの違いにより
・ATtiny2313にはPCINT8 割り込みが存在しませんので、TIMER1割り込みのみの対応です。
・ps2keybrdとsound outのポート割り当てを portc から portb に変更してあります。



■ 積み残し／問題点

・PS2 keyboardのデータ取得方法が AVR313 AN のままなので、エラー処理等がありません.

