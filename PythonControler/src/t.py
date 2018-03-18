import numpy as np
a = 'NL,NS,NM'
c = a.split(',')
dic = {'NL':0,'NM':1,'NS':2,'ZE':3,'PS':4,'PM':5,'PL':6}
print(dic[c[1]])

a = [1,2,3,4,5,6,7,8]
b =np.array(a).reshape( 2,-1)
print(b[0])
print(b[0][2:])

data = bytearray(3)
data[0] = 31
data[1] = 32
data = bytearray('abcdefg', encoding='utf-8')
print(data)
print(data.decode(encoding='utf-8'))
 
