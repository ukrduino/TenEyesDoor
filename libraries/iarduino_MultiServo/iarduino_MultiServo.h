//	Библиотека работы arduino с Multi Servo Shield  http://iarduino.ru/
//
//	Подключение библиотеки:
//	#include <iarduino_MultiServo.h>
//	iarduino_MultiServo MSS                  // Объявляем переменную MSS, для работы с библиотекой (объект класса iarduino_MultiServo).
//	
//	Модули подключаются к двум выводам arduino (SCL и SDA) предназначенным для аппаратной работы с шиной I2C
//	В библиотеке реализованы 6 основный и 7 дополнительных функций
//	
//	Основные функции:
//	Функция begin        Назначение:            Инициализация модуля
//	                     Синтаксис:             begin([uint8_t ID [, uint16_t Hz]])
//	                     Параметры:             ID - ID-адрес модуля (необязательный параметр) указывается от 0 до 127 (по умолчанию 0x40)
//	                                            Hz - Частота ШИМ (необязательный параметр) указывается в Гц от 1 до 1526 (по умолчанию 50 Гц)
//	                     Примечание:            Достаточно вызвать 1 раз в начале кода, но допускаются и повторные вызовы, если требуется изменить частоту или адрес
//	                     Возвращаемые значения: Нет
//
//	Функция analogWrite  Назначение:            Установка ШИМ на выходе, с определённым коэффициентом заполнения [и фазовым сдвигом]
//	                     Синтаксис:             analogWrite(uint8_t pin, uint16_t pwm [, uint16_t phase_shift])
//	                     Параметры:             pin - № выхода, от 0 до 15. Если указать SERVO_ALL, то значения применятся ко всем выходам
//	                                            pwm - коэффициент заполнения ШИМ, лежит в пределах от 0 до 4095
//	                                            phase_shift - Фазовый_сдвиг (необязательный параметр) указывается от 0 до 4095 (по умолчанию 0), устанавливает задержку на определённое количество тактов до начала импульсов и не влияет на коэффициент заполнения
//	                     Примечание:            Работает как одноимённая функция для обычных выходов ШИМ
//	                     Возвращаемые значения: Нет
//
//	Функция digitalWrite Назначение:            Установка логического состояния LOW или HIGH на выходе
//	                     Синтаксис:             digitalWrite(uint8_t pin, bool LOW/HIGH)
//	                     Параметры:             pin - № выхода, от 0 до 15. Если указать SERVO_ALL, то значения применятся ко всем выходам
//	                     Примечание:            Работает как одноимённая функция для обычных выходов
//	                     Возвращаемые значения: Нет
//
//	Функция analogRead   Назначение:            Чтение коэффициента заполнения установленного на выходе
//	                     Синтаксис:             analogRead(uint8_t pin)
//	                     Параметры:             pin - № выхода, от 0 до 15.
//	                     Примечание:            Работает как одноимённая функция для аналоговых входов, но возвращает значения от 0 до 4096 (вместо 1023)
//	                                            Значение 0 соответствует постоянному уровню LOW, а 4096 постоянному уровню HIGH
//	                     Возвращаемые значения: uint16_t pwm - коэффициент заполнения ШИМ
//
//	Функция servoWrite   Назначение:            Поворачивает сервопривод на заданный угол
//	                     Синтаксис:             servoWrite(uint8_t pin, uint16_t angle)
//	                     Параметры:             pin - № выхода, от 0 до 15. Если указать SERVO_ALL, то значения применятся ко всем выходам
//	                                            angle - заданный угол поворота от 0° до 360°
//	                     Возвращаемые значения: Нет
//
//	Функция servoSet     Назначение:            Задает максимально допустимый угол поворота сервопривода и позиционирует крайние значения
//	                     Синтаксис:             servoSet(uint8_t pin, uint16_t angle_max, uint16_t pwm_min, uint16_t pwm_max)
//	                     Параметры:             pin - № выхода, от 0 до 15. Если указать SERVO_ALL, то значения применятся ко всем выходам
//	                                            angle_max - максимальный угол поворота от 1° до 360°
//	                                            pwm_min - коэффициент заполнения ШИМ соответствующий углу 0°
//	                                            pwm_max - коэффициент заполнения ШИМ соответствующий максимальному углу
//	                     Синтаксис:             servoSet(uint8_t pin, name)
//	                     Параметры:             pin - № выхода, от 0 до 15. Если указать SERVO_ALL, то значения применятся ко всем выходам
//	                                            name - название сервопривода
//	                     Примечание:            Достаточно вызвать 1 раз, как до, так и после функции begin, но допускаются и повторные вызовы
//												Значения обоих коэффициентов можно подобрать экспериментально, функцией analogWrite
//	                     Возвращаемые значения: Нет
//
//	Дополнительные функции:
//	Функция invert       Назначение:            Инвертирует сигналы на всех выходах
//	                     Синтаксис:             invert(bool true/false)
//	                     Возвращаемые значения: Нет
//
//	Функция outdrv       Назначение:            Устанавливает схему подключения выходов внутри чипа
//	                     Синтаксис:             outdrv(bool true/false)
//	                     Параметры:             true - схема с каскадным выходом   /// Если устройства подключены к модулю через драйвер
//	                                            false - схема с открытым стоком    /// Если устройства подключены к модулю напрямую, без драйвера
//	                     Возвращаемые значения: Нет
//
//	Функция outState     Назначение:            Устанавливает одно из трех состояний на выходах, при подаче «1» на вход OE (отключение выходов)
//	                     Синтаксис:             outState(uint8_t LOW/HIGH/IMPEDANCE)
//	                     Примечание:            При указании HIGH и выборе схемы с открытым стоком, на выходах будет состояние IMPEDANCE
//	                     Возвращаемые значения: Нет
//
//	Функция extClock     Назначение:            Устанавливает работу чипа от внешнего источника тактирования, с указанием его частоты
//	                     Синтаксис:             extClock(uint16_t кHz)
//	                     Параметры:             кHz - частота внешнего источника тактирования, указывается в кГц от 1 до 50'000
//	                     Примечание:            Для перехода на внутренний источник тактирования, нужно указать параметр false
//	                     Возвращаемые значения: Нет
//
//	Функция restart      Назначение:            Перезагружает все чипы на шине, с установкой значений по умолчанию
//	                     Синтаксис:             restart()
//	                     Возвращаемые значения: Нет
//
//	Функция bus          Назначение:            Устанавливает скорость работы шины I2C
//	                     Синтаксис:             bus(uint16_t кHz)
//	                     Параметры:             кHz - частота, указывается в кГц от 1 до ((F_CPU/10000)-16)/2, по умолчанию частота = 100кГц
//	                     Примечание:            Функцию можно вызывать как до, так и после функции begin
//	                     Возвращаемые значения: Нет
//
//	Функция reg          Назначение:            Чтение или запись одного байта данных в регистр чипа
//	                     Синтаксис:             reg(uint8_t adds [, uint8_t data])
//	                     Параметры:             adds - адрес регистра, данные которого нужно прочитать/записать
//	                                            data - данные для записи
//	                     Примечание:            Если функция вызвана без параметра data, то она вернет байт данных находящийся в регистре с адресом adds
//	                                            Если функция вызвана с параметром data, то она запишет данные data в регистр с адресом adds
//	                     Возвращаемые значения: uint8_t data - прочтённые или записываемые данные
//


