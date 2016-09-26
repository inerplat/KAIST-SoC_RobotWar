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
   while(delay_time)
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
   float MySin,MyCos;

   int InitDX, InitDY;
   int EndX, EndY;
   int InitSX, InitSY;
   int dxSx, dxSy;
   int dySx, dySy;
   int dx,dy;
   int x,y;
   float a;
   int cosa;
   int sina;
   int rhzoom;

   x = y = 0;
   dx = 320;
   dy = 480;

   long tx,ty;

   angle %= 360;
   a = angle*(3.1415926535f*2)/360.0f;

   MySin = (float)sin( a );
   MyCos = (float)cos( a );

   tx = (-cdx/zoom) * MyCos + (-cdy/zoom) * MySin;
   ty = (cdx/zoom) * MySin + (-cdy/zoom) * MyCos;

   if( zoom<=0 )   rhzoom = 0;
   else         rhzoom = (int)((float)(1<<9)/zoom);
   cosa = (int)(rhzoom * cos( a ));
   sina = (int)(rhzoom * sin( a ));

   if( dx<=0 || dy<=0 ) 
      return -1;

   InitDX   = x;
   InitDY   = y;
   EndX   = x+dx-1;
   EndY   = y+dy-1;

   InitSX   = (x+tx+ctx)*512;
   dxSx   = cosa;
   dxSy   = -sina;

   InitSY   = (y+ty+cty)*512;
   dySx   = sina;
   dySy   = cosa;

   draw_value.InitDX=InitDX;
   draw_value.InitDY=InitDY;
   draw_value.EndX=EndX;
   draw_value.EndY=EndY;
   draw_value.InitSX=InitSX;
   draw_value.InitSY=InitSY;
   draw_value.dxSx=dxSx;
   draw_value.dxSy=dxSy;
   draw_value.dySx=dySx;
   draw_value.dySy=dySy;
   
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
   draw_rotate_value(CenterX,CenterY,90,60,zoom,rotate_angle);
   draw_value.imgbuf = Buffer;

   if( ioctl(devfb, DISPLAY_IMG_DRAW, &draw_value) ) {
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

   if( ioctl(devfb, CLEAR_SCREEN, &clear_shade) ) {
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
   fb=open("/mnt/f0/dev_module/imgproc", O_RDWR);
   if(fb < 0) {
      printf("/mnt/f0/dev_module/imgproc open failed!!\n");
      return -1;
   }

   devfb = fb;

   return 0;   
}

int ReadImageFromFPGA(unsigned int *addr)
{
   unsigned int buf_addr;
   
   if ( ioctl(devfb, RD_IMG_DATA, &buf_addr) ) {
      printf("ioctl RD_IMG_DATA error\n");
      return -1;
   }
   *addr = buf_addr;

   return 0;
}

int ImgDisplayToLCD(void)
{
   draw_rotate_value(160,240,90,60,2.5,90); 
   if ( ioctl(devfb, DISPLAY_IMG_DATA, &draw_value) ) {
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
    int t_row_s=0, t_col_s=0;
    int t_row_e=0, t_col_e=0;
    int row_s=0, col_s=0;
    int row_e=0, col_e=0;
    int row_l=0, col_l=0;
    int schk, tchk;
    int ccnt, cchk;
    int tmptmp, tmp;
    int tg, p_tg, m_tg ,p_m_tg;
    int attack_mode;
    int turn_cnt=0;
    int black_cnt;
   char ch;
   int ret;
   int cnt=0;
   int atk_cnt=0;
    unsigned char buff[5] = {0,};
   unsigned int buf_addr;
   unsigned short (*img_buf)[256];
      init_console();
   if(argc <= 1) {
      printf("Usage 1 : imgproc_test -rd     <Read Image Data>\n");
      printf("Usage 2 : imgproc_test -dp     <Display to LCD>\n");
      printf("Usage 3 : imgproc_test -dp2    <Display 2 Frame>\n");
      exit(1);
   }
//   eagle_camera_off(); 
   ret = uart_open();
   if (ret < 0) return EXIT_FAILURE;
   uart_config(UART1, 115200, 8, UART_PARNONE, 1);


   ret = ImageProcess_Open(); 
   if (ret < 0) return EXIT_FAILURE;

   Delay(0xffffff);

	if(strcmp("-dp", argv[1]) == 0) {
      //ret = ImgDisplayToLCD();
      //if (ret < 0) return EXIT_FAILURE;
      printf("\nPress Enter Key to STOP the test !!!");


      init_robot();
	
   Delay(0xffffff);
   Delay(0xffffff);
      while(1)
      {
         ret = ReadImageFromFPGA(&buf_addr); 
         if(ret<0) return EXIT_FAILURE;
         img_buf = (unsigned short (*)[256])buf_addr;
         cnt=0;
         printf("hot ssan\n");
		col_e=0;
		col_s=0;
		row_s=0;
		tchk=0;
	        black_cnt=0;
		 for(x=5;x<180;x++)
		 {
			schk=0;
			for(y=5;y<120;y++)
			{

//---------------------------------------------------------------------

				if(img_buf[y][x]==0)
			    {
				    ++black_cnt;
				ccnt=0;
				cchk=0;
				for(z=1;z<=10;z++) 
				{
				    if(img_buf[y+z][x]==2016) ++ccnt;
				    if(img_buf[y-z][x]==2016) ++cnt;
				    if(img_buf[y+z][x]==0) ++cchk;
				    if(img_buf[y-z][x]==0) ++cchk;
				}
				if(ccnt>6) img_buf[y][x]=2017;
				else if(cchk>6) img_buf[y][x]=65504; // 찾은 검은색이 6을 초과하면 검은색(0)을 노란색(65504)로 변환한다
				ccnt=0;
				cchk=0;
				for(z=1;z<=3;z++) // 위와 같은 방식으로 가로로 +3,-3씩 이동하며 초록색과 검은색을 찾고, 변환한다
				{
				    if(img_buf[y][x+z]==0) ++cchk;
				    if(img_buf[y][x-z]==0) ++cchk;
				    if(img_buf[y][x+z]==2016) ++ccnt;
				    if(img_buf[y][x-z]==2016) ++ccnt;
				}
				if(ccnt>1) img_buf[y][x]=2017;
				else if(cchk>1) img_buf[y][x]=65504;
			 }			

//-----------------------------------------------------------------------
			    if(schk==0 && (img_buf[y][x]==2016 || img_buf[y][x]==2017 || (img_buf[y][x]==0 && (img_buf[y+5][x]==2016) || (img_buf[y+6][x]==2016))))
				{
					schk=1; 
					tmptmp=x; 
					t_col_s=y;
				}
/*
			    if(img_buf[y][x]==63488 || img_buf[y][x]==31) 
				{
					++tg; 
					p_tg=x;
				}
*/

				if(img_buf[y][x]==2016 || img_buf[y][x]==2017)
				{
					t_row_e=x;
					t_col_e=y; 
				}
			}
/*
			if(tg>m_tg)
			{
				m_tg=tg;
				p_m_tg=p_tg; 
			}
*/
			if(t_col_e-t_col_s>col_e-col_s) 
			{
				col_e=t_col_e;
				col_s=t_col_s;
				col_l=tmptmp;
			}
//-------------------------------------------------------------------

			if(x>120) continue; 
			schk=0;
			for(w=5;w<180;w++)
			{
			    if(schk==0 && (img_buf[x][w]==2016))
			    {
				schk=1;
				t_row_s=w;
				tmp=x;
			    }
			    if(img_buf[x][w]==2016 || img_buf[x][w]==2017)
			    {
				t_row_e=w;
				t_col_e=x;
			    }
			}
			if(t_row_e-t_row_s>row_e-row_s)
			{
			    row_e=t_row_e;
			    row_s=t_row_s;
			    row_l=tmp;
			}
		}
	printf("%d %d %d\n",col_l,col_s,col_e); 
	++atk_cnt;
	// if(black_cnt>=17000)
	/*
	if(atk_cnt>=4)
	{
		//주위 공격 모션
		Delay(5000000);
	}

	}
	*/
	if(row_e-row_s>110 && col_l>75 && col_l<105 && col_e-col_s>110)
	{
		printf("assa kick\n");
		Kick();
		atk_cnt=0;
	}
	else if(col_l>60 && col_l<120 && col_e-col_s>15)
	{
		printf("%d %d\n",col_e,col_s);
		if(col_e-col_s>15 && col_e-col_s<90)
		{
			printf("GO\n");
			F_walk();
		}
		else if(col_e-col_s>=90) 
		{
			printf("honbap attack!");
			attack_mode%=7;
			++attack_mode;
			printf(" NO. %d\n",attack_mode);
			if(attack_mode==1) combo1();
			else if(attack_mode==2) combo2();
			else if(attack_mode==3) combo3();
			else if(attack_mode==4) combo4();
			else if(attack_mode==5) combo5();
			else if(attack_mode==6) combo6();
			else if(attack_mode==7) combo7();
			atk_cnt=0;
			Delay(5000000);
		}		
	}
	else if(col_l>0 && col_l<60 && col_e-col_s>30)
	{
		printf("%d left\n",row_s);
		Turn_left();
	}
	else if (col_l>120 && col_e-col_s>30) 
	{
		printf("%d right\n",row_s);
		Turn_right();
	}
	else
	{
	    ++turn_cnt;
	    if(turn_cnt>8) turn_cnt=0;
	    else if(turn_cnt>4) Turn_right();
	    else if(turn_cnt<=4) Turn_left();
	    printf("Eye Lee shin\n");
	}
/*
	else
	{
		if(p_tg>85 && p_tg<95)
		{
		    printf("Ang gimottg");
		    F_walk(); 
		}
		Turn_left();
		Delay(0xffffff);
	}
*/
}
	   ch = getchar();
      ImgDisplayQuit();
      printf("\nTest is Stopped\n");
   }
   uart_close();
   close(devfb);

   eagle_camera_on();

   return(0);
}
