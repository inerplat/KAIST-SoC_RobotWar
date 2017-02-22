# [KAIST-SoC_RobotWar](https://github.com/inerplat/KAIST-SoC_RobotWar)

작성자 : DH Kim

FPGA와 Image_Load폴더에 해당파트의 상세한 설명이 있습니다.

작업시 주의사항
```
1. RS232C-스트레오 3극 통신할때 보드단자가 취약합니다. 모션을 실행할때는 반드시 분리하세요.

2. 두뇌보드-로봇간 시리얼 통신에서 아무것도 없는 핀은 하단 3번째, 흰색 레이블이 달린 핀이 하단 4번째에 들어가야합니다.

3. 보드에 카메라와 보드전원 및 시리얼 통신 케이블 규격은 같지만, VCC와 GND의 위치가 다릅니다. 반대로 꽂지 않도록 주의하세요.

4. 로봇이 갑자기 주저앉는다면 높은확률로 배터리 부족입니다. 당황하지마시고 충전부터 하세요

  4-1. 충전을 해도 토크가 부족하거나 잦은 오버로드에러가 뜬다면 고장입니다.
```
## [Eagle-MCU source](https://github.com/inerplat/KAIST-SoC_RobotWar/tree/master/Image_load)
두뇌보드 중 MCU에 올라가는 소스 입니다
FPGA로부터 처리된 영상을 통해 색상을 기준으로 판단하고 공격을 합니다

gcc컴파일러를 cygwin을 이용하여 컴파일 했으며 RS232C-스트레오3극 시리얼통신을 putty를 이용해 진행했습니다
통신에 문제가 있다면 당황하지 마시고 아래사항을 확인하길 바랍니다
```
1. 장치관리자에서 드라이버가 온전히 설치되었는지 확인

2. 통신속도를 115200으로 변경하였는지 확인

3. 스트레오 3극케이블이 보드에 느슨하게 꽂혀있지 않은지 확인

4. 실행파일 경로(/mnt/f0/)에 eagle downloader를 이용하여 파일을 생성했는지 확인

 4-1. eagle downloader를 이용하는데 mini usb케이블이 연결되어야 합니다

 4-2. usb_download를 실패하더라도 f0폴더에 파일은 생성됩니다. 실패한경우 rm명령어를 통해 삭제하세요.

5. 카메라 포트에 전원을 혹은 전원포트에 카메라를 연결하셨다면 그동안 수고하셨습니다.

6. 실행파일을 부트파일로 지정한경우 'nand write.jffs2 0xc0000000 0x200000 0x100000'명령어를 통해 변경할 수 있습니다

 6-1. 오타로 메모리 주소를 잘못 입력 하셨다면 대전여행 다녀오시면 됩니다.
```
## [Verilog HDL FPGA source](https://github.com/inerplat/KAIST-SoC_RobotWar/tree/master/FPGA)

 SAA7111레지스터를 사용하는 Video decoder로부터 YCbCr로 구성된 영상데이터를 FPGA에 받습니다.

 Cb Cr이 색차인 것을 이용하여 Red에 인접한 색상은 모두 Red로, Green에 인접한 색상은 모두 Green으로, Blue에 인접한색상은 Blue로 변환하며
 Y기 60미만인 경우 검은색, 190 이상인 경우는 흰색으로 처리합니다.
 YCbCr데이터가 R, G, B 벡터로 변환될때 처리되며, R, G, B벡터는 DecVdata벡터에 RGB565방식으로 저장이 됩니다.
 스풀링으로 인한 프레임드롭을 방지하기 위해 SRAM을 2구간으로 나누어 영상데이터를 번갈아 가며 저장하며, 최종적으로 vdata에 있는 영상정보를 SRAM에 저장하고 eamem_data를 통해 메모리주소를 eagle-MCU로 전송합니다.


 FPGA보드에 프로그래밍이 안되는 경우 아래와 같은상황인지 확인하길 바랍니다.
```
1. FPGA 보드에 프로그램을 업로드하는경우 전원과 USB-Blaster만 필요합니다. RS232C나 mini-usb는 굳이 꽂을필요는 없습니다.

2. 컴파일하고 바로 programmer탭으로 업로드하면 안됩니다.매번 jic파일을 변경해야합니다.

  2-1. Convert Programming files에서 programming file type을 JTAG Indirect Configueration File(.jic)를 선택했는지 확인합니다.

  2-2. Device를 EPCS64로 변경했는지 확인합니다.

  2-3. Flash Loader Device 를 Cyclone IV E / EP4CE75로 변경하였는지 확인합니다.

  2-4. SOF Data를 SoC_Brain.sof로 변경하였는지 확인합니다.
```
