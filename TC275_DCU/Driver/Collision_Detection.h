#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

// 충돌 감지 핀 정의
#define COLLISION_PIN IfxPort_P02_0

// 함수 프로토타입
void Collision_Detection_Init(void);   // 충돌 감지 초기화 함수
void Collision_ISR(void);
uint8 Is_Collision_Detected(void);
void Reset_Collision_Detected(void);

#endif // COLLISION_DETECTION_H
