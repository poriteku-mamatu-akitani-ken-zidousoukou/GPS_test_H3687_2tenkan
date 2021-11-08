/***********************************************************************/
/*                                                                     */
/*  FILE        :GPS_LCD_3687F_test.c                                  */
/*  DATE        :Sat, Mar 27, 2021                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :H8/3687                                               */
/*                                                                     */
/*  This file is generated by Hitachi Project Generator (Ver.2.1).     */
/*                                                                     */
/***********************************************************************/
/*************************************************************************/
/*                                                                       */
/*  FILE        :main.c                                                  */
/*  DATE        :Mon, Sep 25, 2010                                       */
/*  DESCRIPTION :Main Program                                            */
/*  CPU TYPE    :H8/3687F                                                */
/*                                                                       */
/*  This file is generated by KPIT GNU Project Generator.                */
/*                                                                     	 */
/*************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
//void abort(void);
//#ifdef __cplusplus
//}
//#endif

/*$GPGGA,062854,3529.5587,N,13638.5868,E,1,04,03.0,00019.3,M,034.9,M,,*43*/
/*12345678901234567890123456789012345678901234567890123456789012345678901*/

/*$GPRMC,062854,A,3529.5587,N,13638.5868,E,000.0,328.5,130910,,,A*7A*/
/*123456789012345678901234567890123456789012345678901234567890123456*/

#include <machine.h>
#include "iodefine.h"
#include "string.h"
#include "stdlib.h"
//#include "sci3.c"

//void sci3_2_init (void);					// SCI3初期化関数
//void sci3_2_send (char data);				// 1文字送信関数
//void sci3_2_send_str (char *str);			// 文字列送信関数
//char sci3_2_recv (void);						// 1文字受信関数

//#define	BRG_9600	64
//#define CNTR_9600	0

//#define PAR_NONE	0x00
//#define PAR_ODD		0x30
//#define PAR_EVEN	0x20

//#define STOP_1		0x00
//#define STOP_2		0x08

//#define BIT_7		0x40
//#define BIT_8		0x00

	char rx_data;								// 受信データ
	char rx_buffer[500];

	int copys = 0,mozi = 0; 					//何番目かと文字数
	char gps_data_copy[30];		    			//基準データコピー
									// 受信バッファ
	unsigned char rx_rd_ptr;
    unsigned char rx_wr_ptr;
	char a[7];											// 比較用基準文字列
	char b[7];											// 比較用文字列
	char wt[7];										// rx_b[7]から世界標準時
	char lati[11];										// rx_b[14]から緯度	
	char longi[12];										// rx_b[26]から経度
	char alti[6];										// rx_b[59]から高度
	char disp1[17];										// 1行目液晶表示文字列
	char disp2[17];										// 2行目液晶表示文字列
	

//	P5DDR = 0xff;
//	P1DDR = 0xff;								//ポート1全て書き込み
// UART割り込み
// vector 23 SCI3
//__interrupt(vect=23) void INT_SCI3(void)
//{
//	rx_buffer[rx_wr_ptr]=SCI3.RDR;
//	rx_wr_ptr++;
//	if (rx_wr_ptr>=500)
//		rx_wr_ptr=0;
//}


//void sci3_2_init (void)						// SCI3初期化関数
//{
//	int i;
//	SCI3_2.SCR3.BIT.TE  = 0;				//【1】トランスミットイネーブルビット：送信動作を禁止
//	SCI3_2.SCR3.BIT.RE  = 0;				// 　　レシーブイネーブルビット：受信動作を禁止
//	SCI3_2.SCR3.BIT.CKE = 0;				//【2】クロックソースを内部ボーレートジェネレータに設定
//	SCI3_2.SMR.BYTE		= 0x00;				//【3】送受信フォーマットの設定
//	SCI3_2.BRR			= 64;				//【4】ビットレートの設定(9600[bit/s])
//	for (i = 0; i < 1; i++);				//【5】1ビット期間以上待つ(dummy wait)
//	IO.PMR1.BIT.TXD2    = 1;				//【6】P22端子をTXD出力端子機能に設定
//	SCI3.SMR.BYTE=CNTR_9600 | PAR_NONE | STOP_1 | BIT_8;
//	SCI3.BRR=BRG_9600;
//	SCI3_2.SCR3.BIT.TE  = 1;				//【7】トランスミットイネーブルビット：送信動作を許可
//	SCI3_2.SCR3.BIT.RE  = 1;				//【8】レシーブイネーブルビット：受信動作を許可
//	SCI3.SCR3.BIT.RIE=1;
//}

