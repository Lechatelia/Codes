添加人：Lechatelia
注意每次添加配置文件时，注意
1，需要添加文件的路径，如果之前没添加过
2，在工程project栏里面添加相应的c文件
3，如出现\OBJ\test.axf: Error: L6218E: Undefined symbol WFI_SET (referred from sys.o) 一般是左侧没有添加该函数所在相应的c文件
4，在FWLIB里面默认的很多外设没有添加，按照上一点，需要自己添加
branch master

