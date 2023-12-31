/*
Peter Semiletov, 2023
*/

#include <vector>
#include <cmath>

#include "dsp.h"


using namespace std;


float db_scale;

void init_db()
{
/*
#if defined(__clang__)
//
#elif defined(__GNUC__) || defined(__GNUG__)

  unsigned int mxcsr = __builtin_ia32_stmxcsr ();
  mxcsr |= MXCSR_DAZ | MXCSR_FTZ;
  __builtin_ia32_ldmxcsr (mxcsr);

#elif defined(_MSC_VER)
//
#endif
*/

  db_scale = log (10.0) * 0.05;
}



/*************** SATURATION/DIST/OVERDRIVE *******************/

#include <cmath>



float jimi_fuzz (float input_sample, float level, float distortion)
{
  // Этап 1: Гиперболическое искажение
  float hyperbolic_sample = tanh (input_sample * distortion);

    // Этап 2: Насыщение
  float saturated_sample = hyperbolic_sample * 0.7f;  // Уменьшаем амплитуду для насыщения

    // Этап 3: Применение "ламповости"
  float lamp_sample = (1.0f - expf(-fabs (saturated_sample))) * (saturated_sample >= 0 ? 1 : -1);

    // Умножаем на уровень для настройки громкости
  float output_sample = lamp_sample * level;

  if (output_sample > 1.0f)
      output_sample = 1.0f;
  else
      if (output_sample < -1.0f)
         output_sample = -1.0f;


  return output_sample;
}

/*
float fuzz (float input, float level, float intensity)
{
    float distorted = sin (input) + intensity * sin (input * 12.0f);

    // Уровень выхода
    distorted *= level;


    if (distorted > 1.0f || distorted < -1.0f)
       distorted /= 2;

        // Ограничиваем выходное значение в диапазоне от -1 до 1
    if (distorted > 1.0f) {
        distorted = 1.0f;
    } else if (distorted < -1.0f) {
        distorted = -1.0f;
    }


   return distorted;
}
*/

float overdrive (float input, float drive, float level)
{
//    float output = std::sin(input * 2.0f * 3.14159265359f); // Примерный алгоритм, можно настроить под нужное звучание
  float output = std::atan (input * 3.0f * 3.14159265359f); // Примерный алгоритм, можно настроить под нужное звучание

  output *= drive; // Уровень искажения зависит от параметра drive

   // Применяем предварительное усиление
  output *= level;


  // Ограничиваем выходное значение в пределах [-1, 1]
  if (output > 1.0f)
      output = 1.0f;
  else
      if (output < -1.0f)
         output = -1.0f;


  return output;
}


// Функция для "хриплого" гитарного искажения
float gritty_guitar_distortion (float input_sample, float distortion_level)
{
    // Шаг 1: Усиление с более высоким коэффициентом
  float amplified_sample = input_sample * (1.5f + 4.0f * distortion_level);

    // Шаг 2: Операционный усилитель с более высокой нелинейностью
  float op_amp_output = tanh (2.0f * amplified_sample);

    // Шаг 3: Клиппер/ограничитель с более сильным искажением
  float clipped_sample = tanh (2.0f * op_amp_output);

  clipped_sample = std::min(1.0f, std::max(-1.0f, clipped_sample));

  return clipped_sample;
}


//analog
float warmify (float x, float warmth)
{
  // Проверяем, что "warmth" находится в пределах от 0 до 1
  warmth = std::min (1.0f, std::max(0.0f, warmth));

  // Применяем теплое, аналоговое воздействие на сигнал
  float warm_x = x * (1.0f - warmth) + std::sin(x * M_PI) * warmth;

  // Ограничиваем значения в диапазоне от -1 до 1
  warm_x = std::min(1.0f, std::max(-1.0f, warm_x));

  return warm_x;
}


/*************** FILTERS *******************/


float hp_filter (float input, float samplerate, float fc)
{
  // Рассчитываем константу времени T из частоты среза
  float T = 1.0f / (2.0f * M_PI * fc);

    // Рассчитываем коэффициент для фильтра
  float alpha = T / (T + 1.0f);

    // Инициализируем предыдущее значение (первоначальное условие)
  static float prev_output = 0.0f;

    // Применяем фильтр к входному сигналу
  float output = alpha * (prev_output + input - prev_output);

    // Обновляем предыдущее значение для следующего вызова
  prev_output = output;

  return output;
}


/*************** MISC *******************/


// Функция для мягкого понижения уровня сигнала, если он превышает порог в 18 дБ (выше нуля)

const float thresholdLevel18db = pow (10.0f, 18.0f / 20.0f); // Значение, соответствующее +18 дБ


float soft_limit (float input)
{
  // Проверяем, превышает ли входной сигнал порог
  if (input > thresholdLevel18db)
     {
      // Рассчитываем множитель для понижения уровня
      float reduction_factor = thresholdLevel18db / input;

        // Применяем мягкое понижение уровня
      return input * reduction_factor;
    }

    // Если сигнал не превышает порог, возвращаем его без изменений
  return input;
}



float apply_resonance (float input, float resonance_amount)
{
  // Вычисляем коэффициент для изменения резонанса
  float resonance_factor = 1.0f + resonance_amount;

 // Применяем резонансное изменение
  float resonant_signal = input * resonance_factor;

  // Ограничиваем результат в пределах от -1 до 1 (клиппинг)
  if (resonant_signal > 1.0f)
     resonant_signal = 1.0f;
  else
      if (resonant_signal < -1.0f)
          resonant_signal = -1.0f;


  return resonant_signal;
}