//void sci3_2_send (char data)				// 1文字送信関数
//{
//    while (SCI3_2.SSR.BIT.TDRE == 0) ;		//【1,2】0でデータあり、1になるまで待つ
//    SCI3_2.TDR = data;						//【3】  受け取った文字を送信
//}

//void sci3_2_send_str (char *str)			// 文字列送信関数
//{
//     while(*str != '\0'){					//【4】文字が\0になるまで繰り返す
//          sci3_2_send(*str);				//　　 1文字送信
//          str++;							// 　　次の文字に移る
//     }
//}

char sci3_2_recv (void)						// 1文字受信関数
{
    char data;								// 受信データ格納変数
    while (!(SCI3_2.SSR.BYTE & 0x78)); 		//【1,2】受信またはエラーフラグが立つまで待つ
    if (SCI3_2.SSR.BIT.RDRF == 1) {			//【3】受信完了なら
        data = SCI3_2.RDR;					//【4】データ取り出し
    } 
    else {
        SCI3_2.SSR.BYTE &= 0xC7;			//【5】エラー発生時、エラーフラグをクリア
        data = '\0';						//【6】ダミーデータとして'\0'を返す
    }
    return data;
}
//                 
//#include <machine.h>
//#include "iodefine.h"						// 定義ヘッダファイルのインクルード


void set_SCI3 (void)					// SCI3初期化関数
{
	int i;
	SCI3.SCR3.BIT.TE  = 0;				//【1】トランスミットイネーブルビット：送信動作を禁止
	SCI3.SCR3.BIT.RE  = 0;				// 　　レシーブイネーブルビット：受信動作を禁止
	SCI3.SCR3.BIT.CKE = 0;				//【2】クロックソースを内部ボーレートジェネレータに設定
	SCI3.SMR.BYTE  = 0x00;				//【3】送受信フォーマットの設定
	SCI3.BRR = 0x40;					//【4】ビットレートの設定(19200[bit/s])
	for (i = 0; i < 1; i++) ;			//【5】1ビット期間以上待つ(dummy wait)
	IO.PMR1.BIT.TXD   = 1;				//【6】P22端子をTXD出力端子機能に設定
	SCI3.SCR3.BIT.TE  = 1;				//【7】トランスミットイネーブルビット：送信動作を許可
	SCI3.SCR3.BIT.RE  = 1;				//　　 レシーブイネーブルビット：受信動作を許可
}

void sci3_tx (char data)				// 1文字送信関数
{
    while (SCI3.SSR.BIT.TDRE == 0) ;	//【1,2】0でデータあり、1になるまで待つ
    SCI3.TDR = data;					//【3】  受け取った文字を送信
}

void sci3_tx_str (char *str)			// 文字列送信関数
{
     while(*str != '\0'){				//【4】文字が\0になるまで繰り返す
          sci3_tx(*str);				//　　 1文字送信
          str++;						// 　　次の文字に移る
     }
}

char sci3_rx (void)						// 1文字受信関数
{
    char data;							// 受信データ格納変数
    while (!(SCI3.SSR.BYTE & 0x78)) ;	//【1,2】受信またはエラーフラグが立つまで待つ
    if (SCI3.SSR.BIT.RDRF == 1) {		//【3】受信完了なら
        data = SCI3.RDR;				//【4】データ取り出し
    } 
    else {
        SCI3.SSR.BYTE &= 0xC7;			//【5】エラー発生時、エラーフラグをクリア
        data = '\0';					//【6】ダミーデータとして'\0'を返す
    }
    return data;
}

//void main(void)
//{
//	int i;
//	char rx_data;								// 受信データ格納変数
//	char tx_data[100];							// 送信データ格納変数

