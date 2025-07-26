
obj-m += pci_demo_driver.o

all:
	# 使用make命令调用内核的构建系统
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean