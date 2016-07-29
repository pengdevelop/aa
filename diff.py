import os

dirname1 = "D:\\vx_694\\694\\vxtest\\src\\tests\\int"
dirname2 = "D:\\proj\\Test_Date\\ktos_code\\ktos\\ktos\\ktos\\tests\\vxmulti\\suite\\tests\\int"

for item2 in os.listdir(dirname2):
    count = 0
    for item1 in os.listdir(dirname1):
        if (item1 == item2):
            print item1
    