//    set_SCI3();									//【1】SCI3初期化
//    sci3_tx_str("H8/3694F SCI3 Test Ready..."); //【2】文字列送信
//	while(1) {		
//               rx_data = sci3_rx();				//【3】文字データ受信							//
//          i = 0;								//
//          do{									// 　　受信入力処理
//               tx_data[i] = rx_data;			//　　 文字配列に格納
//               i++;								//
//          }while(rx_data != '!');				//【4】送受信終了の合図'!'の検出
//          tx_data[i] = '\0';					//【5】ヌル文字の付加
//          sci3_tx_str(tx_data);					//【6】文字列の送信
//     }
//}

void main(void){
	
	int i;
	int c;
	int d;
	int zi,fun,byo,ido1,ido2,ido3,keido,koudo;
	
	rx_wr_ptr=0;

	init_lcd();											// LCD初期化
//	sci3_2_init();										// シリアルポート初期化
	set_SCI3 ();
    lcd_clr();											// LCDの表示クリア

	lcd_xy(1, 1);										// LCD1行目，1文字目
	lcd_puts("GPS DATA DISPLAY");						
	lcd_xy(1, 2);										// LCD2行目，1文字目
	lcd_puts("PROGRAM");
	wait(1000);											// 待ち
	lcd_clr();											// LCDの表示クリア
	
    strcpy(a, "$GPGGA");								// $GPGGAをaにコピー
    
	lcd_clr();											// LCDの表示クリア
	set_imask_ccr(0x00);						//割り込み許可	
	
 //	set_ccr( 0x00 );                    /* 全体割り込み許可         */
	
	while(1) {
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//SWの状態を取得(ポート13)
		while (P1DDR != 0x08)//ONになるまで待機
		{
			;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			do {												// 改行文字でない（一連のデータの一番最後でない）場合
	//			rx_data = sci3_2_recv();					// rx_dataに受信した1文字を入れる
				rx_data = sci3_rx();
				rx_buffer[i] = rx_data;							// 1文字を順に文字列配列に入れる
				i++;
			} while (rx_data != '\n');


			rx_buffer[i] = '\0';
			for (i = 0; i < 6; i++) {							// 文字列rx_dataから最初の6文字を抜き出す
				b[i] = rx_buffer[i];
			}

			b[i] = '\0';
			c = strcmp(a, b);								// 文字列aと文字列bを比較する
			if (c == 0) {	    							// aとbが等しい場合
				if (b[5] == 'A') {

					for (i = 0; i < 6; i++) {					// 世界標準時刻データ取り出し
						wt[i] = rx_buffer[i + 7];
					}
					wt[i] = '\0';

					for (i = 0; i < 9; i++) {					// 緯度データ取り出し
						lati[i] = rx_buffer[i + 18];
					}
					lati[i] = '\0';

					for (i = 0; i < 10; i++) {				// 経度データ取り出し
						longi[i] = rx_buffer[i + 30];
					}
					longi[i] = '\0';

					for (i = 0; i < 4; i++) {					// 高度データ取り出し
						alti[i] = rx_buffer[i + 59];
					}
					alti[i] = '\0';

					/////////////////////////////////[コピーが入る]//////////////////////////

					for (i = 0; i < 9; i++) {     			// 1行目表示用データの編集（緯度）
						disp1[i] = lati[i];
						gps_data_cop_ido[i] = lati[i];			//緯度コピー
					}
					disp1[i] = ' ';


					for (i = 0; i < 6; i++) {					// 世界標準時刻	
						disp1[i + 10] = wt[i];
					}
					disp1[16] = '\0';


					for (i = 0; i < 10; i++) {    			// 2行目表示用データ編集（経度）
						disp2[i] = longi[i];
						gps_data_cop[i+9] = longi[i];	//コピー
					}
					disp2[i] = ' ';							// 高度
					for (i = 0; i < 5; i++) {
						disp2[i + 11] = alti[i];
					}
					disp2[16] = '\0';
					gps_data_cop[19] = '\0';			//コピー
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//　コピーした座標データーから角度を計算
			if(syokai == 0){//初回であれば基準の座標のみ取得

			
			}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					lcd_clr();											// LCDの表示クリア

					lcd_xy(1, 1);
					lcd_puts(disp1);
					lcd_xy(1, 2);
					lcd_puts(disp2);
				}
			}
			else;
		}




	}

}


void abort(void)
{
	
}




