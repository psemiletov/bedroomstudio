/*
Peter Semiletov, 2023
*/


#ifndef DSP_H
#define DSP_H
/*
#if defined(__clang__)
//
#elif defined(__GNUC__) || defined(__GNUG__)

#define MXCSR_DAZ (1<<6)
#define MXCSR_FTZ (1<<15)

#elif defined(_MSC_VER)
//
#endif
*/

//#define _USE_MATH_DEFINES

#include <cmath>
#include <limits>
//#include <ostream>
#include <iostream>
#include <vector>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#include <cmath>

class BaxandallEqualizer {
public:
    float lowGain;   // Уровень нижней полосы (-90 до +6 dB)
    float midGain;   // Уровень средней полосы (-90 до +6 dB)
    float highGain;  // Уровень верхней полосы (-90 до +6 dB)
    float lowFrequency;  // Частота нижней полосы (Гц)
    float highFrequency; // Частота верхней полосы (Гц)

    BaxandallEqualizer() : lowGain(0), midGain(0), highGain(0), lowFrequency(100), highFrequency(10000) {
        recalculateCoeffs();
    }

    void setSampleRate(float newSampleRate) {
        sampleRate = newSampleRate;
        recalculateCoeffs();
    }

    void setLowGain(float gain) {
        lowGain = gain;
        recalculateCoeffs();
    }

    void setMidGain(float gain) {
        midGain = gain;
        recalculateCoeffs();
    }

    void setHighGain(float gain) {
        highGain = gain;
        recalculateCoeffs();
    }

    float process(float inputSample) {
        // Применение эквалайзера
        float lowFiltered = lowCoeff * inputSample;
        float midFiltered = midCoeff * inputSample;
        float highFiltered = highCoeff * inputSample;

        // Выходное значение - комбинация трех полос
        return lowFiltered + midFiltered + highFiltered;
    }

    void recalculateCoeffs() {
        // Расчет коэффициентов для фильтрации
        float lowGainLinear = pow(10.0f, lowGain / 20.0f);
        float midGainLinear = pow(10.0f, midGain / 20.0f);
        float highGainLinear = pow(10.0f, highGain / 20.0f);

        lowCoeff = calculateCoeff(lowFrequency);
        midCoeff = calculateCoeff((lowFrequency + highFrequency) / 2.0f);
        highCoeff = calculateCoeff(highFrequency);

        lowCoeff *= lowGainLinear;
        midCoeff *= midGainLinear;
        highCoeff *= highGainLinear;
    }

    float calculateCoeff(float frequency) {
        float omega = 2.0f * M_PI * frequency / sampleRate;
        return (omega / (omega + 1.0f));
    }

    float sampleRate = 44100.0f; // Начальное значение частоты дискретизации
    float lowCoeff = 1.0f;
    float midCoeff = 1.0f;
    float highCoeff = 1.0f;
};
/*
int main() {
    BaxandallEqualizer eq;

    // Изменение частоты дискретизации перед первым вызовом process
    eq.setSampleRate(48000.0f);

    // Установка уровней усиления
    eq.setLowGain(6.0f);   // Усиление низких частот на +6 дБ
    eq.setMidGain(-6.0f);  // Ослабление средних частот на -6 дБ
    eq.setHighGain(3.0f);  // Усиление высоких частот на +3 дБ

    // Пример использования эквалайзера
    float inputSignal = 0.5f; // Входной сигнал
    float outputSignal = eq.process(inputSignal);

    return 0;
}
*/

/*
class BaxandallEqualizer {
public:
    float lowGain;   // Уровень нижней полосы (-90 до +6 dB)
    float midGain;   // Уровень средней полосы (-90 до +6 dB)
    float highGain;  // Уровень верхней полосы (-90 до +6 dB)
    float lowFrequency;  // Частота нижней полосы (Гц)
    float highFrequency; // Частота верхней полосы (Гц)

    BaxandallEqualizer() : lowGain(0), midGain(0), highGain(0), lowFrequency(100), highFrequency(20000) {
    }

    void setSampleRate(float newSampleRate) {
        sampleRate = newSampleRate;
    }

    float process(float inputSample) {
        // Расчет коэффициентов для фильтрации
        float lowGainLinear = pow(10.0f, lowGain / 20.0f);
        float midGainLinear = pow(10.0f, midGain / 20.0f);
        float highGainLinear = pow(10.0f, highGain / 20.0f);

        float lowCoeff = calculateCoeff(lowFrequency);
        float midCoeff = calculateCoeff((lowFrequency + highFrequency) / 2.0f);
        float highCoeff = calculateCoeff(highFrequency);

        // Применение эквалайзера
        float lowFiltered = lowCoeff * lowGainLinear * inputSample;
        float midFiltered = midCoeff * midGainLinear * inputSample;
        float highFiltered = highCoeff * highGainLinear * inputSample;

        // Выходное значение - комбинация трех полос
        return lowFiltered + midFiltered + highFiltered;
    }

private:
    float calculateCoeff(float frequency) {
        float omega = 2.0f * M_PI * frequency / sampleRate;
        return (omega / (omega + 1.0f));
    }

    float sampleRate = 44100.0f; // Начальное значение частоты дискретизации
};


*/

