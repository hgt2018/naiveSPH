# stl模型导入(ascii格式)

file_name = "model.stl"
output_name = "model.txt"

model_text=""
output_text=[]
with open(file_name,'r') as f:
    model_text = f.read() # 读取全部信息

model_lines = model_text.split("\n")
line_num = len(model_lines)
index = 1
while(index+7 <line_num):
    norm_str = model_lines[index].split()[2:]
    v1_str = model_lines[index+2].split()[1:]
    v2_str = model_lines[index+3].split()[1:]
    v3_str = model_lines[index+4].split()[1:]
    x1, y1, z1 = float(v1_str[0]), float(v1_str[1]), float(v1_str[2]) 
    x2, y2, z2 = float(v2_str[0]), float(v2_str[1]), float(v2_str[2]) 
    x3, y3, z3 = float(v3_str[0]), float(v3_str[1]), float(v3_str[2]) 
    x = (x1+x2+x3)/3.
    y = (y1+y2+y3)/3.
    z = (z1+z2+z3)/3.
    # output_line = "{:.6f} {:.6f} {:.6f} {:s} {:s} {:s}\n",format(x,y,z,norm_str[0],norm_str[1],norm_str[2])
    # output_line = "%.6f %.6f %.6f %s %s %s\n",%(x,y,z,norm_str[0],norm_str[1],norm_str[2])
    output_text.append("%.6f %.6f %.6f %s %s %s\n"%(x,y,z,norm_str[0],norm_str[1],norm_str[2]))
    index += 7
    
with open(output_name,"w") as f:
    f.writelines(output_text)
