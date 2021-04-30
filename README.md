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

  