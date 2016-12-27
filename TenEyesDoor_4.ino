/* Скетч для Arduino Mega 2560 для двери с 10 глазами
   Алгоритм работы:
   1. Вкл питания.
   2. Сервоприводы (10) закрывают глаза.
   3. Вставляется ключ в скважину (до половины, т.к. вход дальше ему преграждает саленоид).
   4. Срабатывает датчик холла и начинается игра.
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
   8. Если пользователь угадал 5 комбинаций (не обязательно подряд) открывается саленоид в замочной скважине и позволяет всунуть в нее ключ до конца.
   9. Когда ключ (с магнитом) вставлен,
          срабатывает датчик Холла № 2(звучит орг'ан)
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
#define buttonPin9 40
#define buttonPin8 41
#define buttonPin7 42
#define buttonPin6 43
#define buttonPin5 44
#define buttonPin4 45
#define buttonPin3 46
#define buttonPin2 47
#define buttonPin1 48
#define buttonPin0 49

/* датчики Холла */
/* так как датчики холла расчитаны на 3,3в то данные с них будем считывать с аналогового входа */
#define hallPin0 A2
#define hallPin1 A3

int hallValue0 = 0;
int hallValue1 = 0;

/* Реле */

#define salenoidPin 9
#define electroMagneticLockPin 8

/* Для регистрации количества правильных ответов и отображения на прогресс баре */
byte progress = 0;
/* Для хранения переменной правильного ответа */
byte answer = 11;
/* любое число кроме чисел от 0-9 */
byte yourAnswer = 11;

/* для контроля времени на ответ */
unsigned long time1;
unsigned long time2;
/* Время отведенное на ответ в милисекундах например 3000 = 3 секунды */
unsigned long timeForAnswer = 3000;

/* управление игрой */

bool gameStarted = false;


void setup() {
  Serial.begin(115200);
  /* старт проигрывателя */
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  /* старт сервоприводов */
  MSS.servoSet(SERVO_ALL, SERVO_SG90);
  MSS.begin();
  delay(200);
  /* закрыть все глаза */
  MSS.servoWrite(SERVO_ALL, 0);
  delay(1000);
  /* настройка выводов ардуино */
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
  pinMode(salenoidPin, OUTPUT);
  pinMode(electroMagneticLockPin, OUTPUT);
  digitalWrite(salenoidPin, HIGH);
  digitalWrite(electroMagneticLockPin, HIGH);
}

void loop() {
  if (!gameStarted) {
    waitForGameStart();
  }
  else
  {
    //  Serial.print("progress loop(): ");
    //  Serial.println(progress);
    checkProgressAndStartNewTask();
    getAnswer();
    //  Serial.print("progress loop() 2: ");
    //  Serial.println(progress);
  }
}

void waitForGameStart()
{
  hallValue0 = analogRead(hallPin0);
  if (hallValue0 < 10)
  {
    gameStarted = true;
    MSS.servoWrite(SERVO_ALL, 100);
    delay(1000);
  }
}

void getAnswer()
{
  time1 = millis();
  delay(10);
  time2 = millis();
  while ((time2 - time1) < timeForAnswer)
  {

    readButtons();
    if (yourAnswer == answer)
    {
      //      Serial.println("playSuccess!!!");
      yourAnswer = 11;
      doProgress();
      playSuccess();
      delay(200);
      break;
    }
    if (yourAnswer < 10)
    {
      if (yourAnswer != answer)
      {
        //        Serial.println("playFail!!!");
        yourAnswer = 11;
        playFail();
        delay(200);
        break;
      }
    }
    time2 = millis();
  }
}


void  readButtons() {
  if (!digitalRead(buttonPin9))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(9);
    delay(500);
    yourAnswer = 9;
    return;
  };
  if (!digitalRead(buttonPin8))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(8);
    delay(500);
    yourAnswer = 8;
    return;
  };
  if (!digitalRead(buttonPin7))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(7);
    delay(500);
    yourAnswer = 7;
    return;
  };
  if (!digitalRead(buttonPin6))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(6);
    delay(500);
    yourAnswer = 6;
    return;
  };
  if (!digitalRead(buttonPin5))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(5);
    delay(500);
    yourAnswer = 5;
    return;
  };
  if (!digitalRead(buttonPin4))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(4);
    delay(500);
    yourAnswer = 4;
    return;
  };
  if (!digitalRead(buttonPin3))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(3);
    delay(500);
    yourAnswer = 3;
    return;
  };
  if (!digitalRead(buttonPin2))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(2);
    delay(500);
    yourAnswer = 2;
    return;
  };
  if (!digitalRead(buttonPin1))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(1);
    delay(500);
    yourAnswer = 1;
    return;
  };
  if (!digitalRead(buttonPin0))
  {
    //    Serial.print("Button pressed: ");
    //    Serial.println(0);
    delay(500);
    yourAnswer = 0;
    return;
  };
}


void checkProgressAndStartNewTask()
{
  //  Serial.print("progress main: ");
  //  Serial.println(progress);
  if (progress < 5)
  {
    //    Serial.print("progress main 2: ");
    //    Serial.println(progress);
    updateLeds(ledsArray, colorsArray, COLOR_COUNT);
  }
  else {
    addProgressLeds(ledsArray);
    ledStrip.write(ledsArray, LED_COUNT);
    stopGame();
  }
}

void stopGame()
{
  delay(1000);
  MSS.servoWrite(SERVO_ALL, 0);
  delay(1000);
  openDoor();
  /* конец программы */
  while (1) {}
}

void openDoor()
{
  hallValue1 = analogRead(hallPin1);
  digitalWrite(salenoidPin, LOW);
  while (hallValue1 > 10)
  {
    hallValue1 = analogRead(hallPin1);
    delay(50);
  }
  playGrandFinalSuccess();
  digitalWrite(electroMagneticLockPin, LOW);
}

void updateLeds(rgb_color * ledsArray, rgb_color * colorsArray, int colorsArraySize)
{
  /* выбор случайного числа (номера цвета от 0 до 9) */
  answer = random(colorsArraySize - 1);
  //  Serial.print("Right answer: ");
  //  Serial.println(answer);
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
  // printArray(ledsArray, LED_COUNT);
}

void addProgressLeds(rgb_color * ledsArray)
{
  //  Serial.print("progress led add: ");
  //  Serial.println(progress);
  for (byte i = 0; i < progress; i++)
  {
    //      Serial.print("progress led add 2: ");
    //      Serial.println(progress);
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

void doProgress()
{
  //  Serial.print("progress 1: ");
  //  Serial.println(progress);
  progress ++;
  //  Serial.print("progress 2: ");
  //  Serial.println(progress);
}
