import crcmod
if __name__ == '__main__':
    crc_array = [0x01,0x84,0x03]#0x03 0x01
    ret = [0x00,0x00]
    #Name Identifier-name, Poly  Reverse Init-value XOR-out Check
    # ['modbus','CrcModbus',0x18005,REVERSE,0xFFFF,0x0000,0x4B37]
    crc16 = crcmod.mkCrcFun(0x18005, rev=True, initCrc=0xFFFF, xorOut=0x0000)
    crc_calc = crc16(bytes(crc_array)) #计算得到的CRC
    ret[1]=(int)(crc_calc/256)
    ret[0]=crc_calc%256
    print(ret)