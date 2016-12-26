/* Скетч для Arduino Mega 2560 для двери с 10 глазами
   Алгоритм работы:
   1. Вкл питания.
   2. Сервоприводы (10) закрывают глаза.
   3. Вставляется ключ в скважину.
   3. Сервоприводы (10) открывают глаза
   3. В глазах загораются светодиоды с определенным цветом свечения (10 разных), цвет свечения одного из них
      совпадает с цветом свечения светодиода в замочной скважине(или около нее)
   4. Каждые 3-5 секунд комбинация меняется (под музыку)
   5. Если за отведенное время (3-5 секунд) пользователь нажал на глаз(встроенную в него кнопку) который светится
      таким же цветом как и светодиод в замочной скважине то он - угадал комбинацию и ему защитывается один бал (играет мызыка успешного выбора)
      на прогресбаре загорается очередной светодиод.
   6. Если пользователь не нажал никуда за отведенное время (3-5 сек), то комбинация меняется под музыку
   7. Если пользователь нажал на глаз цвет свечения светодиода которого не совпадает с цветом
      свечения светодиода замочной скважины, то он не угадал комбинацию (играет музыка неудачного выбора) и
      комбинация цветов меняется (под музыку).
   8. Если пользователь угадал 5 комбинаций (не обязательно подряд) открывается саленоид в замочной скважине и позволяет всунуть в нее ключ.
   9. Когда ключ (с магнитом) вставлен,
          срабатывает датчик Холла (звучит орг'ан)
          глаза закрываются
          и через реле открывается электромагнитный замок двери.
   10.Выкл
*/


/* Используется библиотека https://codeload.github.com/pololu/pololu-led-strip-arduino/zip/4.1.0 */
#include <PololuLedStrip.h>
/* Используется библиотека http://iarduino.ru/file/255.html */
#include <iarduino_MultiServo.h>

/* Используется библиотека https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Introduction */
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

/* Для плеера */

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;



/*Объявляем переменную MSS, для работы с библиотекой сервоприводов (объект класса iarduino_MultiServo).*/
iarduino_MultiServo MSS;
/*Вывод данных для светодиодов подключен на пин 12 Arduino*/
PololuLedStrip<12> ledStrip;

/*Настройка цветов*/
const rgb_color red = {255, 0, 0};
const rgb_color green = {0, 255, 0};
const rgb_color blue = {0, 0, 255};
const rgb_color white = {255, 255, 255};
const rgb_color yellow = {255, 255, 0};
const rgb_color magenta = {255, 0, 255};
const rgb_color cian = {0, 255, 255};
const rgb_color aqua = {0, 125, 125};
const rgb_color purple = {125, 0, 125};
const rgb_color teal = {0, 125, 125};

/* Буфер для записи в светодиоды 10(глаз) + 1(замочная скважина) + 5 прогресс бар
   Первые 10 элементов (colors[0]-colors[9]) выводятся на глаза
   одинадцатый -  colors[10] - на замочную скважину,
   последние 5 colors[11] - colors[15] - на диоды прогресс бара
*/
#define LED_COUNT 16
rgb_color ledsArray[LED_COUNT];


#define COLOR_COUNT 10
/* Массив для перемешивания цветов перед выводом на светодиоды,
   после пермешивания копируется в первые 10 элементов (colors[0]-colors[9])
*/
rgb_color colorsArray[] = {red, green, blue, white, yellow, magenta, cian, aqua, purple, teal};

/* кнопки глаз*/
int buttonPin9 = 40;
int buttonPin8 = 41;
int buttonPin7 = 42;
int buttonPin6 = 43;
int buttonPin5 = 44;
int buttonPin4 = 45;
int buttonPin3 = 46;
int buttonPin2 = 47;
int buttonPin1 = 48;
int buttonPin0 = 49;

/* Для регистрации количества правильных ответов и отображения на прогресс баре */
int progress = 0;
/* Для хранения переменной правильного ответа */
int answer = 0;

/* для контроля времени на ответ */
unsigned long time1;
unsigned long time2;
/* Время отведенное на ответ в милисекундах например 3000 = 3 секунды */
unsigned long timeForAnswer = 3000;

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  
  MSS.servoSet(SERVO_ALL, SERVO_SG90);
  MSS.begin();
  delay(2000);
  MSS.servoWrite(SERVO_ALL, 0);
  delay(2000);
  MSS.servoWrite(SERVO_ALL, 100);
  pinMode(buttonPin0, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);
  pinMode(buttonPin7, INPUT_PULLUP);
  pinMode(buttonPin8, INPUT_PULLUP);
  pinMode(buttonPin9, INPUT_PULLUP);
}

void loop() {  
  checkProgressAndStartNewTask();
  getAnswer();
}

