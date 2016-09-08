#Verilog HDL FPGA source

SAA7111레지스터를 사용하는 Video decoder로부터 YCbCr로 구성된 영상데이터를 FPGA에 받습니다.
대회측에서는 RGB로 변환한뒤 영상처리하기를 권고했지만 
독일물드신분의 빅-픽쳐로 YCbCr상태에서 영상처리를 하고 RGB변환 및 사이즈 축소 후 SRAM으로 영상데이터를 보냅니다.


Cb Cr이 색차인 것을 이용하여 Red에 인접한 색상은 모두 Red로, Green에 인접한 색상은 모두 Green으로, Blue에 인접한색상은 Blue로 변환하며
Y기 60미만인 경우 검은색, 190 이상인 경우는 흰색으로 처리합니다.
YCbCr데이터가 R, G, B 벡터로 변환될때 처리되며, R, G, B벡터는 DecVdata벡터에 RGB565방식으로 저장이 됩니다.
스풀링으로 인한 프레임드롭을 방지하기 위해 SRAM을 2구간으로 나누어 영상데이터를 번갈아 가며 저장하며, 최종적으로 vdata에 있는 영상정보를 SRAM에 저장하고 eamem_data를 통해 메모리주소를 eagle-MXU로 전송합니다.

FPGA보드에 프로그래밍이 안되는 경우 아래와 같은상황인지 확인하길 바랍니다.

1. FPGA 보드에 프로그램을 업로드하는경우 전원과 USB-Blaster만 필요합니다. RS232C나 mini-usb는 굳이 꽂을필요는 없습니다.

2. 컴파일하고 바로 programmer탭으로 업로드하면 안됩니다.매번 jic파일을 변경해야합니다.
 
  2-1. Convert Programming files에서 programming file type을 JTAG Indirect Configueration File(.jic)를 선택했는지 확인합니다.

  2-2. Device를 EPCS64로 변경했는지 확인합니다.

  2-3. Flash Loader Device 를 Cyclone IV E / EP4CE75로 변경하였는지 확인합니다.
 
  2-4. SOF Data를 SoC_Brain.sof로 변경하였는지 확인합니다.
  
  