#ifndef iarduino_MultiServo_h
#define iarduino_MultiServo_h

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define	IMPEDANCE			2	//	третье состояние выхода (состояние высокого импеданса - состояние высокого сопротивления - состояние отключённого выхода)
#define	SERVO_ALL			255	//	№ выхода используемый как указание значений всем выходам
#define SERVO_SG90			1	//	http://iarduino.ru/shop/Mehanika/mikroservoprivod---1-6-kg-sm.html
#define SERVO_MG90			2	//	http://iarduino.ru/shop/Mehanika/mg995r-pro-servo.html
#define SERVO_MG996R		3	//	http://iarduino.ru/shop/Mehanika/mg995r-pro-servo.html
#define SERVO_FutabaS3003	4	//	http://iarduino.ru/shop/Mehanika/futaba-s3003.html

class iarduino_MultiServo{
	public:
	/**	основные пользовательские функции **/
		void	begin					(uint8_t=0x40, uint16_t=50);						//	инициализ-я	чипа										(ID адрес, частота ШИМ в Гц)
		void	analogWrite				(uint8_t, uint16_t, uint16_t=0);					//	установка	ШИМ на выходе								(№ выхода от 0 до 15, коэффициент заполнения от 0 до 4095, фазовый сдвиг от 0 до 4095)
		void	digitalWrite			(uint8_t, bool);									//	установка	логического уровня на выходе				(№ выхода от 0 до 15, LOW / HIGH)
	   uint16_t	analogRead				(uint8_t);											//	чтение		коэффициента заполнения на выходе			(№ выхода от 0 до 15)
		void	servoWrite				(uint8_t, uint16_t);								//	установка	угла поворота сервопривода					(№ выхода от 0 до 15, угол поворота от 0° до 360°)
		void	servoSet				(uint8_t, uint16_t, uint16_t, uint16_t);			//	установка	параметров для сервопривода					(№ выхода от 0 до 15, максимальный угол поворота от 1° до 360°, коэффициент заполнения от 0 до 4095 для угла 0°, коэффициент заполнения от 0 до 4095 для максимального угла поворота)
		void	servoSet				(uint8_t, uint8_t);									//	установка	параметров для сервопривода					(№ выхода от 0 до 15, название сервопривода)

