#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>

#include "uart_api.h"
#include "cam_disp.h"
#include "robot_protocol.h"
#include <termios.h>
static struct termios inittio, newtio;

#define DISABLE_IMG_IRQ      0x24402
#define RD_IMG_DATA         0x24403
#define DISPLAY_IMG_DATA   0x24404
#define DISPLAY_IMG_DRAW   0x24405   
#define CLEAR_SCREEN      0x24407
#define GRAPHIC_FLIP   0x24410

int devfb;
void init_console(void)
{
	tcgetattr(0, &inittio);
	newtio = inittio;
	newtio.c_lflag &= ~ICANON;
	newtio.c_lflag &= ~ECHO;
	newtio.c_lflag &= ~ISIG;
	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 0;

	cfsetispeed(&newtio, B115200);

	tcsetattr(0, TCSANOW, &newtio);
}


void Delay(int delay_time)
{
	while (delay_time)
		delay_time--;
}

typedef struct __draw_raw_value {
	int InitDX;
	int InitDY;
	int EndX;
	int EndY;
	int InitSX;
	int InitSY;
	int dxSx;
	int dxSy;
	int dySx;
	int dySy;
	int imgbuf_en;
	unsigned short *imgbuf;
} DrawRaw_value;

static DrawRaw_value draw_value;

struct __shade_color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

int draw_rotate_value(int cdx, int cdy, int ctx, int cty, float zoom, unsigned int angle)
{
	float MySin, MyCos;

	int InitDX, InitDY;
	int EndX, EndY;
	int InitSX, InitSY;
	int dxSx, dxSy;
	int dySx, dySy;
	int dx, dy;
	int x, y;
	float a;
	int cosa;
	int sina;
	int rhzoom;

	x = y = 0;
	dx = 320;
	dy = 480;

	long tx, ty;

	angle %= 360;
	a = angle*(3.1415926535f * 2) / 360.0f;

	MySin = (float)sin(a);
	MyCos = (float)cos(a);

	tx = (-cdx / zoom) * MyCos + (-cdy / zoom) * MySin;
	ty = (cdx / zoom) * MySin + (-cdy / zoom) * MyCos;

	if (zoom <= 0)   rhzoom = 0;
	else         rhzoom = (int)((float)(1 << 9) / zoom);
	cosa = (int)(rhzoom * cos(a));
	sina = (int)(rhzoom * sin(a));

	if (dx <= 0 || dy <= 0)
		return -1;

	InitDX = x;
	InitDY = y;
	EndX = x + dx - 1;
	EndY = y + dy - 1;

	InitSX = (x + tx + ctx) * 512;
	dxSx = cosa;
	dxSy = -sina;

	InitSY = (y + ty + cty) * 512;
	dySx = sina;
	dySy = cosa;

	draw_value.InitDX = InitDX;
	draw_value.InitDY = InitDY;
	draw_value.EndX = EndX;
	draw_value.EndY = EndY;
	draw_value.InitSX = InitSX;
	draw_value.InitSY = InitSY;
	draw_value.dxSx = dxSx;
	draw_value.dxSy = dxSy;
	draw_value.dySx = dySx;
	draw_value.dySy = dySy;

	//   printf("InitDX=%d\n", InitDX);
	//   printf("InitDY=%d\n", InitDY);
	//   printf("EndX=%d\n", EndX);
	//   printf("EndY=%d\n", EndY);
	//   printf("InitSX=%d\n", InitSX);
	//   printf("InitSY=%d\n", InitSY);
	//   printf("dxSx=%d\n", dxSx);
	//   printf("dxSy=%d\n", dxSy);
	//   printf("dySx=%d\n", dySx);
	//   printf("dySy=%d\n", dySy);

	return 0;
}

int draw_img_from_buffer(unsigned short *Buffer, int CenterX, int CenterY, float zoom, int rotate_angle)
{
	draw_rotate_value(CenterX, CenterY, 90, 60, zoom, rotate_angle);
	draw_value.imgbuf = Buffer;

	if (ioctl(devfb, DISPLAY_IMG_DRAW, &draw_value)) {
		printf("Draw Image to LCD -> fail!!!\n");
		exit(1);
	}
}

void ClearScreen(unsigned char r, unsigned char g, unsigned char b)
{
	struct __shade_color clear_shade;

	clear_shade.r = r;
	clear_shade.g = g;
	clear_shade.b = b;

	if (ioctl(devfb, CLEAR_SCREEN, &clear_shade)) {
		printf("Clear Screen -> fail!!!\n");
		exit(1);
	}
}

static int gFlip(void)
{
	return ioctl(devfb, GRAPHIC_FLIP, 0);
}

int ImageProcess_Open(void)
{
	int fb;
	fb = open("/mnt/f0/dev_module/imgproc", O_RDWR);
	if (fb < 0) {
		printf("/mnt/f0/dev_module/imgproc open failed!!\n");
		return -1;
	}

	devfb = fb;

	return 0;
}

int ReadImageFromFPGA(unsigned int *addr)
{
	unsigned int buf_addr;

	if (ioctl(devfb, RD_IMG_DATA, &buf_addr)) {
		printf("ioctl RD_IMG_DATA error\n");
		return -1;
	}
	*addr = buf_addr;

	return 0;
}

int ImgDisplayToLCD(void)
{
	draw_rotate_value(160, 240, 90, 60, 2.5, 90); // FPGA로부터 읽어 온 영상(180x120)의 중심점(90,60)을 기준으로하여 90도 회전하고, 2.5배 확대하여 LCD의 중심점(160,240)에 위치하도록 셋팅하는 함수 : comment by yyb[110909]
	if (ioctl(devfb, DISPLAY_IMG_DATA, &draw_value)) {
		printf("ioctl DISPLAY_IMG_DATA error\n");
		return -1;
	}

	return 0;
}

void ImgDisplayQuit(void)
{
	ioctl(devfb, DISABLE_IMG_IRQ, 0);
}

