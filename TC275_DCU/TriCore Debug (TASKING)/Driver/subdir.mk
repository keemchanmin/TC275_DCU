################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Driver/Asclin1.c" \
"../Driver/Asclin2.c" \
"../Driver/Asclin3.c" \
"../Driver/Can.c" \
"../Driver/Collision_Detection.c" \
"../Driver/Mp3.c" \
"../Driver/Rfid.c" \
"../Driver/Step_Motor.c" \
"../Driver/Ultrasonic.c" 

COMPILED_SRCS += \
"Driver/Asclin1.src" \
"Driver/Asclin2.src" \
"Driver/Asclin3.src" \
"Driver/Can.src" \
"Driver/Collision_Detection.src" \
"Driver/Mp3.src" \
"Driver/Rfid.src" \
"Driver/Step_Motor.src" \
"Driver/Ultrasonic.src" 

C_DEPS += \
"./Driver/Asclin1.d" \
"./Driver/Asclin2.d" \
"./Driver/Asclin3.d" \
"./Driver/Can.d" \
"./Driver/Collision_Detection.d" \
"./Driver/Mp3.d" \
"./Driver/Rfid.d" \
"./Driver/Step_Motor.d" \
"./Driver/Ultrasonic.d" 

OBJS += \
"Driver/Asclin1.o" \
"Driver/Asclin2.o" \
"Driver/Asclin3.o" \
"Driver/Can.o" \
"Driver/Collision_Detection.o" \
"Driver/Mp3.o" \
"Driver/Rfid.o" \
"Driver/Step_Motor.o" \
"Driver/Ultrasonic.o" 


# Each subdirectory must supply rules for building sources it contributes
"Driver/Asclin1.src":"../Driver/Asclin1.c" "Driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/A/autoever_aurix/TC275_DCU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver/Asclin1.o":"Driver/Asclin1.src" "Driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver/Asclin2.src":"../Driver/Asclin2.c" "Driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/A/autoever_aurix/TC275_DCU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver/Asclin2.o":"Driver/Asclin2.src" "Driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver/Asclin3.src":"../Driver/Asclin3.c" "Driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/A/autoever_aurix/TC275_DCU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver/Asclin3.o":"Driver/Asclin3.src" "Driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver/Can.src":"../Driver/Can.c" "Driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/A/autoever_aurix/TC275_DCU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver/Can.o":"Driver/Can.src" "Driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver/Collision_Detection.src":"../Driver/Collision_Detection.c" "Driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/A/autoever_aurix/TC275_DCU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver/Collision_Detection.o":"Driver/Collision_Detection.src" "Driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver/Mp3.src":"../Driver/Mp3.c" "Driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/A/autoever_aurix/TC275_DCU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver/Mp3.o":"Driver/Mp3.src" "Driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver/Rfid.src":"../Driver/Rfid.c" "Driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/A/autoever_aurix/TC275_DCU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver/Rfid.o":"Driver/Rfid.src" "Driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver/Step_Motor.src":"../Driver/Step_Motor.c" "Driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/A/autoever_aurix/TC275_DCU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver/Step_Motor.o":"Driver/Step_Motor.src" "Driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Driver/Ultrasonic.src":"../Driver/Ultrasonic.c" "Driver/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/A/autoever_aurix/TC275_DCU/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<"
"Driver/Ultrasonic.o":"Driver/Ultrasonic.src" "Driver/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Driver

clean-Driver:
	-$(RM) ./Driver/Asclin1.d ./Driver/Asclin1.o ./Driver/Asclin1.src ./Driver/Asclin2.d ./Driver/Asclin2.o ./Driver/Asclin2.src ./Driver/Asclin3.d ./Driver/Asclin3.o ./Driver/Asclin3.src ./Driver/Can.d ./Driver/Can.o ./Driver/Can.src ./Driver/Collision_Detection.d ./Driver/Collision_Detection.o ./Driver/Collision_Detection.src ./Driver/Mp3.d ./Driver/Mp3.o ./Driver/Mp3.src ./Driver/Rfid.d ./Driver/Rfid.o ./Driver/Rfid.src ./Driver/Step_Motor.d ./Driver/Step_Motor.o ./Driver/Step_Motor.src ./Driver/Ultrasonic.d ./Driver/Ultrasonic.o ./Driver/Ultrasonic.src

.PHONY: clean-Driver