/*
class BaxandallEqualizer {
public:
    float lowGain;   // Уровень нижней полосы (-90 до +6 dB)
    float midGain;   // Уровень средней полосы (-90 до +6 dB)
    float highGain;  // Уровень верхней полосы (-90 до +6 dB)
    float lowFrequency;  // Частота нижней полосы (Гц)
    float highFrequency; // Частота верхней полосы (Гц)

    BaxandallEqualizer() : lowGain(0), midGain(0), highGain(0), lowFrequency(100), highFrequency(10000) {
    }

    float process(float inputSample, float sampleRate) {
        // Расчет коэффициентов для нижней, средней и верхней полос эквалайзера
        float lowGainLinear = pow(10.0f, lowGain / 20.0f);
        float midGainLinear = pow(10.0f, midGain / 20.0f);
        float highGainLinear = pow(10.0f, highGain / 20.0f);

        float lowOmega = 2.0f * M_PI * lowFrequency / sampleRate;
        float midOmega = 2.0f * M_PI * (lowFrequency + highFrequency) / (2 * sampleRate);
        float highOmega = 2.0f * M_PI * highFrequency / sampleRate;

        // Расчет коэффициентов для фильтрации
        float lowCoeff = lowGainLinear / (1.0f + lowGainLinear);
        float midCoeff = midGainLinear / (1.0f + midGainLinear);
        float highCoeff = highGainLinear / (1.0f + highGainLinear);

        // Применение эквалайзера
        float lowFiltered = (1.0f - lowCoeff) * inputSample + lowCoeff * lowFilteredState;
        float midFiltered = (1.0f - midCoeff) * inputSample + midCoeff * midFilteredState;
        float highFiltered = (1.0f - highCoeff) * inputSample + highCoeff * highFilteredState;

        // Обновление состояний фильтров
        lowFilteredState = lowFiltered;
        midFilteredState = midFiltered;
        highFilteredState = highFiltered;

        // Выходное значение - комбинация трех полос
        return lowFiltered + midFiltered + highFiltered;
    }

private:
    float lowFilteredState = 0.0f;
    float midFilteredState = 0.0f;
    float highFilteredState = 0.0f;
};

*/


class Flanger {
 public:
  // Конструктор.
  Flanger(float samplerate) {
    this->samplerate = samplerate;
    delay = 0.3;
    depth = 0.5;
    rate = 1;
  }

  // Устанавливает значение задержки.
  void set_delay(float delay) {
    this->delay = delay;
  }

  // Устанавливает значение глубины.
  void set_depth(float depth) {
    this->depth = depth;
  }

  // Устанавливает значение скорости изменения задержки.
  void set_rate(float rate) {
    this->rate = rate;
  }

  // Обрабатывает сэмпл сигнала.
  float process(float sample) {
    // Обновляем задержку.
    delay += rate / samplerate;

    // Вычисляем задержку в сэмплах.
    int delay_samples = delay * samplerate;

    // Создаем копию входного сигнала.
    std::vector<float> copy = {sample};

    // Применяем задержку к копии сигнала.
    for (int i = 1; i < delay_samples; i++) {
      copy.push_back(sample * delay);
    }

    // Смешиваем основной сигнал с задержанным сигналом.
    float mixed = sample;
    for (float delayed_sample : copy) {
      mixed += delayed_sample * depth;
    }

    // Возвращаем обработанный сэмпл.
    return mixed;
  }

 private:
  // Частота дискретизации.
  float samplerate;

  // Задержка.
  float delay;

  // Глубина.
  float depth;

  // Скорость изменения задержки.
  float rate;
};


class FlangerEffect {
public:

    int buffer_size;  // Размер буфера задержки (можно настроить)

    float samplerate;
    float depth;
    float rate;
    float delay;
    float phase;
    float buffer [96000];
    int index;

    FlangerEffect()
    {

    buffer_size = 96000;

    samplerate = 48000;
    delay = 300.0f;
    depth = 16;

    rate = 256;
    phase = 0.0f;
   //buffer = 0.0f
    index = 0;
    }

