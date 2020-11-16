ifneq ($(KERNELRELEASE),)
	testmodule-objs := sample_module.o kerlib.o
	obj-m := testmodule.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

MODULE := testmodule.ko

.PHONY: clean
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean