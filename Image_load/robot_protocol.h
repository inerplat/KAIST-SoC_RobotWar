/* ------------------------------------------------------------------------ */
/*                  KAIST BATTLE ROBOT CONTROL PROGRAM						*/
/*                   														*/
/*          Coding by YKH      												*/
/*          file : robot_protocol.h    										*/
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
/*                  KAIST BATTLE ROBOT CONTROL PROGRAM            			*/
/*                   														*/
/*          Modified by Goon-Ho Choi, HANOOL ROBOTICS 2003.07.16     		*/
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
#ifndef __ROBOT_PROTOCOL_H__
#define __ROBOT_PROTOCOL_H__

#define Header0	0xff
#define Header1 0xff
#define Packet	0x09
#define pID	0xfd
#define CMD	0x16
#define DATA1	0x00
///////////////////////////////////////////////////////////////////////////////
void DelayLoop(int delay_time);
void Send_Command(unsigned char CS1, unsigned char CS2, unsigned char DATA0);
unsigned char Receive_Ack(void);
void Motion(unsigned char DATA0);
void init_robot(void);
void Turn_left();
void Turn_right();
void F_walk();
void B_walk();
void Standup_B();
void Standup_F();
void Attack1();
void Attack2();
void S_Attack1();
void S_Attack2();
void S_Attack3();
void S_Attack4();
void Kick();
void combo1();
void combo2();
void combo3();
void combo4();
void combo5();
void combo6();
void combo7();
void qturn1();
void qturn2();
////////////////////////////////////////////////////////////////////////////////

#endif // __ROBOT_PROTOCOL_H__

