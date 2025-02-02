#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

typedef void (*event_ultrasonic)(uint32);

// 함수 프로토타입
void ultrasonic_init(void);      // GPIO 초기화 함수
void ultrasonic_event_10us();
void ultrasonic_trigger();
void ultrasonic_register_distance_event(event_ultrasonic func);
void Echo_ISR(void);       // Echo 신호 인터럽트 처리 함수
int Display_Distance(void);

#endif // ULTRASONIC_SENSOR_H