void getAnswer()
{
  time1 = millis();
  delay(10);
  time2 = millis();
  while ((time2 - time1) < timeForAnswer)
  {
    /* любое число кроме чисел от 0-9 */
    int yourAnswer = 55;
    yourAnswer = readButtons();
    if (yourAnswer == answer)
    {
      playSuccess();
      progress += 1;
      delay(100);
      break;
    }
    if (yourAnswer < 10 && yourAnswer != answer)
    {
      playFail();
      break;
    }
    time2 = millis();
  }
}


int readButtons() {
  if (!digitalRead(buttonPin9))
  {
    Serial.print("Your answer: ");
    Serial.println(9);
    delay(500);
    return 9;
  };
  if (!digitalRead(buttonPin8))
  {
    Serial.print("Your answer: ");
    Serial.println(8);
    delay(500);
    return 8;
  };
  if (!digitalRead(buttonPin7))
  {
    Serial.print("Your answer: ");
    Serial.println(7);
    delay(500);
    return 7;
  };
  if (!digitalRead(buttonPin6))
  {
    Serial.print("Your answer: ");
    Serial.println(6);
    delay(500);
    return 6;
  };
  if (!digitalRead(buttonPin5))
  {
    Serial.print("Your answer: ");
    Serial.println(5);
    delay(500);
    return 5;
  };
  if (!digitalRead(buttonPin4))
  {
    Serial.print("Your answer: ");
    Serial.println(4);
    delay(500);
    return 4;
  };
  if (!digitalRead(buttonPin3))
  {
    Serial.print("Your answer: ");
    Serial.println(3);
    delay(500);
    return 3;
  };
  if (!digitalRead(buttonPin2))
  {
    Serial.print("Your answer: ");
    Serial.println(2);
    delay(500);
    return 2;
  };
  if (!digitalRead(buttonPin1))
  {
    Serial.print("Your answer: ");
    Serial.println(1);
    delay(500);
    return 1;
  };
  if (!digitalRead(buttonPin0))
  {
    Serial.print("Your answer: ");
    Serial.println(0);
    delay(500);
    return 0;
  };
}


void checkProgressAndStartNewTask()
{
  if (progress < 5)
  {
    updateLeds(ledsArray, colorsArray, COLOR_COUNT);
  }
  else {
    stopGame();
  }
}

void stopGame()
{
  delay(1000);
  MSS.servoWrite(SERVO_ALL, 0);
  delay(2000);
  playGrandFinalSuccess();
  // openDoor()
  while (1) {}
}

void updateLeds(rgb_color * ledsArray, rgb_color * colorsArray, int colorsArraySize)
{
  /* выбор случайного числа (номера цвета от 0 до 9) */
  answer = random(colorsArraySize - 1);
  Serial.print("Right answer: ");
  Serial.println(answer);
  /* перемешивание цветов */
  for (int i = 0; i < colorsArraySize; i++)
  {
    shruffleArray(colorsArray, colorsArraySize);
  }
  /* вывод перемешенных цветов на светодиоды */
  for (int i = 0; i < colorsArraySize; i++)
  {
    ledsArray[i] = colorsArray[i];
  }
  /* вывод цвета по случайному индексу из перемешанного массива цветов на ключ */
  ledsArray[colorsArraySize] = ledsArray[answer];
  /* добавление прогресс бара к массиву отображения */
  addProgressLeds(ledsArray);
  ledStrip.write(ledsArray, LED_COUNT);
  printArray(ledsArray, LED_COUNT);
}

void addProgressLeds(rgb_color * ledsArray)
{
  for (int i = 0; i < progress; i++)
  {
    /* с конца прогресс бара загораются зеленые диоды */
    ledsArray[LED_COUNT - 1 - i] = green;
  }
}


/* Перемешивает массив цветов */
void shruffleArray(rgb_color * colorsArray, int arrSize)
{
  int last = 0;
  rgb_color temp = colorsArray[last];
  for (int i = 0; i < arrSize - 1; i++)
  {
    int index = random(arrSize - 1);
    colorsArray[last] = colorsArray[index];
    last = index;
  }
  colorsArray[last] = temp;
}
/* выдодит массив цветов просто для дебага */
void printArray(rgb_color * arr, int arrSize)
{
  for (int i = 0; i < arrSize; i++)
  {
    Serial.print(i);
    Serial.print(". ");
    Serial.print(arr[i].red);
    Serial.print(',');
    Serial.print(arr[i].green);
    Serial.print(',');
    Serial.println(arr[i].blue);
  }
}

void playSuccess()
{
    myDFPlayer.play(1);
  }

void playFail()
{
    myDFPlayer.play(2);
  }

void playGrandFinalSuccess()
{
    myDFPlayer.play(3);
  }