    float process(float input) {
        // Рассчитываем задержку в сэмплах
        float delay_samples = delay * 0.001f * samplerate;

        // Вычисляем значение LFO (низкочастотной осцилляции)
//        float lfo = depth * 0.5f * (1.0f - cos(2.0f * M_PI * rate * phase));
        float lfo = depth * (1.0f - cos(3.0f * M_PI * rate * phase));

        // Обновляем фазу LFO
        phase += 1.0f / samplerate;

        // Определяем позицию для чтения из буфера с задержкой
        int read_index = (index - static_cast<int>(delay_samples + lfo)) % buffer_size;
        if (read_index < 0) {
            read_index += buffer_size;
        }

        // Записываем текущий вход в буфер и считываем задержанный сэмпл
        buffer[index] = input;
        float output = buffer[read_index];

        // Обновляем индекс
        index = (index + 1) % buffer_size;

        return output;
    }


};





extern float db_scale;

inline float db2lin (float db)
{
  return (float) exp (db * db_scale);
}

/*
static inline float db2lin (float db)
{
  return powf (10.0f, db / 20);
}
*/

/*
inline float float2db (float v)
{
  if (v == 0.0f)
     return 0.0f;

  if (v > 0.0f)
     return (float) 20 * log10 (v / 1.0f);

  return (float) 20 * log10 (v / -1.0f);
}
*/

void init_db();



///

inline bool float_greater_than (float a, float b)
{
  return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * std::numeric_limits<double>::epsilon());
}


inline bool float_less_than (float a, float b)
{
  return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * std::numeric_limits<double>::epsilon());
}


inline bool float_equal (float x, float y)
{
  return std::abs(x - y) <= std::numeric_limits<double>::epsilon() * std::abs(x);
}

/*
inline float conv (float v, float middle, float max)
{
  if (v == middle)
     return 0;

  if (v > middle)
     return (max - middle - v);
  else
      return middle - v;
}


inline float conv_to_db (float v, float v_min, float v_max, float range_negative, float range_positive)
{
  if (v == 0)
     return 0;

  if (v > 0)
     {
      float x = v_max / range_positive;
      float y = v_max / v;
      return v / (y * x);
     }
  else
      {
       float x = v_min / range_negative;
       float y = v_min / v;

       return v / (y * x);
      }
}
*/

inline float scale_val (float val, float from_min, float from_max, float to_min, float to_max)
{
  return (val - from_min) * (to_max - to_min) /
          (from_max - from_min) + to_min;
}


#define PANLAW_SINCOS 0
#define PANLAW_SQRT 1
#define PANLAW_LINEAR0 2
#define PANLAW_LINEAR6 3
#define PANLAW_SINCOSV2 4


//sin/cos panner, law: -3 dB
#define PANMODE01 1
inline void pan_sincos (float &l, float& r, float p)
{
  float pan = 0.5f * M_PI * p;
  l = cos (pan);
  r = sin (pan);
}


//square root panner, law: -3 dB
#define PANMODE02 2
inline void pan_sqrt (float &l, float& r, float p)
{
  l = sqrt (1 - p);
  r = sqrt (p);
}


//linear panner, law: 0 dB
#define PANMODE03 3
inline void pan_linear0 (float &l, float& r, float p)
{
  l = 0.5f + (1 - p);
  r = 0.5f + p;
}


//linear panner, law: -6 dB
#define PANMODE04 4
inline void pan_linear6 (float &l, float& r, float p)
{
  l = 1 - p;
  r = p;
}


inline void pan_sincos_v2 (float &l, float& r, float p)
{
  float pan = p * M_PI / 2;
  l = l * sin (pan);
  r = r * cos (pan);
}


//power panner, law: -4.5 dB
#define PANMODE05 5
inline void pan_power45 (float &l, float& r, float p)
{
  l  = powf ((1 - p), 0.75) * l;
  r = powf (p, 0.75) * r;
}

//power panner, law: -1.5 dB
//  -1.5dB = 10^(-1.5/20) = 0.841395142 (power taper)
#define PANMODE06 6
inline void pan_power15 (float &l, float& r, float p)
{
  l  = powf ((1 - p), 0.25) * l;
  r = powf (p, 025) * r;
}

//equal power panner, law: -3 dB
//  -3dB = 10^(-3/20) = 0.707945784
#define PANMODE07 7
inline void pan_equal_power3 (float &l, float& r, float p)
{
  l  = sqrt (1 - p) * l; // = power((1-pan),0.5) * MonoIn;
  r = sqrt(p) * r; // = power(pan,0.5) * MonoIn
}



/*************** SATURATION/DIST/OVERDRIVE *******************/

float jimi_fuzz (float inputSample, float level, float distortion);
//float fuzz (float input, float level, float intensity);
float overdrive (float input, float drive, float level);
float gritty_guitar_distortion (float input_sample, float distortion_level);
float warmify (float x, float warmth);


/*************** FILTERS *******************/

float hp_filter (float input, float samplerate, float fc);


/*************** MISC *******************/


float soft_limit (float input);
float apply_resonance (float input, float resonance_amount);


#endif
