# TenEyesDoor
Arduino project for quest room
Скетч для Arduino Mega 2560 для двери с 10 глазами
   Алгоритм работы:
   1. Вкл питания.
   2. Сервоприводы (10) закрывают глаза.
   3. Вставляется ключ в скважину (до половины, т.к. вход дальше ему преграждает саленоид).
   4. Срабатывает датчик холла и начинается игра.
   5. Сервоприводы (10) открывают глаза
   6. В глазах загораются светодиоды с определенным цветом свечения (10 разных), цвет свечения одного из них совпадает с цветом свечения светодиода в замочной скважине(или около нее)
   7. Каждые 3-5 секунд комбинация меняется (под музыку)
   8. Если за отведенное время (3-5 секунд) пользователь нажал на глаз(встроенную в него кнопку) который светится таким же цветом как и светодиод в замочной скважине то он - угадал комбинацию и ему защитывается один бал (играет мызыка успешного выбора)
      на прогресбаре загорается очередной светодиод.
   9. Если пользователь не нажал никуда за отведенное время (3-5 сек), то комбинация меняется под музыку
   10. Если пользователь нажал на глаз цвет свечения светодиода которого не совпадает с цветом свечения светодиода замочной скважины, то он не угадал комбинацию (играет музыка неудачного выбора) и комбинация цветов меняется (под музыку).
   11. Если пользователь угадал 5 комбинаций (не обязательно подряд) открывается саленоид в замочной скважине и позволяет всунуть в нее ключ до конца.
   12. Когда ключ (с магнитом) вставлен, срабатывает датчик Холла № 2(звучит орг'ан) глаза закрываются и через реле открывается электромагнитный замок двери.
   13.Выкл
