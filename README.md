#### [1. c, h 파일 분리 사용자 라이브러리 작성법][https://driz2le.tistory.com/58]

![image](https://user-images.githubusercontent.com/18729679/116334140-b172d200-a80f-11eb-8ef6-b2763a6c2388.png)

![image](https://user-images.githubusercontent.com/18729679/116334265-dff0ad00-a80f-11eb-933e-a237a3609afe.png)

> 다음과 같이 기존의 ino파일에 있던 내용을 c에서 작성하여 선언한 후 ino에서 실행시키는 방법으로 진행 할 수 있다.



![image-20210428105828682](C:\Users\HJKim\AppData\Roaming\Typora\typora-user-images\image-20210428105828682.png)

- 상단에 c로 선언한 파일에서는 기존의 ino와 다르게 Arduino.h 파일을 include 해줘야 한다.

![image-20210428105922681](C:\Users\HJKim\AppData\Roaming\Typora\typora-user-images\image-20210428105922681.png)



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
    int array[9] = { 3, 7, 9, 5, 6, 1, 8, 2, 4 }; // 내림차순 선택정렬
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

#### 3. 함수 포인터 

```c
void set_init(int PARAMETER, void* value){

  switch(PARAMETER){
    case PARAMETER_SET_INIT:
      ((STEP_MOTOR*)value)-> motor_id = step_motor.motor_id;
      ((STEP_MOTOR*)value)-> dir = step_motor.dir;
      ((STEP_MOTOR*)value)-> angle = step_motor.angle;
      ((STEP_MOTOR*)value)-> rotation_number = step_motor.rotation_number;
      ((STEP_MOTOR*)value)-> rpm = step_motor.rpm;
    break;
  }
}
```

