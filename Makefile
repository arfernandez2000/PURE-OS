
all:  bootloader kernel userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean
pvs:
	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
	
format: 
	clang-format -style=file --sort-includes --Werror -i ./Kernel/*.c ./Kernel/include/*.h
	clang-format -style=file --sort-includes --Werror  ./Kernel/interruptions/*.c 
	clang-format -style=file --sort-includes --Werror  ./Kernel/drivers/*.c 
	clang-format -style=file --sort-includes --Werror  ./Kernel/ipc/*.c 
	clang-format -style=file --sort-includes --Werror  ./Kernel/memory_manager/*.c 
	clang-format -style=file --sort-includes --Werror -i ./Userland/SampleCodeModule/*.c ./Userland/SampleCodeModule/include/*.h
	clang-format -style=file --sort-includes --Werror  ./Userland/SampleCodeModule/shell/*.c 
	clang-format -style=file --sort-includes --Werror -i ./Userland/SampleCodeModule/shell/commands/*.c ./Userland/SampleCodeModule/shell/include/*.h
check:
	cppcheck --quiet --enable=all --force --inconclusive .
	
	

.PHONY: bootloader image collections kernel userland all clean
