#### [1. c, h 파일 분리 사용자 라이브러리 작성법][https://driz2le.tistory.com/58]

![image](https://user-images.githubusercontent.com/18729679/116334140-b172d200-a80f-11eb-8ef6-b2763a6c2388.png)

![image](https://user-images.githubusercontent.com/18729679/116334265-dff0ad00-a80f-11eb-933e-a237a3609afe.png)

> 다음과 같이 기존의 ino파일에 있던 내용을 c에서 작성하여 선언한 후 ino에서 실행시키는 방법으로 진행 할 수 있다.



![image](https://user-images.githubusercontent.com/18729679/116673663-a3bf7700-a9de-11eb-9c74-8f002e5b9a16.png)

- 상단에 c로 선언한 파일에서는 기존의 ino와 다르게 Arduino.h 파일을 include 해줘야 한다.

![image](https://user-images.githubusercontent.com/18729679/116673885-e2edc800-a9de-11eb-8ad2-60d00cd9ee67.png)



#### 2. [함수 포인터 구조 작성법](https://boycoding.tistory.com/233)

- 다음 구조를 참조해 작성하면 된다.

  ```c
  int foo() { 
      return 5; 
  } 
  int goo() {
      return 6; 
  } 
  
  int main() {
      int (*fcnPtr)() = foo; // fcnPtr points to function foo 
      fcnPtr = goo; // fcnPtr now points to function goo 
      
      return 0; 
  }
  ```

> 위 코드에서 *fcnPtr*은 인수가 없고 정수를 반환하는 함수에 대한 포인터다.(즉, 함수 포인터) 그러므로 이 타입과 같은 함수를 가리킬 수 있다. 괄호가 필요한 이유는`int* fcnPtr()`과 같은 코드는 정수에 대한 포인터를 반환하는 인수가 없는 함수 fcnPtr의 전방 선언으로 해석되기 때문에 우선순위를 위해서다.

- 추가적으로 상수 함수 포인터를 만들기 위해서는 *뒤에 const 키워드를 사용하면 된다.

```c
int (*const fcnPtr)();
```

- 함수 포인터에 함수 호출 법은 다음과 같다.

```c
int foo(int x) { 
    return x; 
} 

int main() { 
    int (*fcnPtr)(int) = foo; // fcnPtr에 foo함수를 할당한다. 
    (*fcnPtr)(5); // foo(5)를 fcnPtr로 호출한다. 
    return 0; 
}

```

> 보통은 `fcnPtr(5);` 처럼 암시적 역참조를 통해 구현하는게 보통 방법이다.

- 다른 함수의 인자로 함수 전달(콜백)

> 함수 포인터의 최대 장점으로 사용자가 원하는 함수를 변수로 넣으며 호출 할 수 있다.
>
> 아래 코드는 다음과 같이 comparisonFcn을 통해 오름차순, 내림차순을 대입하는 코드이다.

 ```c
void SelectionSort(int* array, int size, bool (*comparisonFcn)(int, int)) { 
    for (int i = 0; i < size; ++i) { 
        int smallestIndex = i; 
        for (int j = i + 1; j < size; ++j) {
            if (comparisonFcn(array[sammlestIndex], array[j])) // 비교 부분 
                smallestIndex = j; 
        } 
        
        std::swap(array[smallestIndex], array[j]); 
    } 
} 

bool ascending(int x, int y) { 
    return x > y; // 오름차순 
} 

bool descending(int x, int y) { 
    return x < y; // 내림차순 
} 

void printArray(int *array, int size) { 
    for (int index=0; index < size; ++index) 
        std::cout << array[index] << " "; 
    std::cout << '\n'; 
} 

int main() { 
    int array[9] = { 3, 7, 9, 5, 6, 1, 8, 2, 4 }; // 내린차순 선택정렬
    selectionSort(array, 9, descending); 
    printArray(array, 9); // 오름차순 선택정렬 
    selectionSort(array, 9, ascending); 
    printArray(array, 9); 
    return 0; 
} 

//출력
9 8 7 6 5 4 3 2 1 
1 2 3 4 5 6 7 8 9

 ```

#### 3. 전역변수의 static은 파일 간의 전역변수를 의미한다.

- 먼저 기본적으로 아래와 같이 선언하면 int는 전역변수가 된다.

```c
#include <stdio.h>

//전역변수 선언
int data = 0;

int main(){
    printf("%d", data);
}

```

> 위와 같이 선언된 변수는 전역으로서 다른 c파일 내부에서도 똑같이 int data를 선언하면 이전에 선언되어 있는 data를 참조해 오류가 발생한다.
>
> 즉, 다른 파일의 함수 또는 전역에서 해당 변수 값을 참조하게 된다.



- 반면에 아래와 같이 선언한 int 변수는 전역변수이자 정적변수가 된다.

```c
#include <stdio.h>

//전역변수에 정적변수 선언(같은 파일에서만 참조가능한 전역변수)
static int data = 0;

int main(){
    printf("%d", data);
}

```


> 기본 static(정적변수)는
>
> 1. while 함수 내에서도 한번만 초기화 된다.
>
> ```c
> for(int i=0; i<10; i++){
>     static int value = 1;
>     printf("%d ", value);
>     value ++;
> }
> 
> //출력 값은 1 2 3 4 5 6 7 8 9 10이 된다.
> //static은 한번만 실행된 다음 없어지는 줄이라 생각하면 된다.
> ```
>
> 2. 프로그램이 종료될 때 까지 메모리에 남아있다. 즉, 저 while 문이 종료되어도 참조가 가능하다.



- 이런 static을 위와 같이 전역변수 형태의 변수에 static을 선언하게 되면 다음과 같은 이점이 있다.

> #### 파일 단위의 지역변수 같은 의미가 된다. (해당 파일에서만 사용가능한 변수가 된다.) => 객체를 파일 별로 코드 작성하여 무결성 보장
>
> 예를 들어 tmp1.c 파일과 tmp2.c 파일이 있는데 두 파일 모두에 똑같이 전역변수에 static int value를 선언하였다고 하자.
>
> 그렇다면 tmp3.c에서는 해당 tmp1.c 또는 tmp2.c에 있는 같은 이름의 변수를 선언할 수 있다 => 보통 프로그래머는 get, set을 통해 다른파일에 접근하곤 하는데, 만약 전역 정적변수가 아닌 전역 변수였다면, 같은 이름의 변수가 있던 것을 모르고, 또는 잘못 접근하고 있던 점을 모르고 컴파일 오류 없이 작동되어 잘못 참조되어 있던 것을 모를 가능성이 크다.
>
> 
>
> 이게 필요한 이유는 비슷한 구조를 가진  객체를 파일로 분리하면 같은 변수를 쓰는게 편할 때가 있기 때문이다.
>
> 
>
> 예를 들면 강아지 별로 c파일을 분리해 puppy 파일엔 int puppy_name, puppy_weight, puppy_age 를 선언하고, booldog파일엔 int booldog_name, booldog_weight,  booldog_age 로 선언할 수 있지만 그렇게 되면 강아지가 30마리면 30마리 별로 개별적인 변수 이름과 구조를 지어줘야 한다.
>
> 심지어 저 위의 경우는 name, weight, age 세 가지지만 개의 종류에 따라 특정 개에는 name, weight, age, ear_color로 만들어줘야하고, 특정 개에는 name, weight, age, tail, tail_color 등 여러 가지로 분리해서 구조체를 작성해야한다면 이전의 name, weight, age 구조를 그대로 다른 파일의 값에서 가져오는게 당연히 편할 것이다. 
>
> 
>
> 따라서 보통의 프로그래머는 puppy.c, booldog.c파일에 다음과 같이 선언한 구조를 h에서 가져와 분리해 선언한다.
>
> ```c
> //dog.h
> 
> typedef struct puppy_dog_t{
>  int name;
>  int weight;
>  int age;
> }PUPPY_DOG;
> 
> typedef struct booldog_dog_t{
>  int name;
>  int weight;
>  int age;
> }BOOL_DOG;
> ```
>
> ```c
> //puppy1.c
> 
> PUPPY_DOG puppy{1,5,3};
> ```
>
> ```c
> //puppy2.c
> PUPPY_DOG puppy{2,4,3};
> ```
>
> 
>
>
> ```c
> //booldog1.c
> BOOL_DOG booldog{2,15,6};
> ```
>
> 이렇게 선언 된 구조가 전역 정적 구조체가 아닌 전역변수로 선언되어 같은 puppy1, puppy2 처럼 같은 종류의 다른 강아지의  name, weight, age를 main.c에서 가져온다면 둘 중에 어떻게 같은 강아지의 같은 변수의 강아지를 가져올 수 있을까?
>
> 또 puppy파일 별로 `PUPPY_DOG puppy1`, `PUPPY_DOG puppy2`로 불편하게 선언해줘야하는걸까? 만약 puppy1은 사고로 꼬리가 없어서 구조체 변수를 따로 선언해야한다면 모든 구조체의 get, set 함수들을 수정해야하는건가 하는 불편성이 존재한다.
>
> 
>
> 답은 전역변수를 static으로 선언해주고 puppy1 파일의 get, set만 변경해 main에서는 get, set을 함수포인터로 가져오면 둘의 같은 이름의 객체를 부르고, 같은 get, set을 코드로 실행시켜도 다른 실행을 할 수 있다. 즉, 변수를 변경하는 것을 모두 다 할 필요가 없다.
>
> 
>
> 또한 전역 변수를 참조하여 생기는 기타 오류를 찾는데 몇 천, 만줄을 분석하고 찾아볼 가능성을 없앨 수 있고, 같은 객체를 계속해서 분리하고 또 따로 설계하는데 변수 네이밍을 다시 또 바꿔 선언해야하는 문제를 해결할 수 있다.
>
> 
>
> 즉, 프로그래머가 파일 별로 개별 객체를 분류하고, 이를 맨위에 전역 정적변수로 선언하면 이는 파일 간 안전성을 보장하며, 다른 파일에서 해당 파일의 변수를 참조해야할 경우 get, set을 통해 접근하는 방식으로 코드를 짜는데 무결성을 보장할 수 있다.



#### 4. 기본적으로 class와 struct의 별 차이는 없지만 세 가지가 있다.

1. 맨 위에 struct를 선언하면 전역변수이지만 class는 기본적으로 static 구조이다. 즉,  전역으로 선언되지 않는다.
2. 처음 선언한 class는 기본적으로 private로 같은 클래스 내에서만 사용할 수 있고, 바깥에서는 접근이 불가능하다. => class A 내부에 int a=10;이 있다면 main 함수에서 printf("%d", A.a)가 불가능하다.
3. 만약 class를 public으로 선언하면 static struct와 차이가 없다.



#### 5. typedef struct의 구조체 타입 네이밍은 생략 가능하다.

```c
//서로 같은 의미이다.
typedef struct data_t{
    int a;
    int b;
}DATA;

typedef struct{
    int a;
    int b;
}DATA;

```

#### 6. DMA

- DMA controller라는 주변 장치가 직접 메모리에 접근하여 값을 읽거나 쓸 수 있다.

  > C 표준 라이브러리 함수 중에 memcpy()라는 것이 있다.  이 함수는 source address가 가리키는 주소에서 데이터를 읽어 destination address 주소가 가리키는 곳에 주어진 크기만큼 데이터를 복사해주는 동작을 수행한다.  매우 단순한 동작인데, 복사하고자 하는 크기가 작을 때는 별 문제가 없지만, 만약 그 크기가 커지게 되면 그만큼 많은 시간이 필요하게 된다.

  > 임베디드 시스템에 사용되는 대부분의 CPU는 RISC 구조를 가지고 있는데, RISC의 대표적 특징은 모든 동작이 load, store 방식이란 점이다.
  >
  > 즉,  100바이트의 데이터를 복사하기 위해서는 100번의 load, store 동작을 수행하게 되고, 100번이 수행되었는지 확인하기 위하여 카운트 기능과 비교 구문도 같이 수행할 수 밖에 없게 된다.
  >
  > 
  >
  > 이러한 단순 메모리 복사 기능을 CPU가 수행하는 것이 아닌, 이러한 기능만 수행하는 주변 장치를 따로 만들어서 메모리 복사를 담당하고, CPU는 다른 동작을 수행하는 것이 효율이 높다.
  >
  > 
  >
  > 예를 들어, 9600bps로 설정된 UART를 통해서 32바이트 데이터를 송신하기 위한 시간을 계산해 보면 그 시간동안 CPU가 얼마만큼이나 많은 명령어를 처리할 수 있는지 놀랄것이다.  그러므로, UART 송신 기능을 DMA에게 처리하도록 하고, CPU는 그 시간동안 다른 동작을 수행하면 되는것이다.
  >
  > 출처: https://embed-avr.tistory.com/130 [AVR로 시작하는 임베디드]



#### 7. Feather m0(SAMD21G)의 경우 아두이노의 #include <SoftwareSerial.h> 을 사용할 수 없다.

- 핀 맵과 지원 구조가 다르기 때문에 해당 라이브러리를 사용할 수 없다.

- [따라서 다음 내용에 있는 sercom을 통해 추가 통신을 확장할 수 있다.](https://velog.io/@wimes/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8-M0-SAMD21G-%EC%97%90%EC%84%9C-%EC%83%88%EB%A1%9C%EC%9A%B4-serial-%EB%A7%8C%EB%93%A4%EA%B8%B0-qfk36n99m7)

  

#### 8. servo PWM 역상 코드 위치

> C:\Users\HJKim\Documents\Arduino\libraries\Servo\src\samd\servo.cpp

```c++
//핸들러의 HIGH LOW 부분을 반대로 변경하면 PWM이 역상으로 출력된다.


void Servo_Handler(timer16_Sequence_t timer, Tc *tc, uint8_t channel, uint8_t intFlag)
{
    if (currentServoIndex[timer] < 0) {
        tc->COUNT16.COUNT.reg = (uint16_t) 0;
        WAIT_TC16_REGS_SYNC(tc)
    } else {
        if (SERVO_INDEX(timer, currentServoIndex[timer]) < ServoCount && SERVO(timer, currentServoIndex[timer]).Pin.isActive == true) {
            //여기 부분(본래 코드는 LOW)
            digitalWrite(SERVO(timer, currentServoIndex[timer]).Pin.nbr, HIGH);   // pulse this channel low if activated
        }
    }

    // Select the next servo controlled by this timer
    currentServoIndex[timer]++;

    if (SERVO_INDEX(timer, currentServoIndex[timer]) < ServoCount && currentServoIndex[timer] < SERVOS_PER_TIMER) {
        if (SERVO(timer, currentServoIndex[timer]).Pin.isActive == true) {   // check if activated
            //여기 부분(본래 코드는 HIGH)
            digitalWrite(SERVO(timer, currentServoIndex[timer]).Pin.nbr, LOW);   // it's an active channel so pulse it high
        }

        // Get the counter value
        uint16_t tcCounterValue = tc->COUNT16.COUNT.reg;
        WAIT_TC16_REGS_SYNC(tc)

        tc->COUNT16.CC[channel].reg = (uint16_t) (tcCounterValue + SERVO(timer, currentServoIndex[timer]).ticks);
        WAIT_TC16_REGS_SYNC(tc)
    }
    else {
        // finished all channels so wait for the refresh period to expire before starting over

        // Get the counter value
        uint16_t tcCounterValue = tc->COUNT16.COUNT.reg;
        WAIT_TC16_REGS_SYNC(tc)

        if (tcCounterValue + 4UL < usToTicks(REFRESH_INTERVAL)) {   // allow a few ticks to ensure the next OCR1A not missed
            tc->COUNT16.CC[channel].reg = (uint16_t) usToTicks(REFRESH_INTERVAL);
        }
        else {
            tc->COUNT16.CC[channel].reg = (uint16_t) (tcCounterValue + 4UL);   // at least REFRESH_INTERVAL has elapsed
        }
        WAIT_TC16_REGS_SYNC(tc)

        currentServoIndex[timer] = -1;   // this will get incremented at the end of the refresh period to start again at the first channel
    }

    // Clear the interrupt
    tc->COUNT16.INTFLAG.reg = intFlag;
}

```

#### 9. pin 번호

- 현재 사용하고 있는 칩 네임은 ATSAMD21G18A로 다음 데이터 시트를 참조하면 확인할 수 있다.(https://www.microchip.com/wwwproducts/en/ATsamd21g18)
- 이 중 아두이노 IDE를 사용하기 위해 해당 칩에 부트로더를 설치해야한다. 특히 ATSAMD21G를 사용하는 보드 중에 Feather m0 보드 IDE를 사용할 것임으로 다음 사이트에서 부트로더를 설치하는 방법을 참조한다. (https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le?view=all&gclid=EAIaIQobChMI4YKuw-Sa8AIVRthMAh37hADmEAAYASAAEgJI9vD_BwE#setup)



#### 9. buzzer

- 간단하게 tone 명령어를 사용해 진행할 수 있다.

  ```c++
  //해당 핀번호에 몇 hz를 몇 time만큼 소리나게 인가한다.
  tone(int pin_num, int hz, int time);
  ```

  - tone 명령어의 경우 한번 진행 한 후 busy 상태로 수행하는게 아니라 다음 동작으로 넘어가기에 delay를 해줘야한다.

  ```c++
  	tone(PARAMETER_BUZZER_PIN, 880, 100);
  	//위의 880만큼의 hz를 동작 시킨후 100만큼 delay를 수행하지 않으면 바로 1760hz의 동작을 시작하므로 880이 진행되지 않는 것처럼 느껴진다.
      delay(100);
      tone(PARAMETER_BUZZER_PIN, 1760, 100);
  ```

  

#### 10. step motor

- HighPowerStepperDriver pololu사를 사용하였다.

  ```c++
  #include <SPI.h>
  #include <HighPowerStepperDriver.h>
  
  const uint8_t CSPin = 4;
  
  // This period is the length of the delay between steps, which controls the
  // stepper motor's speed.  You can increase the delay to make the stepper motor
  // go slower.  If you decrease the delay, the stepper motor will go faster, but
  // there is a limit to how fast it can go before it starts missing steps.
  const uint16_t StepPeriodUs = 2000;
  
  HighPowerStepperDriver sd;
  
  void setup()
  {
    SPI.begin();
    sd.setChipSelectPin(CSPin);
  
    // Give the driver some time to power up.
    delay(1);
  
    // Reset the driver to its default settings and clear latched status
    // conditions.
    sd.resetSettings();
    sd.clearStatus();
  
    // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
    // for most applications, and we find that it usually works well.
    sd.setDecayMode(HPSDDecayMode::AutoMixed);
  
    // Set the current limit. You should change the number here to an appropriate
    // value for your particular system.
    sd.setCurrentMilliamps36v4(1000);
  
    // Set the number of microsteps that correspond to one full step.
    sd.setStepMode(HPSDStepMode::MicroStep32);
  
    // Enable the motor outputs.
    sd.enableDriver();
  }
  
  void loop()
  {
    // Step in the default direction 1000 times.
    sd.setDirection(0);
    for(unsigned int x = 0; x < 1000; x++)
    {
      sd.step();
      delayMicroseconds(StepPeriodUs);
    }
  
    // Wait for 300 ms.
    delay(300);
  
    // Step in the other direction 1000 times.
    sd.setDirection(1);
    for(unsigned int x = 0; x < 1000; x++)
    {
      sd.step();
      delayMicroseconds(StepPeriodUs);
    }
  
    // Wait for 300 ms.
    delay(300);
  }
  ```

  

> 코드 설명은 다음과 같다.
>
> 1. `SPI.begin();` : spi 통신을 통해 모터를 제어하게 되는데 spi 통신을 위한 초기 세팅을 시작한다.
>
> 2. ` sd.setChipSelectPin(CSPin);`모터 드라이버 코드 내용을 살펴보면 몇번 핀의 드라이버를 사용할지 선택할 수 있다. SPI 통신이므로 CSPin을 통해 해당 핀에 데이터 전송한다.
>
> 3. `sd.resetSettings();`,`sd.clearStatus(); `:  모터 세팅 초기화 및 클리어
>
> 4. `sd.setStepMode()`: mode 별로 Slow Fast, Automixed 등의 방법이 있다.
>
> 5. `sd.setCurrentMilliamps36v4(1000);`: 모터 전류의 양을 제어 => 모터 회전 속도가 높을 수록 이 값이 높아야한다. 작성한 코드는 3000까지 높였다.
>
> 6. `sd.setStepMode(HPSDStepMode::MicroStep32);`: 모터 한 스텝 당 회전 각을 조율한다.
>
>    > 데이터 시트를 보면 1~256의 MicroStep을 제공한다.
>    >
>    > 모터 자체에서의 한 스텝은 1.8도 인데 MicroStep32으로 세팅하면 한 스텝의 각도가 1.8/32가 된다. 즉, 한 바퀴를 돌기 위해선 MicroStep1이 200 스텝 돌아야 한바퀴인 360도가 되고,
>    >
>    > MicroStep32의 경우 200*32 = 6400 스텝을 돌아야 한 바퀴가 된다.
>
> 7.   `sd.enableDriver();`: 모터 작동 모드를 한다. 만약 `sd.disableDriver()`로 하면 모터에 명령어를 줘도 작동하지 않는다.
>
> 8.   `sd.setDirection(0);`: 모터 방향 설정. 0이면 정방향, 1이면 역방향이다.
>
> 9.   `sd.step();`: 모터 한 스텝을 도는 명령이다. 만약 200 스텝을 돌고 싶다면 for문으로 200번 실행하면 된다.
>
> 10.    `delayMicroseconds(StepPeriodUs);`: 모터 속도 제어는 다음의 delay로 결정된다. 속도를 빠르게 하고 싶으면 delay의 값을 조금 주고, 느리게 하고 싶으면 많이 주면 된다. 하지만 전류의 양에 따라 delay를 조금 줘도 작동이 안될 때가 있다. 따라서 모터 속도가 일정 범위에서 빨라지지 않는다면 위의 5번을 통해 전류의 양을 증가시키자.

#### 11. [스텝모터 특징](http://wethes.inpiad.co.kr/index/bbs/board.php?bo_table=comm04&wr_id=8)

> **스테핑모터**는 특성이 한 스텝을 이동할 때마다 제자리를 찾는 과정에서 __더갔다가 (overshoot) 조금 덜(undershoot) 갔다가를__ 반복 하면서 그 한스텝의 자리를 찾기 위한 진동이 있다.
> 이 진동이 __아주 저속 일 때는 진동의 모음으로 모터가 심하게 진동과 소음을 발생__하는 것을 느낄수 있다.
> 상대적으로 DC모터 같은 종류는 조용한데 문제는 이 진동이 특정 회전(주파수)에 도달하게되면 진동들이 중첩이 되면서 진동을 키우는 방향으로 가게 된다. 이런 현상을 공진 이라고 하는데 진동이 커지다보면 특정 주파수에서는 아예 회전 불능 상태에 빠질 때 가 있다.
> 마치 모터가 정회전을 할까 역회전을 할까 갈피를 못잡는 경우인데 스테핑 모터를 처음 경험하게되면 이런 현상이 정상인지 문제인지를 고민 하게된다.
> 
> __________________________________________________________________________________
> 이런 통제 불능의 상태를 극복하기 위해 드라이버에서 다음과 같은 방법을 사용한다.
> 
>  1.공진 주파수를 피해서 운전.
>  가속 또는 감속을 해서 빨리지나 치거나 아예 이 주파수로 운전을 안 하거나 해서 회피 한다.
>그러나 모든 경우에 이 방법을 적용 할 수는 없다.
> 
> 2.부하가 이 진동을 흡수 할 수 있는 구조로 설계.
>무부하 상태가 가장 나쁘다고 볼 수 있으며 적절히 마찰과 관성이 있어야 한다.
> 모터회사에서 파는 뎀퍼 라는 물건이 이런 역할을 한다.
> 
> __3.모터의 회전각을 쪼개서 진동을 줄이는 방법__.
>마이크로 스텝 드라이버 라고 하는데. 한 스텝을 잘개 쪼개서 스텝마다의 진동을 작게 나타나게 한다. 스텝이 작아진 만큼 진동도 줄어 든다.
> 요즘에는 이런 기능을 하는 드라이버 IC가 많이 나와 있는데 시중에는 한 스텝을 512분주내외로 분할한 제품이 판매 되고 있다.
> 
> 4.진동(회전자의 위치)을 감지해서 진동에 준하게 상전류를 조절하면서 진동을 줄이는 방법.
>출력단의 전류를 관찰하면 모터의 진동이랄까 회전자의 위치를 알아낼 수 있는데 이것으로 스텝이 진행됨에 따라 상전류를 조절하면 공진이 현상이 현저히 줄어든다.
> 
> 5.모터에 엔코더를 부착하여 제어.
> 회전자의 위치를 정확히 파악하여 위치에 따라 상전류를 조절한다.
> 이 방법이 가장 우수한 성능을 낼수 있지만 만들기에는 가장 어려운 방법이다.

- 즉, 모터 동작 와중에 소음이 크거나, 진동이 많고 스텝을 이동하는데 가끔씩 역방향을 돌기도 하고, 진행도 하지 않는다면 그것은 overshoot, undershoot의 영향이다.
- 따라서, 기존의 방법이였던`sd.setStepMode(HPSDStepMode::MicroStep32);`를 통해 각도를 제어하면 된다.

#### 12. 모터 드라이버의 경우 Pololu High-Power Stepper Motor Driver 36v4를 사용하였다.

- 설명은 자제하고 다음 위키를 참조하면 많은 정보를 알 수 있다.(https://www.pololu.com/product/3730)

#### 13. 칩의 경우 ATSAMD21G18A를 사용하였다.

- 이것도 데이터 시트를 참조하는 것이 빠르다.(https://ww1.microchip.com/downloads/en/DeviceDoc/SAM-D21DA1-Family-Data-Sheet-DS40001882G.pdf)

#### 14. ATSAMD21G18A 칩에 아두이노를 사용하기 위해선 부트로더를 다운받아 업데이트 해줘야한다.

- 이것도 다음 사이트에 같은 칩에 다른 보드 아두이노 제로에 Feather-m0 부트 로더를 넣는 방법이 있다.(https://learn.adafruit.com/proper-step-debugging-atsamd21-arduino-zero-m0/restoring-bootloader)

- 다음 사이트에 들어가서 부트로더 헥사파일을 다운 후 J-link mini EDU를 사용해 부트로더를 넣으면 된다.

- 핀 번호의 경우 가지고 있는 J-link에 mini EDU를 관련 사이트를 참조하면 확인할 수 있다.(https://www.segger.com/products/debug-probes/j-link/models/j-link-edu-mini/)

  > SWDIO, SWDCLK, nRST, GND, 3V3 순으로 핀을 업로드하는데 필요하다.
  >
  > ![image-20210525110024233](C:\Users\HJKim\AppData\Roaming\Typora\typora-user-images\image-20210525110024233.png)
  >
  > 이 그림을 보고 연결하면 다음과 같은 사진이 된다. (실제 연결은 바닥을 보고 해야하기 때문에 상하 대칭이다. => 뒤집어서 해야함)

![image-20210525110357027](C:\Users\HJKim\AppData\Roaming\Typora\typora-user-images\image-20210525110357027.png)

![image](https://user-images.githubusercontent.com/18729679/119428730-6e414b80-bd48-11eb-9a1b-3649dba9cf10.png)

#### 15. Serial 포트 추가하는 법

- 아두이노에서는 SoftwareSerial을 통해 통신포트를 간편히 확장할 수 있지만 Feather m-0에서는 지원하지 않는다.
- 따라서 다음 링크를 참조하여 설명한다. (https://velog.io/@wimes/%EC%95%84%EB%91%90%EC%9D%B4%EB%85%B8-M0-SAMD21G-%EC%97%90%EC%84%9C-%EC%83%88%EB%A1%9C%EC%9A%B4-serial-%EB%A7%8C%EB%93%A4%EA%B8%B0-qfk36n99m7)
- 정말 자세히 설명되어 있으니 해당 내용 읽고 따라하면 된다.



#### 16. 서보모터 사용법

- 간단하게 Servo.h 라이브러리를 사용하면 된다.

  ```c++
                  //해당 핀 번호에 servo 모터를 확인한다.
  				servo_pan.attach(SERVO_MOTOR_PAN);
                  servo_tilt.attach(SERVO_MOTOR_TILLT);
                  delay(1000);
  				
  				//서보 각도를 조절한다(110도)
                  servo_pan.write(110);
                  delay(1000);
  				//서보 모터의 작동을 중지시킨다.(detach를 하지 않으면 모터가 끊임없이 PWM을 전송하며 모터 버징이 일어난다.)
                  servo_pan.detach();
                  
                  delay(1000);
  
                  servo_tilt.write(90);
                  delay(1000);
                  servo_tilt.detach();
  ```

#### 17. LED의 경우 `#include <Adafruit_NeoPixel_ZeroDMA.h>`를 사용했다.(https://learn.adafruit.com/dma-driven-neopixels)

```c++
                switch( ((LED*)value)->LED_id){
                    case 1:
                        //LED_DATA1에 LED_NUM(여기선 90)만큼을 사용하겠다 초기화
                        neoPixelLED.init(LED_DATA1, LED_NUM);
                        break;
                    case 2:
                        neoPixelLED.init(LED_DATA2, LED_NUM);
                        break;
                    default:
                        neoPixelLED.init(LED_DATA1, LED_NUM);
                }
				//밝기 0~255 중 100으로 선정
                neoPixelLED.setBrightness(100);
				//clear를 실행하면 기존의 show 된 led를 모두 off 시킴
                neoPixelLED.clear();
                for(int led_num = 0; led_num < 90; led_num++){
                    //각각의 led 번호에 color를 설정함.
                    neoPixelLED.setColor(led_num, ((LED*)value)->R, ((LED*)value)->G, ((LED*)value)->B, ((LED*)value)->W );
                }
				//show를 실행하면 설정된 led가 켜진다.
                neoPixelLED.show();
```



  

#### 18. Booting led(Blynk)의 경우 다음과 같이 delay를 하지 않고 사용하는 방법이 있다.

```c++
    //현재 시간 확인
	unsigned long currentMillis = millis();
	//현재 시간이 이전 시간보다 1000이상 차이가 난다면 상태를 변환하면 된다.
    if(currentMillis - prevMillis >= 1000){
        prevMillis = currentMillis;

        switch(PARAMETER){
            case PARAMETER_SET_BOOTING_LED_REVERSE:
                if( booting_led.booting_led_1 == LOW ){
                    booting_led.booting_led_1 = HIGH;
                }else booting_led.booting_led_1 = LOW;

                if( booting_led.booting_led_2 == LOW ){
                    booting_led.booting_led_2 = HIGH;
                }else booting_led.booting_led_2 = LOW;
            break;
        }

        digitalWrite(PARAMETER_BOOTING_LED_1, booting_led.booting_led_1);
        digitalWrite(PARAMETER_BOOTING_LED_2, booting_led.booting_led_2);
    }
```

#### 19. 핀번호 데이터 시트는 다음 내용을 참조하면 된다.

- https://cdn-shop.adafruit.com/product-files/2772/atmel-42181-sam-d21_datasheet.pdf

- https://cdn-learn.adafruit.com/downloads/pdf/adafruit-feather-m0-basic-proto.pdf

- 특히, ATSAMD21G의 경우 다음과 같이 PB00번 으로 구성되어 있다.(https://cdn-learn.adafruit.com/assets/assets/000/028/801/original/adafruit_products_M0SCHEM.png?1448656357)

  ![image](https://cdn-learn.adafruit.com/assets/assets/000/028/801/original/adafruit_products_M0SCHEM.png?1448656357)

- ATSAMD21G 핀에 매핑하는 아두이노 핀 번호를 알 수 있다.

- 예를 들면 PB02번에 해당하는 핀에 HIgh로 놓고 싶다면

  ```c++
  pinMode(A5,OUTPUT);
  digitalWrite(A5, HIGH);
  
  ```

  

- 하지만 기타 핀 번호의 경우 정의가 되어 있지 않은 경우가 있다.
- 이는 variant.cpp에 정의 되어 있는 내역을 확인하면 핀 번호를 확인할 수 있다. 예를 들어 PA 19번의 경우 34번이 아두이노 입력 핀이다.

![image](https://user-images.githubusercontent.com/18729679/119442430-c9803780-bd62-11eb-9cca-f6bac5f27cb3.png)

- 위치 경로는 다음과 같다. 아마 아두이노를 설치하면 다음과 같이 핀 매핑이 되어있는 파일이 설치되는 것 같다.

- 이전에 설명한 시리얼 포트 확장에 대한 SERCOM 내용도 이미 여기에 포함되어 있었다. 따라서 따로 위에 시리얼 확장 코드를 main.ino에서 작성할 필요가 없었다.

  `pinPeripheral(PIN_NUM, PIO_SERCOM_ALT/PIO_SERCOM);`

  으로 초기화 후 명령어 Serial5.print함수 쓰면 사용이 가능하다.

- 파일 경로는 C:\Users\HJKim\AppData\Local\Arduino15\packages\adafruit\hardware\samd\1.6.7\variants\feather_m0쯤에 있었으니 다른 컴퓨터에서도 아두이노를 설치하면 찾아 볼 수 있다.



#### 20. 타이머 핸들러 내용을 확인해보면 다음과 같다.

- ATSAMD21G18A 데이터 시트(https://ww1.microchip.com/downloads/en/DeviceDoc/SAM-D21DA1-Family-Data-Sheet-DS40001882G.pdf)
- 추가적으로 다른 사람이 만든 인터럽트 코드이다.(https://gist.github.com/nonsintetic/ad13e70f164801325f5f552f84306d6f)

```c++
void setTimerFrequency(int frequencyHz) {
  int compareValue = (CPU_HZ / (TIMER_PRESCALER_DIV * frequencyHz)) - 1;
  TcCount16* TC = (TcCount16*) TC3;
  // Make sure the count is in a proportional position to where it was
  // to prevent any jitter or disconnect when changing the compare value.
  TC->COUNT.reg = map(TC->COUNT.reg, 0, TC->CC[0].reg, 0, compareValue);
  TC->CC[0].reg = compareValue;
  Serial.println(TC->COUNT.reg);
  Serial.println(TC->CC[0].reg);
  //
  while (TC->STATUS.bit.SYNCBUSY == 1);
}

void startTimer(int frequencyHz) {
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync

  TcCount16* TC = (TcCount16*) TC3;

  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  // Use the 16-bit timer
  TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  // Use match mode so that the timer counter resets when the count matches the compare register
  TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  // Set prescaler to 1024
  TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  setTimerFrequency(frequencyHz);

  // Enable the compare interrupt
  TC->INTENSET.reg = 0;
  TC->INTENSET.bit.MC0 = 1;

  NVIC_EnableIRQ(TC3_IRQn);

  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
}

void TC3_Handler() {
  TcCount16* TC = (TcCount16*) TC3;
  // If this interrupt is due to the compare register matching the timer count
  // we toggle the LED.
  if (TC->INTFLAG.bit.MC0 == 1) {
    TC->INTFLAG.bit.MC0 = 1;
    // Write callback here!!!
    serial_interrupt();
  }
}
```

-  `REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3) ;`

> 데이터시트 122쪽을 보면 다음과 같은 내용을 확인할 수 있다.
>
> 	1. CLKEN을 확인하면 Clock Enable 비트로 generic clock을 enable할지 disable할지 결정해야한다.
> 	2. GCLK_CLKCTRL_GEN_GCLK0의 경우 Generic clock generator 0번을 사용하겠다는 의미이다.
> 	3. GCLK_CLKCTRL_ID_TCC2_TC3의 경우도 TCC2, TC3 사용 의미

-   `while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync`

> 데이터시트 121쪽을 보면 Synchronization Busy Status에 대한 설명이 있다.
>
> This bit is cleared when the synchronization of registers between the clock domains is complete. This bit is set when the synchronization of registers between clock domains is started.
>
> 즉, 동기화가 완료되면 0으로 초기화 되고 동기화 중에는 1로 set되어 있다.



- `C->CTRLA.reg &= ~TC_CTRLA_ENABLE;`

> 데이터시트 108쪽, 420쪽을 보면 CRTLA에 대한 내용이 있다.
>
> Setting the Software Reset bit in CTRLA (CTRLA.SWRST=1) will trigger write-synchronization and set SYNCBUSY.SWRST. When writing a ‘1’ to the CTRLA.SWRST bit it will immediately read as ‘1’. CTRL.SWRST and SYNCBUSY.SWRST will be cleared by hardware when the peripheral has been reset. Writing a '0' to the CTRL.SWRST bit has no effect. The Ready interrupt (if available) cannot be used for Software Reset writesynchronization.

​	

-  `// Enable the compare interrupt
    TC->INTENSET.reg = 0;
    TC->INTENSET.bit.MC0 = 1;`

> 데이터시트 47쪽에 해당 내용이 있다.
>
> Interrupt Line Mapping Each of the 29 interrupt lines is connected to one peripheral instance, as shown in the table below. Each peripheral can have one or more interrupt flags, located in the peripheral’s Interrupt Flag Status and Clear (INTFLAG) register. The Interrupt flag is set when the Interrupt condition occurs. Each interrupt in the peripheral can be individually enabled by writing a one to the corresponding bit in the peripheral’s Interrupt Enable Set (INTENSET) register, and disabled by writing a one to the corresponding bit in the peripheral’s Interrupt Enable Clear (INTENCLR) register. An interrupt request is generated from the peripheral when the Interrupt flag is set and the corresponding interrupt is enabled. The interrupt requests for one peripheral are ORed together on system level, generating one interrupt request for each peripheral. An interrupt request will set the corresponding Interrupt Pending bit in the NVIC Interrupt Pending registers (SETPEND/CLRPEND bits in ISPR/ICPR). For the NVIC to activate the interrupt, it must be enabled in the NVIC interrupt enable register (SETENA/CLRENA bits in ISER/ICER). The NVIC Interrupt Priority registers IPR0-IPR7 provide a priority field for each interrupt
>
> 

> 또한 데이터시트 66쪽을 보면 TC3의 Generator 표의 내용을 보면 MC0, MC1이 선택되어 있는 것을 알 수 있다.
>
> 
>
> 다시, 데이터시트 611쪽을 확인하면 다음과 같은 Interrupt Enable Set에 대한 내용을 확인 할 수 있다.
>
> ![image](https://user-images.githubusercontent.com/18729679/119764260-09bef180-beec-11eb-9fb0-db408f3e441b.png)
>
>  내용을 확인하면 Match or Capture Channel이란 설명이 나온다.(https://m.blog.naver.com/duvallee/221459826325)
>
> - 캡쳐 채널은(Capture Channels) 채널은 입력모드(input mode)로서 다음은 입력캡쳐모드에 대한 블럭다이어그램입니다.
>
> - input Capture(IC) 모드는 CH의 값을 읽어 Capture/Compare Register(CCR)에 CNT의 카운트 값을 저장하고 interrupt를 발생시키는 모드이다.
>
>   ##    **어디서 많이 봤던거 맞다! 예전에 STM32F103_NUCLEO 에서 PWM을 만들기 위해 필요했던 CC레지스터가 바로 이거다!!!!!!**

-   `NVIC_EnableIRQ(TC3_IRQn);`

    `TC->CTRLA.reg |= TC_CTRLA_ENABLE;`
    `while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync`

> > 여기는 말 안해도 확인할 수 있다고 생각한다.
> >
> > TC3의 인터럽트를 Nested Vector Interrupt에 넣고, 기존에 Timer ControlA에 대한 레지스터를 Enable하여 Sync가 종료될 때까지 기다린다.

-   `if (TC->INTFLAG.bit.MC0 == 1) {
      TC->INTFLAG.bit.MC0 = 1;
      // Write callback here!!!
      serial_interrupt();
    }`

>이 내용은 데이터시트 597페이지에 완전 자세하게 나와있다.
>
>![image](https://user-images.githubusercontent.com/18729679/119771050-07629480-bef8-11eb-9c69-e1fdc97d7edd.png)
>
>TC->INTFLAG.bit.MC0의 경우 CC레지스터 값을 읽으면 1로 set이 된다.
>
>따라서 1의 값을 가진다면 기존의 타이머의 시간이 흘렀다는 의미 이므로 이 때 callback을 사용하면 된다. 또한 이와 동시에 TC->INTFLAG.bit.MC0에 1을 넣으면 기존의 캡처 작업에서 값이 초기화가 진행된다.
>
>즉, 코드를 해석하면 CC값이 읽어지면 1로 타이머 인터럽트가 발생됐단 의미이고, 이 값을 다시 clear 해주기 위해 1을 넣은 다음 코드를 실행하면 된다.
>
>

#### 21. Capture mode 와 Compare 모드의 차이(https://www.microchip.com/forums/m1059100.aspx)

- "Capture mode"는 특정 이벤트가 발생할 때만 CCP register에 현재 시간 값을 저장하는 것이고,
- "Compare mode"는 지속적으로 CCP register과 timer 값을 비교해서 둘이 match되면 인터럽트, 트리거, transition을 발생하는 모드이다.