	/**	дополнительные пользовательские функции **/
		void	invert					(bool=true);										//	инверсия	сигналов на всех выходах					(true / false)
		void	outdrv					(bool=true);										//	выбор схемы	подключения всех выходов внутри чипа		(true - схема с каскадным выходом / false - схема с открытым стоком)
		void	outState				(uint8_t=0);										//	состояние	на всех выходах при подаче «1» на вход OE	(LOW / HIGH / IMPEDANCE) примечание: при указании HIGH и выборе схемы с открытым стоком, на выходах будет состояние высокого импеданса
		void	extClock				(uint16_t=0);										//	установка	частоты внешнего источника тактирования		(частота в кГц, от 1 до 50000)
		void	restart					();													//	перезагр-ка	всех чипов на шине							(без параметров)
		void	bus						(uint16_t=100);										//	установка	скорости работы шины I2C					(частота в кГц)
		uint8_t	reg						(uint8_t, uint16_t=256);							//	действия с  регистрами									(адрес регистра, 1 байт данных для записи) примечание: если указать только адрес регистра, то функция вернёт 1 байт данных из регистра
	
	private:
	/**	внутренние переменные **/
		uint8_t	PCA_uint_ID =			0x40;												//	адрес		чипа на шине I2C
	   uint16_t	PCA_uint_PWM =			50;													//	частота		ШИМ на выходах чипа в Гц
	   uint32_t	PCA_uint_OSC =			25000000;											//	частота		генератора тактовых импульсов в Гц, для формирования ШИМ
	   uint16_t	PCA_uint_ANGLE[16] =	{180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180};	//	максимальный угол поворота
	   uint16_t	PCA_uint_ANGLE_MIN[16] ={170,170,170,170,170,170,170,170,170,170,170,170,170,170,170,170};	//	коэффициент заполнения для угла 0°
	   uint16_t	PCA_uint_ANGLE_MAX[16] ={470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470};	//	коэффициент заполнения для максимального угла
		bool	PCA_flag_I2C =			false;												//	флаг		инициализации шины I2C
		bool	PCA_flag_EXTCLK =		false;												//	флаг		состояния бита EXTCLK
		bool	PCA_flag_INVRT =		false;												//	флаг		состояния бита INVRT
		bool	PCA_flag_OUTDRV =		true;												//	флаг		состояния бита OUTDRV
		uint8_t	PCA_uint_OUTNE =		0;													//	значение	двух битов OUTNE
		
	/**	внутренние функции **/
		void	PCA_func_SET_MODE();														//	установка	регистров режимов MODE1, MODE2 и PRE_SCALE	(без параметров)
	
	/**	внутренние функции для работы с шиной I2C **/
		void	I2C_func_begin			();													//	установка	скорости работы шины I2C					(без параметров)
		void	I2C_func_START			();													//	выполнение	состояния START								(без параметров)
		void	I2C_func_RESTART		();													//	выполнение	состояния RESTART							(без параметров)
		void	I2C_func_STOP			();													//	выполнение	состояния STOP								(без параметров)
		void	I2C_func_SEND_ID		(uint8_t, bool);									//	передача	первого байта								(ID-адрес, бит RW)
		void	I2C_func_WRITE_BYTE		(uint8_t);											//	передача	одного байта								(байт для передачи)
		uint8_t	I2C_func_READ_BYTE		(bool);												//	получение	одного байта								(бит подтверждения ACK/NACK)
		void	I2C_func_WRITE_WORD		(uint8_t);											//	передача	нескольких байт из массива BUS_buff_DATA	(количество байт)
		void	I2C_func_READ_WORD		(uint8_t);											//	получение	нескольких байт в массив BUS_buff_DATA		(количество байт)
	   uint16_t	I2C_mass_STATUS[0x04] =	{100,1000,1,0x78};									//	статус		скорость работы шины в кГц (макс F_CPU/2), ожидание сброса флага TWINT в циклах, флаг успешного результата, содержимое флагов TWS регистра состояния TWSR
		uint8_t	BUS_buff_DATA[0x21];														//	буфер		данных для функций I2C_func_READ_WORD, I2C_func_WRITE_WORD. Элемент массива 0x20 содержит число переданных/прочтённых байт
};

#endif