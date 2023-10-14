/*
 code is Public Domain and written by Peter Semiletov, 2023
*/

#include "lv2/core/lv2.h"


#include <cmath>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#include "dsp.h"
#include "fx-resofilter.h"

#define METALLUGA_URI "https://github.com/psemiletov/metalluga"

typedef enum { MTL_INPUT = 0, MTL_OUTPUT = 1, MTL_DRIVE = 2, MTL_LEVEL = 3, MTL_TONE = 4, MTL_RESO = 5} PortIndex;



class CMetalluga
{
public:

  int samplerate;

  CResoFilter lp;
  CResoFilter hp;

  const float* drive;
  const float* level;
  const float* tone;
  const float* reso;

  const float* input;
  float *output;

  CMetalluga();
};


CMetalluga::CMetalluga()
{
  hp.mode = FILTER_MODE_HIGHPASS;
  lp.reset();
  hp.reset();
}



float applyResonance(float input, float resonanceAmount)
{
  // Вычисляем коэффициент для изменения резонанса
  float resonanceFactor = 1.0f + resonanceAmount;

    // Применяем резонансное изменение
    float resonantSignal = input * resonanceFactor;

    // Ограничиваем результат в пределах от -1 до 1 (клиппинг)
    if (resonantSignal > 1.0f) {
        resonantSignal = 1.0f;
    } else if (resonantSignal < -1.0f) {
        resonantSignal = -1.0f;
    }

    return resonantSignal;
}


float highPassFilter(float input, float sampleRate, float fc) {
    // Рассчитываем константу времени T из частоты среза
    float T = 1.0f / (2.0f * M_PI * fc);

    // Рассчитываем коэффициент для фильтра
    float alpha = T / (T + 1.0f);

    // Инициализируем предыдущее значение (первоначальное условие)
    static float prevOutput = 0.0f;

    // Применяем фильтр к входному сигналу
    float output = alpha * (prevOutput + input - prevOutput);

    // Обновляем предыдущее значение для следующего вызова
    prevOutput = output;

    return output;
}




// Функция для более "хриплого" гитарного искажения
float grittyGuitarDistortion(float inputSample, float distortionLevel)
{
    // Шаг 1: Усиление с более высоким коэффициентом
  float amplifiedSample = inputSample * (1.5f + 4.0f * distortionLevel);

    // Шаг 2: Операционный усилитель с более высокой нелинейностью
  float opAmpOutput = tanh(2.0f * amplifiedSample);

    // Шаг 3: Клиппер/ограничитель с более сильным искажением
  float clippedSample = tanh(2.0f * opAmpOutput);

    // Ограничиваем значения в диапазоне от 0 до 1
  clippedSample = std::min(1.0f, std::max(0.0f, clippedSample));

  return clippedSample;
}


static void
activate(LV2_Handle instance)
{}


static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
  init_db();
  CMetalluga *instance = new CMetalluga;
  instance->samplerate = rate;

  return (LV2_Handle)instance;
}


static void
connect_port(LV2_Handle instance, uint32_t port, void* data)
{
  CMetalluga *inst = (CMetalluga*)instance;

  switch ((PortIndex)port) {
  case MTL_INPUT:
    inst->input = (const float*)data;
    break;
  case MTL_OUTPUT:
    inst->output = (float*)data;
    break;
  case MTL_DRIVE:
    inst->drive = (const float*)data;
    break;
  case MTL_LEVEL:
    inst->level = (const float*)data;
    break;
  case MTL_TONE:
    inst->tone = (const float*)data;
    break;
  case MTL_RESO:
    inst->reso = (const float*)data;
    break;

  }
}



static void
run(LV2_Handle instance, uint32_t n_samples)
{
  CMetalluga *inst = (CMetalluga*)instance;

  const float* const input  = inst->input;
  float* const       output = inst->output;

  for (uint32_t pos = 0; pos < n_samples; pos++)
      {

       float f = input[pos];

 //   f = highPassFilter(f, inst->tone_filter.samplerate, 7.2f);

       f *= db2lin (*(inst->level));

//     f = highPassFilter(f, inst->session_samplerate, 11.0f);

       f = grittyGuitarDistortion(f, *(inst->drive));

       inst->lp.set_cutoff (1 - *(inst->tone));
       inst->hp.set_cutoff (1 - *(inst->tone));

       f = inst->lp.process (f);
       f = inst->hp.process (f);

       f = applyResonance(f, *(inst->reso));

       output[pos] = f;
   }
}


static void deactivate (LV2_Handle instance)
{
}


static void cleanup(LV2_Handle instance)
{
  delete (CMetalluga*)instance;
}


static const void* extension_data (const char* uri)
{
  return NULL;
}


static const LV2_Descriptor descriptor = {METALLUGA_URI,
                                          instantiate,
                                          connect_port,
                                          activate,
                                          run,
                                          deactivate,
                                          cleanup,
                                          extension_data};


LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor (uint32_t index)
{
  return index == 0 ? &descriptor : NULL;
}
