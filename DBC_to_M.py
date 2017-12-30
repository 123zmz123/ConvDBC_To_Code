import re
import json
import pprint
from  pprint import pprint
import canmatrix
from  canmatrix import convert

class Frame():
    def __init__(self,fr_name,id):
        self.fr_name = fr_name
        self.id = id



class DBC_to_M():
    account = {}
    def __init__(self,dbc_path,m_path):
        self.dbc_path = str(dbc_path)
        self.m_path = str(m_path)

    def Read_DBC(self):
        with open(self.dbc_path) as f:
            self.dbc_file = f

    def parse_dbc(self):
        for line in self.dbc_file:
            mes = []
            l = line.strip()
            if l.__len__() == 0:
                continue
            if(l.startswith("SG_ ")):
                pass
            elif (l.startswith("BO_")):
                pattern = re.compile("BO\_ (\w+) (\w+) *: (\w+) (\w+)")
                regex = re.match(pattern)
                if regex:
                    frame_name = regex.group(2)
                    frame_id = regex.group(1)
                    frame_len = regex.group(3)
                    frame_io = regex.group(4)

class can_frame():
    sig_list = list()
    # sig_conv_list = list()
    sig_conv = {}
    def __init__(self):
        pass
    def set_id(self,id):
        self.id = id
    def set_name(self,name):
        self.name = name
    def add_signal(self,signal):
        self.sig_list.append(signal)
        self._parse_signal(signal)
    def _parse_signal(self,signal):
        start_bit = (signal["start_bit"] )
        signal_length = signal["bit_length"]
        Sbit_data_num, Sbit_bit_num = self._start_bit_position(start_bit)
        Ebit_data_num, Ebit_bit_num = self._end_bit_to_data_pos(Sbit_data_num,Sbit_bit_num,signal_length)
        #operation = self._parse_bits(Sbit_data_num,Sbit_bit_num,Ebit_data_num,Ebit_bit_num,signal["name"])
        operation = self._descripe_bits(Sbit_data_num,Sbit_bit_num,Ebit_data_num,Ebit_bit_num,signal["name"])
        #self.sig_conv[str(signal["name"])] = {"St_Bit": (Sbit_data_num,Sbit_bit_num), "End_Bit":(Ebit_data_num,Ebit_bit_num)}
        self.sig_conv[str(signal["name"])] = operation


    def _start_bit_position(self,start_bit):
        Sbit_bit_num = start_bit % 8
        Sbit_data_num = start_bit // 8
        return Sbit_data_num,Sbit_bit_num
    def _end_bit_to_data_pos(self,Sbit_data_num,Sbit_bit_num,data_length): # give the  data_length and data_position of each signal , calculate
        Ebit_data_num = Sbit_data_num
        Ebit_bit_num = Sbit_bit_num                                                                            #end_bit's position
        for count in range(data_length - 1):
            if Ebit_bit_num < 8:
                Ebit_bit_num = Ebit_bit_num + 1
            elif Ebit_bit_num == 8:
                Ebit_bit_num = 0
                if Ebit_data_num > 0:
                    Ebit_data_num = Ebit_data_num- 1
        return Ebit_data_num, Ebit_bit_num



    def _end_bits(self,start_bit,signal_length):
        pass

    def _parse_bits(self,Sbit_data_num,Sbit_bit_num,Ebit_data_num,Ebit_bit_num,sig_name):
        if Sbit_data_num - Ebit_data_num == 0:
            opera = "Data[{Sbit_data_num}]>>{Sbit_bit_num} = {name};\n".format(Sbit_data_num = Sbit_data_num, Sbit_bit_num = Sbit_bit_num,name = sig_name)
        else:
            opera = "Data[{Sbit_data_num}]>>{Sbit_bit_num} = {name};\n Data[{Ebit_data_num}]>>{Ebit_bit_num} = {name};\n".format(Sbit_data_num = Sbit_bit_num,
            Sbit_bit_num = Sbit_bit_num,name = sig_name,Ebit_data_num = Ebit_data_num,Ebit_bit_num=Ebit_bit_num)
        return opera

    def _descripe_bits(self,sbit_d_n,sbit_bit_n,ebit_d_n,ebit_b_n,sig_name):
        operation = "{name}: start -> Data[{s_d_n}] Bit[{s_b_n}]; end -> Data[{e_d_n}] Bit[{e_b_n}] ".format\
            (name = sig_name, s_d_n = sbit_d_n, s_b_n = sbit_bit_n, e_d_n = ebit_d_n, e_b_n = ebit_b_n)
        return  operation




    def set_type(self,flag):
        if flag == False:
            self.type = "Standard_ID"
        else:
            self.type = "Extend_ID"


    def _write_to_C_file(self,s_r_flag):
        with open("test.c") as f:
            if s_r_flag == 1:
                f.newlines("void BMS_"+str(self.name)+"_"+str(self.id)+"_"+"send(void)")
            else:
                f.newlines("void BMS_" + str(self.name) + "_" + str(self.id) + "_" + "receive(void)")
            f.newlines("{")
            f.newlines("Can_message_struct BMS_Tx;")
            f.newlines("BMS_Tx.ID = "+hex(int(self.id))+";")
            f.newlines("BMS_Tx.DLC = 8;")
            f.newlines("BMS_Tx.IDTYPE = " + self.type+";")
            for sig_names in self.sig_conv.keys():
                for bit_p in range(self.sig_conv[sig_names]["St_Bit"],self.sig_conv[sig_names]["End_Bit"] + 1):
                    f.newlines("Data[" + str(bit_p)+"] = " + str(sig_names) )
    def show_parse_to_c(self,s_r_flag):
        if s_r_flag == 1:
            print("void BMS_"+str(self.name)+"_"+str(self.id)+"_"+"send(void)")
        else:
            print("void BMS_" + str(self.name) + "_" + str(self.id) + "_" + "receive(void)")
        print("{")
        print("Can_message_struct BMS_Tx;")
        print("BMS_Tx.ID = " + hex(int(self.id)) + ";")
        print("BMS_Tx.DLC = 8;")
        print("BMS_Tx.IDTYPE = " + self.type + ";")
        #for sig_names in self.sig_conv.keys():
            #print(self.sig_conv[sig_names])
        for signal_name in self.sig_conv.keys():
            print(signal_name,self.sig_conv[signal_name])
            #Sdata, Sdata_offset = self.sig_conv[signal_name]["St_Bit"]
            #Edata, Edata_offset = self.sig_conv[signal_name]["End_Bit"]
            #print("Data[{Sdata}] << {Sdata_offset} = {signal_name} begin;".format(Sdata = Sdata,Sdata_offset = Sdata_offset, signal_name = signal_name))
            #print("Data[{Edata}] << {Edata_offset} = {signal_name} end;".format(Edata=Edata, Edata_offset=Edata_offset,signal_name=signal_name))

    def Write_to_M_file(self):
        pass






def parse_dbc():
    frame = can_frame()
    convert.convert("test.dbc","dd.json")
    f = open("dd.json")
    data = json.load(f)
    l = data["messages"]
    for each_frame in l: # read each frame in json file
        frame.set_id(each_frame["id"]) # frame is can_frame object
        frame.set_name(each_frame["name"])
        frame.set_type(each_frame["is_extended_frame"])
        for signal in each_frame["signals"]:
            frame.add_signal(signal)
        frame.show_parse_to_c(1)
        frame.sig_list.clear()
        frame.sig_conv.clear()


def test_endbit_position(St_bit_in_line, St_bit_at_data,data_length):
    Ed_bit_in_line = St_bit_in_line
    Ed_bit_at_data = St_bit_at_data
    for count in range(data_length - 1):
        if Ed_bit_in_line < 7:
            Ed_bit_in_line = Ed_bit_in_line + 1
        else:
            Ed_bit_in_line = 0
            Ed_bit_at_data = Ed_bit_at_data - 1

    return  Ed_bit_at_data, Ed_bit_in_line













if __name__ == "__main__":
    parse_dbc()
















