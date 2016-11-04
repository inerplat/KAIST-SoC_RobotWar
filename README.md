#KAIST-SoC_RobotWar

작성자 : DH Kim

FPGA와 Image_Load폴더에 해당파트의 상세한 설명이 있습니다.

작업시 주의사항

1. RS232C-스트레오 3극 통신할때 보드단자가 취약합니다. 모션을 실행할때는 반드시 분리하세요.

2. 두뇌보드-로봇간 시리얼 통신에서 아무것도 없는 핀은 하단 3번째, 흰색 레이블이 달린 핀이 하단 4번째에 들어가야합니다.

3. 보드에 카메라와 보드전원 및 시리얼 통신 케이블 규격은 같지만, VCC와 GND의 위치가 다릅니다. 반대로 꽂지 않도록 주의하세요.

4. 로봇이 갑자기 주저앉는다면 높은확률로 배터리 부족입니다. 당황하지마시고 충전부터 하세요

 4-1. 충전을 해도 토크가 부족하거나 잦은 오버로드에러가 뜬다면 고장입니다.