int main(int argc, char **argv)
{
	int x, y, z, w;
	int t_row_s = 0, t_col_s = 0;
	int t_row_e = 0, t_col_e = 0;
	int row_s = 0, col_s = 0;
	int row_e = 0, col_e = 0;
	int row_l = 0, col_l = 0;
	int schk, tchk;
	int ccnt, cchk;
	int tmptmp, tmp;
	int tg, p_tg, m_tg, p_m_tg;
	int attack_mode;
	char ch;
	int ret;
	int cnt = 0;
	unsigned char buff[5] = { 0, };
	unsigned int buf_addr;
	unsigned short(*img_buf)[256];
	init_console();
	if (argc <= 1) {
		printf("Usage 1 : imgproc_test -rd     <Read Image Data>\n");
		printf("Usage 2 : imgproc_test -dp     <Display to LCD>\n");
		printf("Usage 3 : imgproc_test -dp2    <Display 2 Frame>\n");
		exit(1);
	}
	//   eagle_camera_off(); // 커널에서 동작되는 Camera OFF : comment by yyb[110909]

	//[[ molink_yyb_110909_BEGIN -- 로봇 몸체와의 통신을 위한 UART 초기화
	ret = uart_open();
	if (ret < 0) return EXIT_FAILURE;
	uart_config(UART1, 115200, 8, UART_PARNONE, 1);
	//]] molink_yyb_110909_END -- 로봇 몸체와의 통신을 위한 UART 초기화

	ret = ImageProcess_Open(); // 이미지 처리를 위한 드라이버 Open : comment by yyb[110909]
	if (ret < 0) return EXIT_FAILURE;

	Delay(0xffffff);

	if (strcmp("-dp", argv[1]) == 0) {
		//ret = ImgDisplayToLCD(); // FPGA로 부터 읽어온 이미지를 LCD에 실시간으로  Display : comment by yyb[110909]
		//if (ret < 0) return EXIT_FAILURE;
		printf("\nPress Enter Key to STOP the test !!!");


		init_robot(); //로봇초기화

		Delay(0xffffff);
		Delay(0xffffff);
		while (1)
		{
			ret = ReadImageFromFPGA(&buf_addr); //buf_addr에 영상주소를 받아오고, 영상을 받아왔는지 확인하는 변수인 ret에 값을넣는다
			if (ret < 0) return EXIT_FAILURE; // ret<0(영상받아오기 실패)인경우 프로그램을 종료한다
			img_buf = (unsigned short(*)[256])buf_addr; //img_buf포인터 배열에 영상을 집어넣는다
			cnt = 0;
			printf("hot ssan\n");
			col_e = 0;
			col_s = 0;
			row_s = 0;
			tchk = 0;
			for (x = 5; x < 180; x++) // 0부터 실행할 경우 쓰레기데이터를 받아오는 오류로 인해 5부터 시작
			{
				schk = 0;
				for (y = 5; y < 120; y++)
				{

					//---------------------------------------------------------------------

					if (img_buf[y][x] == 0) // img_buf[y][x]이 검은색(0)일경우
					{
						ccnt = 0;
						cchk = 0;
						for (z = 1; z <= 10; z++) // 세로로 +10, -10씩 이동하며 초록색(2016)과 검은색(0)을 찾는다
						{
							if (img_buf[y + z][x] == 2016) ++ccnt;
							if (img_buf[y - z][x] == 2016) ++cnt;
							if (img_buf[y + z][x] == 0) ++cchk;
							if (img_buf[y - z][x] == 0) ++cchk;
						}
						if (ccnt > 6) img_buf[y][x] = 2017; // 찾은 초록색이 6을 초과하면 검은색(0)을 초록색(2017)로 변환한다
						else if (cchk > 6) img_buf[y][x] = 65504; // 찾은 검은색이 6을 초과하면 검은색(0)을 노란색(65504)로 변환한다
						ccnt = 0;
						cchk = 0;
						for (z = 1; z <= 3; z++) // 위와 같은 방식으로 가로로 +3,-3씩 이동하며 초록색과 검은색을 찾고, 변환한다
						{
							if (img_buf[y][x + z] == 0) ++cchk;
							if (img_buf[y][x - z] == 0) ++cchk;
							if (img_buf[y][x + z] == 2016) ++ccnt;
							if (img_buf[y][x - z] == 2016) ++ccnt;
						}
						if (ccnt > 1) img_buf[y][x] = 2017;
						else if (cchk > 1) img_buf[y][x] = 65504;
					}

					//-----------------------------------------------------------------------
					if (schk == 0 && (img_buf[y][x] == 2016 || img_buf[y][x] == 2017 || (img_buf[y][x] == 0 && (img_buf[y + 5][x] == 2016) || (img_buf[y + 6][x] == 2016)))) // schk가 0(로봇을 인식할 시작점)이고 img_buf[y][x]가 초록색(2016,2017)이거나 검은색이고,+5혹은 +6번째가 초록색이면
					{
						schk = 1; // 명령을 다시 실행하지 않도록 schk에 1을 넣어준다
						tmptmp = x; // 세로길이를 측정할 x축의 좌표를 tmptmp에 넣는다
						t_col_s = y; // 세로길이를 측정할 시작점의 y좌표를 t_col_s에 넣는다
					}
					if (img_buf[y][x] == 63488 || img_buf[y][x] == 31) // img_buf[y][x]가 빨간색(63488)혹은 파란색(31)이면
					{
						++tg; // 지름측정
						p_tg = x; // 지름의 x좌표 기록
					}

					if (img_buf[y][x] == 2016 || img_buf[y][x] == 2017) //img_buf[y][x]가 초록색(2016,2017)이면
					{
						t_row_e = x; // t_row_e에 x좌표를 넣어 끝까지 갱신한다
						t_col_e = y; // 마찬가지로 y좌표를 넣어 끝까지 갱신한다
					}
				}
				if (tg > m_tg) // 새로 측정한 태극무늬의 지름이 기존에 측정한 길이보다 길면
				{
					m_tg = tg; // 태극무늬의 지름을 갱신하고
					p_m_tg = p_tg; // x좌표를 p_m_tg에 받는다
				}
				if (t_col_e - t_col_s > col_e - col_s) // 새로 측정한 세로길이(끝점-시작점)이 기존에 기록하고있던 세로길이보다 길면 시작점과 끝점과 x좌표를 기록한다
				{
					col_e = t_col_e;
					col_s = t_col_s;
					col_l = tmptmp;
				}
				//-------------------------------------------------------------------

				if (x > 120) continue; // 처리속도를O(2n*m)에서 O(n^2)로 단축시키기 위한 꼼수, 이전까지의 코드는 세로검사, 이후부터의 코드는 가로검사.
				schk = 0;
				for (w = 5; w < 180; w++)
				{
					if (schk == 0 && (img_buf[x][w] == 2016))
					{
						schk = 1;
						t_row_s = w;
						tmp = x;
					}
					if (img_buf[x][w] == 2016 || img_buf[x][w] == 2017)
					{
						t_row_e = w;
						t_col_e = x;
					}
				}
				if (t_row_e - t_row_s > row_e - row_s)
				{
					row_e = t_row_e;
					row_s = t_row_s;
					row_l = tmp;
				}
			}
			printf("%d %d %d\n", col_l, col_s, col_e);
			if (row_e - row_s > 120 && col_l > 80 && col_l < 100 && col_e - col_s>110) // 인식된 로봇의 가로길이가 120초과, 로봇의 중심의 x좌표가 80초과 100미만, 로봇의 세로길이가 110초과인경우 발차기를한다
			{
				printf("assa kick\n");
				Kick();
			}
			else if (col_l > 60 && col_l < 120 && col_e - col_s>20) // 로봇의 중심의 x좌표가 60초과 120미만, 로봇의 세로 길이가 20초과인경우
			{
				printf("%d %d\n", col_e, col_s);
				if (col_e - col_s > 20 && col_e - col_s < 90) //세로길이가 20초과 90미만인경우(로봇이 멀리있는경우)
				{
					printf("GO\n");
					F_walk(); //전진한다
				}
				else if (col_e - col_s >= 100) // 세로길이가 100이상인경우 공격을 한다
				{
					printf("honbap attack!");
					attack_mode %= 7; // 로봇프로토콜에 등록된 모션 7가지중 하나를 순차적으로 실행한다
					++attack_mode;
					printf(" NO. %d\n", attack_mode);
					if (attack_mode == 1) combo1();
					else if (attack_mode == 2) combo2();
					else if (attack_mode == 3) combo3();
					else if (attack_mode == 4) combo4();
					else if (attack_mode == 5) combo5();
					else if (attack_mode == 6) combo6();
					else if (attack_mode == 7) combo7();
					Delay(5000000);
				}

			}
			else if (col_l > 0 && col_l < 60 && col_e - col_s>30) // 로봇의 x좌표가0초과 60미만, 로봇의 길이가 30초과인경우(로봇이 인식되고 왼쪽에 치우쳐진경우)
			{
				printf("%d left\n", row_s);
				Turn_left(); // 왼쪽으로 돈다
				F_walk();
			}
			else if (col_l > 120 && col_e - col_s > 30) // 로봇의 x좌표가 120을 초과하고, 로봇의 길이가 30초과인경우(로봇이 인식되고 오른쪽에 치우쳐진경우)
			{
				printf("%d right\n", row_s);
				Turn_right(); // 오른쪽으로돈다
				F_walk;
			}
			else // 아무것도 인식하지 못했을경우
			{
				if (p_tg > 85 && p_tg < 95) // 만약 중심에 태극기가 있으면
				{
					printf("Ang gimottg");
					F_walk(); // 전진한다
				}
				Turn_left(); // 없으면 왼쪽으로 회전한다
				Delay(0xffffff);
			}
		}
		ch = getchar();
		ImgDisplayQuit(); // LCD Display 종료 : comment by yyb[110909]
		printf("\nTest is Stopped\n");
	}
	/*
	else if(strcmp("-dp2", argv[1]) == 0) {
	ClearScreen(255, 255, 255); // LCD 화면을 백색으로 Clear : comment by yyb[110909]
	gFlip(); // 그래픽 처리된 내용을 LCD에 보여줌 : comment by yyb[110909]
	ClearScreen(255, 255, 255);
	gFlip();
	ClearScreen(255, 255, 255);
	gFlip();
	ClearScreen(255, 255, 255);
	gFlip();
	printf("Clear Screen!\n");
	ret = ReadImageFromFPGA(&buf_addr);
	if (ret < 0) return EXIT_FAILURE;
	printf("Image Load from FPGA!\n");
	draw_value.imgbuf_en = 0; // 읽어 온 데이터를 처리하지 않고 그대로 다시 LCD에 보여줄 때 설정 : comment by yyb[110909]
	ClearScreen(255, 255, 255);
	draw_img_from_buffer((unsigned short *)buf_addr, 160, 130, 1.8, 0); // buf_addr에 들어 있는 내용을 1.8배 확대하여 0도 회전하고 중심을 (160, 130)로 하여  Display : comment by yyb[110909]
	//      gFlip();
	printf("1. Draw Image to LCD!\n");
	//    }
	printf("\nPress Enter Key to load image again !!!");
	ch = getchar();

	ret = ReadImageFromFPGA(&buf_addr);
	if (ret < 0) return EXIT_FAILURE;
	img_buf = (unsigned short (*)[256])buf_addr;
	printf("Image Load from FPGA!\n");
	for(y=0;y<=120;y++)
	{
	for(x=0;x<=180;x++)
	{
	if(img_buf[y][x]==2016) img_buf[y][x]=0;
	}
	}
	//////////////////////////////////////////////////////////////////
	// img_buf의 데이터에 대한 이미지 처리 과정이 이 위치에서 이루어짐 //
	//////////////////////////////////////////////////////////////////

	draw_value.imgbuf_en = 1; // 읽어 온 데이터를 처리하고, 처리된 데이터를 LCD에 보여줄 때 설정 : comment by yyb[110909]
	draw_img_from_buffer((unsigned short *)buf_addr, 160, 360, 1.8, 0);
	gFlip();
	printf("2. Draw Image to LCD!\n");
	printf("\nPress Enter Key to STOP the test !!!");
	ch = getchar();
	printf("\nTest is Stopped\n");

	}
	else {
	printf("Usage 1 : imgproc_test -rd     <Read Image Data>\n");
	printf("Usage 2 : imgproc_test -dp     <Display to LCD>\n");
	printf("Usage 3 : imgproc_test -dp2    <Display 2 Frame>	\n");
	exit(-1);
	}
	*/
	uart_close();
	close(devfb);

	eagle_camera_on(); // 커널에서 동작되는 Camera ON : comment by yyb[110909]

	return(0);
}
